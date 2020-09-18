#include <nokia_gfx.h>
#include <stdlib.h>



inline void _swap_uint8_t(uint8_t *a, uint8_t *b) {   
  *a+=*b;
  *b = *a - *b;
  *a-=*b;
}

void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color){
  drawLine(x, y, x + w - 1, y, color);
}

void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color){
  drawLine(x, y, x, y + h - 1, color);
}

void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color){
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}

void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {

  uint8_t steep  = abs(y1 - y0) > abs(x1 - x0);
  
  if (steep) {
    _swap_uint8_t(&x0, &y0);
    _swap_uint8_t(&x1, &y1);
  }
  
  if (x0 > x1) {
    _swap_uint8_t(&x0, &x1);
    _swap_uint8_t(&y0, &y1);
  }

  int8_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      nokia_draw_pixel(y0, x0, color);
    } else {
      nokia_draw_pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}


void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  nokia_draw_pixel(x0, y0 + r, color);
  nokia_draw_pixel(x0, y0 - r, color);
  nokia_draw_pixel(x0 + r, y0, color);
  nokia_draw_pixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    nokia_draw_pixel(x0 + x, y0 + y, color);
    nokia_draw_pixel(x0 - x, y0 + y, color);
    nokia_draw_pixel(x0 + x, y0 - y, color);
    nokia_draw_pixel(x0 - x, y0 - y, color);
    nokia_draw_pixel(x0 + y, y0 + x, color);
    nokia_draw_pixel(x0 - y, y0 + x, color);
    nokia_draw_pixel(x0 + y, y0 - x, color);
    nokia_draw_pixel(x0 - y, y0 - x, color);
  }
}


void drawBitmap(uint8_t x, uint8_t y, const uint8_t bitmap[], uint8_t w, uint8_t h, uint8_t color){
  uint8_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  for (uint8_t j = 0; j < h; j++, y++) {
    for (uint8_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = bitmap[j * byteWidth + i / 8];
      if (byte & 0x80)
        nokia_draw_pixel(x + i, y, color);
    }
  }
}
