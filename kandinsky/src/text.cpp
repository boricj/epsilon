#include <kandinsky/text.h>
#include <string.h>
#include <ion/charset.h>
#include "small_font.h"
#include "large_font.h"

KDSize KDText::stringSize(const char * text, FontSize size) {
  if (text == nullptr) {
    return KDSizeZero;
  }
  KDSize commonCharSize = charSize(size);
  KDSize stringSize = KDSize(0, commonCharSize.height());
  while (*text != 0) {
    KDSize cSize = KDSize(commonCharSize.width(), 0);
    if (*text == '\t') {
      cSize = KDSize(k_tabCharacterWidth*commonCharSize.width(), 0);
    }
    else if (*text == '\n') {
      cSize = KDSize(0, commonCharSize.height());
    }
    else if (*text >= Ion::Charset::DiacriticalFirst && *text <= Ion::Charset::DiacriticalLast) {
      cSize = KDSize(0, 0);
    }
    stringSize = KDSize(stringSize.width()+cSize.width(), stringSize.height()+cSize.height());
    text++;
  }
  return stringSize;
}
