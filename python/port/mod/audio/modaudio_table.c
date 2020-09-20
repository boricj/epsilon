#include "modaudio.h"

STATIC MP_DEFINE_CONST_FUN_OBJ_1(modaudio_init_obj, modaudio_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(modaudio_shutdown_obj, modaudio_shutdown);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(modaudio_queue_obj, modaudio_queue);

STATIC const mp_rom_map_elem_t modaudio_module_globals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_audio) },
  { MP_ROM_QSTR(MP_QSTR_init), (mp_obj_t)&modaudio_init_obj },
  { MP_ROM_QSTR(MP_QSTR_shutdown), (mp_obj_t)&modaudio_shutdown_obj },
  { MP_ROM_QSTR(MP_QSTR_queue), (mp_obj_t)&modaudio_queue_obj },
};

STATIC MP_DEFINE_CONST_DICT(modaudio_module_globals, modaudio_module_globals_table);

const mp_obj_module_t modaudio_module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&modaudio_module_globals,
};
