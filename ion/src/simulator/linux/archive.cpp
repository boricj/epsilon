#include <ion/archive.h>
#include <ion/display.h>

#include <SDL.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <string>

namespace Ion {
namespace Archive {

constexpr int MaxEntries = 40;
static File sEntries[MaxEntries];
static void* sObjetEntries[MaxEntries];

static size_t sNumberOfEntries;

static void loadEntries() {
  if (sNumberOfEntries > 0) {
    return;
  }

  DIR *dir = opendir(".");
  struct dirent *dp;
  while ((dp = readdir(dir)) != nullptr) {
    if (dp->d_type == DT_REG) {
      File entry;

      // Grab information about file.
      struct stat fpStat;
      int fp = open(dp->d_name, O_RDONLY);
      fstat(fp, &fpStat);

      // Load file.
      strlcpy(entry.name, dp->d_name, MaxNameLength);
      entry.data = reinterpret_cast<const uint8_t*>(mmap(nullptr, fpStat.st_size, PROT_READ|PROT_EXEC, MAP_PRIVATE, fp, 0));
      entry.dataLength = entry.data ? fpStat.st_size : 0;
      entry.isExecutable = false;

      sObjetEntries[sNumberOfEntries] = SDL_LoadObject((std::string("./")+entry.name).c_str());
      if (sObjetEntries[sNumberOfEntries] && SDL_LoadFunction(sObjetEntries[sNumberOfEntries], "numworks_main") != nullptr) {
        entry.isExecutable = true;
        entry.icon = *reinterpret_cast<const Image**>(SDL_LoadFunction(sObjetEntries[sNumberOfEntries], "_ZN10ImageStore4IconE"));
      }

      close(fp);
      sEntries[sNumberOfEntries++] = entry;

      if (sNumberOfEntries == (MaxNameLength-1)) {
        break;
      }
    }
  }

  closedir(dir);
}

bool fileAtIndex(size_t index, File &entry) {
  loadEntries();

  if (index >= sNumberOfEntries) {
    return false;
  }

  entry = sEntries[index];
  return true;
}

void executeFile(const char *name) {
  KDIonContext::sharedContext()->setOrigin(KDPointZero);
  KDIonContext::sharedContext()->setClippingRect(KDRect(KDPointZero, KDSize(Ion::Display::Width, Ion::Display::Height)));
  void(*fn)() = reinterpret_cast<void(*)()>(SDL_LoadFunction(sObjetEntries[indexFromName(name)], "numworks_main"));
  fn();
}

}
}
