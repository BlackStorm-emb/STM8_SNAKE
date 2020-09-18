#include <nokia_fastGFX.h>
#include <nokia_gfx.h>
#include <nokia.h>
#include <fontNumbers.h>

#define FontK 10

void drawScore(const uint8_t x, const uint8_t y, const uint16_t score){
  if (score > 999) return;
  
  uint8_t hun = score / 100;
  uint8_t dec = (score % 100) / 10;
  uint8_t one = score % 10;
  
  uint8_t size = 3;
  
  if (!hun) {
    if (!dec) size = 1;
    else size = 2;
  }
  
  uint8_t inc_pos = 0;
  
  if (size == 3){
        drawBitmap(x, y, &Font[hun*FontK], 6, 10, 1);
        inc_pos = 8;
  }
  if (size == 2) {
    drawBitmap(x + inc_pos, y, &Font[dec*FontK], 6, 10, 1);
    inc_pos += 8; 
  }
  drawBitmap(x + inc_pos, y, &Font[one*FontK], 6, 10, 1);
  //
  //
}