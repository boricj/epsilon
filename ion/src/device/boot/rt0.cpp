extern "C" {
#include "rt0.h"
}
#include <stdint.h>
#include <string.h>
#include <ion.h>
#include "../device.h"

typedef void (*cxx_constructor)();

extern "C" {
  char _data_section_start_flash;
  char _data_section_start_ram;
  char _data_section_end_ram;
  char _bss_section_start_ram;
  char _bss_section_end_ram;
  cxx_constructor _init_array_start;
  cxx_constructor _init_array_end;
}

void abort() {
#ifdef DEBUG
  while (1) {
  }
#else
  Ion::reset();
#endif
}

void start() {
  // This is where execution starts after reset.
  // Many things are not initialized yet so the code here has to pay attention.

  /* Copy data section to RAM
   * The data section is R/W but its initialization value matters. It's stored
   * in Flash, but linked as if it were in RAM. Now's our opportunity to copy
   * it. Note that until then the data section (e.g. global variables) contains
   * garbage values and should not be used. */
  size_t dataSectionLength = (&_data_section_end_ram - &_data_section_start_ram);
  memcpy(&_data_section_start_ram, &_data_section_start_flash, dataSectionLength);

  /* Zero-out the bss section in RAM
   * Until we do, any uninitialized global variable will be unusable. */
  size_t bssSectionLength = (&_bss_section_end_ram - &_bss_section_start_ram);
  memset(&_bss_section_start_ram, 0, bssSectionLength);

  /* Call static C++ object constructors
   * The C++ compiler creates an initialization function for each static object.
   * The linker then stores the address of each of those functions consecutively
   * between _init_array_start and _init_array_end. So to initialize all C++
   * static objects we just have to iterate between theses two addresses and
   * call the pointed function. */
  for (cxx_constructor * c = &_init_array_start; c<&_init_array_end; c++) {
    (*c)();
  }

  Ion::Device::init();

  ion_app();

  abort();
}
