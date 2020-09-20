extern "C" {
#include "modaudio.h"
#include <py/runtime.h>
}
#include <ion.h>

mp_obj_t modaudio_init(mp_obj_t freq) {
  Ion::Audio::init(mp_obj_get_int(freq));
  return mp_const_none;
}

mp_obj_t modaudio_shutdown() {
  Ion::Audio::shutdown();
  return mp_const_none;
}

mp_obj_t modaudio_queue(mp_obj_t sample) {
  mp_float_t x = mp_obj_get_float(sample);
  return mp_obj_new_int(Ion::Audio::queue(x));
}
