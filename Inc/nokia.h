#ifndef NOKIA_H
#define NOKIA_H

#include <stdint.h>
#include <stdbool.h>

#define FAST 1
#define SLOW 0

#define _width 84
#define _height 48

void nokia_init();
void nokia_setpos(uint8_t page, uint8_t x);
void nokia_clear(); //clear display RAM
void nokia_clearBuffer();
void nokia_clearBufArea(uint8_t start, uint8_t end);
void nokia_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
uint8_t nokia_read_pixel(uint8_t x, uint8_t y);
void nokia_update();


#endif