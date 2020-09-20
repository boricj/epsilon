#ifndef ION_DEVICE_BOOT_ISR_H
#define ION_DEVICE_BOOT_ISR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void start();
void abort();
void isr_systick();
void isr_tim2();

#define AUDIO_BUFFER_SIZE 512

extern volatile size_t audioReadPointer;
extern volatile size_t audioWritePointer;
extern volatile uint32_t audioBuffer[AUDIO_BUFFER_SIZE];

#ifdef __cplusplus
}
#endif

#endif
