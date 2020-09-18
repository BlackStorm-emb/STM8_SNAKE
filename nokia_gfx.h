#ifndef NOKIA_GFX
#define NOKIA_GFX

#include <nokia.h>
#include <stdint.h>

void drawLine(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, uint8_t color);
void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

void drawBitmap(uint8_t x, uint8_t y, const uint8_t bitmap[], uint8_t w, uint8_t h, uint8_t color);

void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color);
void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color);


#endif