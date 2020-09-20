#include <py/obj.h>

mp_obj_t modaudio_init(mp_obj_t freq);
mp_obj_t modaudio_shutdown(void);
mp_obj_t modaudio_queue(mp_obj_t sample);
