#ifndef ION_AUDIO_H
#define ION_AUDIO_H

#include <stddef.h>
#include <stdint.h>

namespace Ion {
namespace Audio {

void init(size_t frequency);
void shutdown();
size_t queue(float sample);

}
}

#endif
