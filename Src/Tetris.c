#include <Tetris.h>
#include <iostm8s103f3.h>
#include <nokia.h>
#include <nokia_gfx.h>
#include <nokia_fastGFX.h>
#include <Buttons.h>
#include <tim4millis.h>
#include <stdlib.h>
#include <stdlib.h>

#define LEFT_BORDER 0
#define RIGHT_BORDER 55
#define UP_BORDER 0
#define DOWN_BORDER 47

const uint32_t update_interval = 200;
const uint32_t update_interval_button = 20;

struct additional_data {
  uint8_t IsDirectionChanged : 1;
  uint8_t speed : 3;
  uint8_t needApple : 1;
  uint8_t unused : 3;
};

void start_tetris(void) {
  uint8_t x_shape, y_shape;
  
  uint32_t current_time, prev_time1, prev_time2;
  
  current_time = prev_time1 = prev_time2 = millis();
  
  KEY_STATE current_key = None, prev_key = None;
  
  struct additional_data states;
  states.speed = 7;
  
  while (1) {
    current_time =  millis();
      
    current_key = read_button();
    
    if (current_time - prev_time1 >= update_interval_button) {
        
        if (current_key == None && prev_key != None && !states.IsDirectionChanged) {
          switch(prev_key){
          case RIGHT_KEY: {
          
          }
            break;
          case LEFT_KEY: {
            
            }
            break;
          case UP_KEY: {
            
            }
            break;
          case DOWN_KEY: {
            
            }
            break;
          }
          
          states.IsDirectionChanged = 1;
          
        }
        prev_key = current_key;
      }
      
    if (current_time - prev_time2 >= update_interval - states.speed * 10) { 
      
    }
    
    nokia_clearBuffer();
    drawRect(LEFT_BORDER, UP_BORDER, RIGHT_BORDER,  DOWN_BORDER, 1); //draw play Area
    nokia_update();
  }
}