#include <iostm8s103f3.h>
#include <Snake.h>
#include <PingPong.h>
#include <Tetris.h>
#include <nokia.h>
#include <nokia_gfx.h>
#include <Buttons.h>
#include <tim4millis.h>
#include <stdlib.h>
#include <stdint.h>

const uint8_t button_interval = 50;
const uint16_t blink_interval = 300;

const uint8_t menu_sprite_84x48[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0x83, 0xC0, 0x40, 0x10, 0x40, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x04, 0x82, 0x20, 0xC0, 0x10, 0x20, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x20, 0x40, 0x28, 0xA0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0xC0, 0x40, 0x28, 0x90, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x20, 0x40, 0x20, 0x10, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x20, 0x40, 0x10, 0x20, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x03, 0xC0, 0xE0, 0x0F, 0xE0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x30, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x44, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x8F, 0x8A, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x80, 0x0A, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x1A, 0x00, 
	0x00, 0x0F, 0x36, 0x31, 0x25, 0x9C, 0x00, 0x00, 0x3F, 0xE4, 0x00, 
	0x00, 0x01, 0x36, 0x49, 0x67, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x07, 0x2A, 0x79, 0xE7, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x2A, 0x41, 0xA5, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0F, 0x22, 0x39, 0x24, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0F, 0x24, 0x93, 0xC0, 0x78, 0xC4, 0x9E, 0x00, 0x00, 0x00, 
	0x00, 0x09, 0x2C, 0x92, 0x00, 0x49, 0x24, 0x90, 0x00, 0x00, 0x00, 
	0x00, 0x09, 0x3C, 0xF2, 0x1F, 0x49, 0x27, 0x90, 0x00, 0x00, 0x00, 
	0x00, 0x09, 0x34, 0x92, 0x00, 0x49, 0x24, 0x90, 0x00, 0x00, 0x00, 
	0x00, 0x09, 0x24, 0x92, 0x00, 0x48, 0xC4, 0x90, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0F, 0x8C, 0x7C, 0xE2, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x12, 0x10, 0x92, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x1E, 0x10, 0x93, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x10, 0x10, 0x93, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x0E, 0x10, 0xE2, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const uint8_t arrow_sprite_9x5[] = {
	0x02, 0x00, 
	0x03, 0x00, 
	0xFF, 0x80, 
	0x03, 0x00, 
	0x02, 0x00, 
};

typedef enum Menu_enum {
  None_menu,
  Snake,
  PingPong,
  Tetris
} Menu_typedef;

struct additional_data {
  uint8_t IsStart : 1;
  uint8_t IsBlink : 1;
  uint8_t a : 7;
} MenuStates;

void drawMenu(Menu_typedef menu_code) {
  nokia_clearBuffer();
  drawRect(0,0, 84, 48, 1);
  drawBitmap(0, 0, menu_sprite_84x48, 84, 48, 1); 
  
  switch (menu_code) {
    case Snake: drawBitmap(2, 19, arrow_sprite_9x5, 9, 5, 1);
      break;
    case PingPong: drawBitmap(2, 27, arrow_sprite_9x5, 9, 5, 1);
      break;
    case Tetris: drawBitmap(2, 35, arrow_sprite_9x5, 9, 5, 1);
      break;
  }
  
  nokia_update();
}

void main( void )
{
  uint32_t current_time, prev_time1, prev_time2;
  
  CLK_CKDIVR = 0x00; //16 MHz
  
  TIM4_init();
  init_buttons();
  nokia_init();
  nokia_clear();
  
  KEY_STATE current_key = None, prev_key = None;
  
  Menu_typedef menu_code = Snake;
  MenuStates.IsStart = false;
  MenuStates.IsBlink = true;
  
  prev_time1 = prev_time2 = current_time = millis();
  
  while(1) {
    current_time = millis();
    
    current_key = read_button();
    
    if (current_time - prev_time2 >= blink_interval){
      prev_time2 = millis();
      MenuStates.IsBlink = ~MenuStates.IsBlink;
    }
    
    if (current_time - prev_time1 >= button_interval) {
      prev_time1 = millis();
      
      if (current_key == None && prev_key != None) {
        switch(prev_key) {
          case UP_KEY: {
            if (menu_code != Snake) menu_code--;
          }
            break;
          case DOWN_KEY: {
            if (menu_code != Tetris) menu_code++;
          }
            break;
          case RIGHT_KEY: {
            MenuStates.IsStart = true;
          }
            break;
        }
      }
      
     prev_key = current_key;
    }
    
    if (MenuStates.IsStart){
      MenuStates.IsStart = false;
      switch (menu_code){
        case Snake: start_snake();
          break;
        case PingPong: start_pingPong();
          break;
        case Tetris: //start_tetris();
          break;
      }
    }
    else if (MenuStates.IsBlink) drawMenu(menu_code);
    else drawMenu(None_menu);
    
  }

}
