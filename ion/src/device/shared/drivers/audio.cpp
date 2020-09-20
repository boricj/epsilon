#include <ion/audio.h>
#include <regs/regs.h>
#include <boot/isr.h>
#include <drivers/config/clocks.h>
#include <string.h>

namespace Ion {
namespace Audio {

using namespace Ion::Device::Regs;

static float integrator;
static float previousSample;
static float volume;

void init(size_t frequency) {
    audioReadPointer = 0;
    audioWritePointer = 0;
    memset((void*)audioBuffer, sizeof(audioBuffer), 0);
    integrator = 0;
    previousSample = 0;
    volume = 0.75;

    RCC.APB1ENR()->setTIM2EN(true);
    NVIC.NVIC_ISER0()->set(28, true);

    uint32_t ticks = Ion::Device::Clocks::Config::HCLKFrequency/Ion::Device::Clocks::Config::APB1Prescaler * 1000000 / frequency;

    TIM2.PSC()->set(ticks / 65536);
    TIM2.ARR()->set(ticks % 65536);
    TIM2.CR1()->setARPE(true);
    TIM2.EGR()->setUG(true);
    TIM2.DIER()->setUIE(true);
    TIM2.CR1()->setCEN(true);

    asm ("CPSIE i");
}

void shutdown() {
    NVIC.NVIC_ICER0()->set(28, true);

    TIM2.CR1()->setCEN(false);
    TIM2.SR()->setUIF(false);
    
}

constexpr float interpolate(float a, float b, float x) {
    return a + (b-a) * x;
}

constexpr size_t length(size_t start, size_t end, size_t capacity) {
    return capacity - ((end >= start) ? (end - start) : (capacity - start + end));
}

size_t queue(float sample) {
    // With a 37.5 Omh headphone and ~1.9v output voltage, scaling is required
    // to not exceed 25mA maximal rating of a single GPIO pin (and the human's
    // hearing too).
//    sample *= 0.25;

//    sample *= volume;

    uint32_t out = 0;
    float previousLevel = (integrator > 0) ? 1 : -1;

    for (size_t i = 0; i < 32; i++) {
        integrator += interpolate(previousSample, sample, i/31.0) - previousLevel;
        if (integrator > 0) {
            previousLevel = 1;
            out |= 1 << i;
        }
        else {
            previousLevel = -1;
        }
    }
    previousSample = sample;

    while (length(audioReadPointer, audioWritePointer, AUDIO_BUFFER_SIZE*32) < 32);
    audioBuffer[audioWritePointer/32] = out;
    audioWritePointer = (audioWritePointer + 32) % (AUDIO_BUFFER_SIZE*32);
    return length(audioReadPointer, audioWritePointer, AUDIO_BUFFER_SIZE*32) / 32;
}

}
}
