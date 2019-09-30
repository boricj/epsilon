#include <ion/archive.h>

#include <string.h>
#include <stdlib.h>
#include "elf.h"

namespace Ion {
namespace Archive {

struct TarHeader
{                              /* byte offset */
  char name[100];               /*   0 */
  char mode[8];                 /* 100 */
  char uid[8];                  /* 108 */
  char gid[8];                  /* 116 */
  char size[12];                /* 124 */
  char mtime[12];               /* 136 */
  char chksum[8];               /* 148 */
  char typeflag;                /* 156 */
  char linkname[100];           /* 157 */
  char magic[8];                /* 257 */
  char uname[32];               /* 265 */
  char gname[32];               /* 297 */
  char devmajor[8];             /* 329 */
  char devminor[8];             /* 337 */
  char padding[167];            /* 345 */
} __attribute__((packed));

static_assert(sizeof(TarHeader) == 512);

bool fileAtIndex(size_t index, File &entry) {
  const TarHeader* tar = reinterpret_cast<const TarHeader*>(0x90000000);
  unsigned size = 0;

  /**
   * TAR files are comprised of a set of records aligned to 512 bytes boundary
   * followed by data. First record is always us, so don't bother checking its
   * vailidy.
   */
  while (index-- > 0) {
    size = 0;
    for (int i = 0; i < 11; i++)
      size = size * 8 + (tar->size[i] - '0');

    // Move to the next TAR header.
    unsigned stride = (sizeof(TarHeader) + size + 511);
    stride = (stride >> 9) << 9;
    tar = reinterpret_cast<const TarHeader*>(reinterpret_cast<const char*>(tar) + stride);

    // Sanity check.
    if (memcmp(tar->magic, "ustar  ", 8) || tar->name[0] == '\x00' || tar->name[0] == '\xFF')
        return false;
  }

  // File entry found, copy data out.
  strlcpy(entry.name, tar->name, MaxNameLength);
  entry.data = reinterpret_cast<const uint8_t*>(tar) + sizeof(TarHeader);
  entry.dataLength = size;

  const Elf32_Ehdr *hdr = reinterpret_cast<const Elf32_Ehdr*>(entry.data);
  entry.isExecutable = hdr->e_ident[EI_MAG0] == ELFMAG0 &&
    hdr->e_ident[EI_MAG1] == ELFMAG1 &&
    hdr->e_ident[EI_MAG2] == ELFMAG2 &&
    hdr->e_ident[EI_MAG3] == ELFMAG3 &&
    hdr->e_ident[EI_CLASS] == ELFCLASS32 &&
    hdr->e_ident[EI_DATA] == ELFDATA2LSB &&
    hdr->e_type == ET_EXEC &&
    hdr->e_machine == EM_ARM;

  entry.icon = nullptr;

  return true;
}

template <typename T, typename src> static T* load_from(src* base, const Elf32_Phdr* hdr, T* ptr) {
  return reinterpret_cast<T*>(base - hdr->p_vaddr + reinterpret_cast<uint32_t>(ptr));
}

void executeFile(const char *name) {
  File entry;
  fileAtIndex(indexFromName(name), entry);
  
  const Elf32_Ehdr *hdr = reinterpret_cast<const Elf32_Ehdr *>(entry.data);
  const Elf32_Phdr *phdrs = reinterpret_cast<const Elf32_Phdr *>(entry.data+hdr->e_phoff);

  if (hdr->e_phnum != 2)
    return;
  
  const Elf32_Phdr *phdr_ro = &phdrs[0];
  const Elf32_Phdr *phdr_rw = &phdrs[1];

  if ((phdr_ro->p_type != PT_LOAD || phdr_ro->p_flags != (PF_R | PF_X)) ||
      (phdr_rw->p_type != PT_LOAD || phdr_rw->p_flags != (PF_R | PF_W))) {
    return;
  }

  // Load segment into RAM.
  uint8_t *load_rw = reinterpret_cast<uint8_t*>(malloc(phdr_rw->p_memsz));
  memset(load_rw, 0, phdr_rw->p_memsz);
  memcpy(load_rw, entry.data + phdr_rw->p_offset, phdr_rw->p_filesz);
  uint32_t* load_got = nullptr;

  // Deal with global offset table.
  const Elf32_Shdr *shdrs = reinterpret_cast<const Elf32_Shdr *>(entry.data + hdr->e_shoff);
  const Elf32_Shdr *shdr_shstrtab = &shdrs[hdr->e_shstrndx];

  for (int i = 0; i < hdr->e_shnum; i++) {
    const Elf32_Shdr *shdr = &shdrs[i];
    const char *shdr_name = reinterpret_cast<const char*>(&entry.data[shdr_shstrtab->sh_offset + shdr->sh_name]);
    if (strcmp(shdr_name, ".got") == 0) {
      load_got = load_from(load_rw, phdr_rw, reinterpret_cast<uint32_t*>(shdr->sh_addr));
      uint32_t* load_got_end = load_got + shdr->sh_size / sizeof(uint32_t);  

      while (load_got < load_got_end) {
        if ((*load_got > phdr_ro->p_offset) && (*load_got < (phdr_ro->p_offset + phdr_ro->p_memsz))) {
          *load_got = (uint32_t)load_from(&entry.data[phdr_ro->p_offset], phdr_ro, (const uint32_t*)(*load_got));
        }
        else if ((*load_got > phdr_rw->p_offset) && (*load_got < (phdr_rw->p_offset + phdr_rw->p_memsz))) {
          *load_got = (uint32_t)load_from(load_rw, phdr_rw, (uint32_t*)(*load_got));
        }

        load_got++;
      }
    }
  }

  // Here we go!
  void (*entryPoint)() = load_from(&entry.data[phdr_ro->p_offset], phdr_ro, reinterpret_cast<void (*)()>(hdr->e_entry));

  register uint32_t* got_reg asm("r9");
  uint32_t* old_got = got_reg;
  got_reg = load_got;
  entryPoint();
  got_reg = old_got;

  free(load_rw);
  return;
}

}
}