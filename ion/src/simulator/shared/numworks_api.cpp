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

void pushRect(Vec2D location, Vec2D size, const uint16_t * pixels) {
  KDRect rect(location.x, location.y, size.x, size.y);

  Ion::Display::pushRect(rect, reinterpret_cast<const KDColor*>(pixels));
}

void pushRectUniform(Vec2D location, Vec2D size, uint16_t color) {
  KDRect rect(location.x, location.y, size.x, size.y);

  Ion::Display::pushRectUniform(rect, KDColor::RGB16(color));
}

void drawTextLarge(Vec2D location, const char *text, uint16_t fg, uint16_t bg) {
  KDPoint point(location.x, location.y);

  KDIonContext::sharedContext()->drawString(text, point, KDFont::LargeFont, KDColor::RGB16(fg), KDColor::RGB16(bg));
}

void drawTextSmall(Vec2D location, const char *text, uint16_t fg, uint16_t bg) {
  KDPoint point(location.x, location.y);

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
