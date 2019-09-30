#ifndef ION_ARCHIVE_H
#define ION_ARCHIVE_H

#include <escher/image.h>

#include <stddef.h>
#include <stdint.h>

namespace Ion {
namespace Archive {

constexpr int MaxNameLength = 40;

struct File {
    char name[MaxNameLength];
    const uint8_t *data;
    size_t dataLength;
    const Image *icon;
    bool isExecutable;
} ;

bool fileAtIndex(size_t index, File &entry);
int indexFromName(const char *name);
size_t numberOfFiles();
void executeFile(const char *name);

}
}

#endif
