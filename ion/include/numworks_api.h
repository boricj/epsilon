#ifndef ION_EXTERNAL_API_H
#define ION_EXTERNAL_API_H

/**
 * This is the API for external add-ins. This file must be self-contained as we
 * both don't want to taint add-ins with epsilon's license nor expose internals
 * to them.
 */

#include <stdint.h>
#include <stddef.h>

namespace NumWorks {

constexpr int ScreenWidth = 320;
constexpr int ScreenHeight = 240;

constexpr int LargeFontWidth = 10;
constexpr int LargeFontHeight = 18;
constexpr int SmallFontWidth = 7;
constexpr int SmallFontHeight = 14;

constexpr int MaxNameLength = 40;

enum class Key : uint8_t {
  Left=0,  Up=1,     Down=2,    Right=3,           OK=4,              Back=5,
  Home=6,  OnOff=7,  /*B3= 8,   B4=9,            B5=10,             B6=11, */
  Shift=12,Alpha=13, XNT=14,    Var=15,            Toolbox=16,        Backspace=17,
  Exp=18,  Ln=19,    Log=20,    Imaginary=21,      Comma=22,          Power=23,
  Sine=24, Cosine=25,Tangent=26,Pi=27,             Sqrt=28,           Square=29,
  Seven=30,Eight=31, Nine=32,   LeftParenthesis=33,RightParenthesis=34,// F6=35,
  Four=36, Five=37,  Six=38,    Multiplication=39, Division=40,        // G6=41,
  One=42,  Two=43,   Three=44,  Plus=45,           Minus=46,           // H6=47,
  Zero=48, Dot=49,   EE=50,     Ans=51,            EXE=52,             // I6=53,
  None = 54
};

struct Vec2D {
  int16_t x, y;
};

void* allocMem(size_t length);
void freeMem(void *ptr);

uint64_t millis();
void msleep(uint32_t ms);

uint64_t scanKeyboard();

void pushRect(Vec2D location, Vec2D size, const uint16_t * pixels);
void pushRectUniform(Vec2D location, Vec2D size, uint16_t color);
void drawTextLarge(Vec2D location, const char *text, uint16_t fg, uint16_t bg);
void drawTextSmall(Vec2D location, const char *text, uint16_t fg, uint16_t bg);
void waitForVBlank();

bool fileAtIndex(size_t index, char *name, const uint8_t **data, size_t *dataLength);
int fileIndexFromName(const char *name);
size_t numberOfFiles();

}

#endif