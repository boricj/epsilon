#include <ion.h>
#include <numworks_api.h>
#include <kandinsky.h>

#include <cstdlib>

namespace NumWorks {

void* allocMem(size_t length) {
  return malloc(length);
}

void freeMem(void *ptr) {
  free(ptr);
}

uint64_t millis() {
  return Ion::Timing::millis();
}

void msleep(uint32_t ms) {
  Ion::Timing::msleep(ms);
}

uint64_t scanKeyboard() {
  return Ion::Keyboard::scan();
}

void pushRect(int16_t x, int16_t y, uint16_t w, uint16_t h, const uint16_t * pixels) {
  KDRect rect(x, y, w, h);

  Ion::Display::pushRect(rect, reinterpret_cast<const KDColor*>(pixels));
}

void pushRectUniform(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color) {
  KDRect rect(x, y, w, h);

  Ion::Display::pushRectUniform(rect, KDColor::RGB16(color));
}

void drawTextLarge(int16_t x, int16_t y, const char *text, uint16_t fg, uint16_t bg) {
  KDPoint point(x, y);

  KDIonContext::sharedContext()->drawString(text, point, KDFont::LargeFont, KDColor::RGB16(fg), KDColor::RGB16(bg));
}

void drawTextSmall(int16_t x, int16_t y, const char *text, uint16_t fg, uint16_t bg) {
  KDPoint point(x, y);

  KDIonContext::sharedContext()->drawString(text, point, KDFont::SmallFont, KDColor::RGB16(fg), KDColor::RGB16(bg));
}

void waitForVBlank() {
  Ion::Display::waitForVBlank();
}

bool fileAtIndex(size_t index, char *name, const uint8_t **data, size_t *dataLength) {
  Ion::Archive::File entry;
  bool found = Ion::Archive::fileAtIndex(index, entry);

  if (found) {
    strlcpy(name, entry.name, MaxNameLength);
    *data = entry.data;
    *dataLength = entry.dataLength;
  }

  return found;
}

int fileIndexFromName(const char *name) {
  return Ion::Archive::indexFromName(name);
}

size_t numberOfFiles() {
  return Ion::Archive::numberOfFiles();
}

}
