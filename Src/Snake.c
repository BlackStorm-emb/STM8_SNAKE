#include <Snake.h>
#include <iostm8s103f3.h>
#include <nokia.h>
#include <nokia_gfx.h>
#include <nokia_fastGFX.h>
#include <Buttons.h>
#include <tim4millis.h>
#include <stdlib.h>
#include <stdlib.h>

#define _step 4

#define LEFT_BORDER 1
#define RIGHT_BORDER 82
#define UP_BORDER 0
#define DOWN_BORDER 45

const uint8_t Snake_Sprite_4x4[]  = {
	0xF0, 
	0xF0, 
	0xF0, 
	0xF0, 
};

const uint8_t Apple_Sprite_4x4[] = {
	0x60, 
	0x90, 
	0x90, 
	0x60, 
};

const uint32_t update_interval = 200;
const uint32_t update_interval_button = 20;

typedef enum Direction_enum {
  Right = 0,
  Down,
  Left,
  Up
} Direction_typedef;

typedef enum Events_enum {
  No,
  Collision
} Events_typedef;

struct additional_data {
  uint8_t IsDirectionChanged : 1;
  uint8_t speed : 3;
  uint8_t needApple : 1;
  uint8_t unused : 3;
} states;

inline Events_typedef checkBorder(uint8_t x, uint8_t y) {
  x *= _step;
  y *= _step;
  x += LEFT_BORDER + 1;
  y += UP_BORDER + 1;
  if (x >= RIGHT_BORDER || x <= LEFT_BORDER || y >= DOWN_BORDER || y <= UP_BORDER)
    return Collision;
  return No;
}

inline Events_typedef checkSnakeCollision(uint8_t x, uint8_t y){
  x *= _step;
  y *= _step;
  x += LEFT_BORDER + 1;
  y += UP_BORDER + 1;
  if (nokia_read_pixel(x, y)) return Collision;
  return No;
}

inline void write_snakeMas(uint8_t *snake, const uint8_t index, const Direction_typedef direction){
  uint8_t MasIndex = index / 4, offset = index % 4;
  snake[MasIndex] &= ~(0x03 << offset * 2);
  snake[MasIndex] |= ((uint8_t)direction << offset * 2);
}

inline Direction_typedef read_snakeMas(uint8_t *snake, const uint8_t index) {
  uint8_t MasIndex = index / 4, offset = index % 4;
  return (Direction_typedef)( (snake[MasIndex] >> offset * 2) & 0x03);
}

inline void move_snake(uint8_t *snake, const uint8_t score, const Direction_typedef new_direction){
  for (uint8_t i = score - 2; i > 0; i--){
    write_snakeMas(snake, i, read_snakeMas(snake, i - 1));
  }
  write_snakeMas(snake, 0, new_direction);
}

inline void draw_snake(uint8_t x, uint8_t y, uint8_t *snake, const uint8_t score){
  x *= _step;
  y *= _step;
  x += LEFT_BORDER + 1;
  y += UP_BORDER + 1;
  drawBitmap(x, y, Snake_Sprite_4x4, 4, 4, 1);
  for (uint8_t i = 0; i <= score - 2; i++) {
    Direction_typedef direction = read_snakeMas(snake, i);
    switch (direction) {
      case Right: x += _step;
        break;
      case Left: x -= _step;
        break;
      case Down: y += _step;
        break;
      case Up: y -= _step;
        break; 
    }
    drawBitmap(x, y, Snake_Sprite_4x4, 4, 4, 1);
  }
}

inline void generate_apple(uint8_t *x, uint8_t *y) { 
  uint8_t old_x = *x, old_y = *y;
  do {
    *x = rand() % 20;
    *y = rand() % 11;
  } while (nokia_read_pixel((*x + LEFT_BORDER + 1) * _step, (*y + UP_BORDER + 1) * _step) && old_x == *x && old_y == *y); 
}

inline void draw_apple(uint8_t x, uint8_t y) {
  x *= _step;
  y *= _step;
  x += LEFT_BORDER + 1;
  y += UP_BORDER + 1;
  drawBitmap(x, y, Apple_Sprite_4x4, 4, 4, 1);
}

void start_snake(void){
 
  uint8_t score = 3, x = 5, y = 5, apple_x, apple_y;
  uint8_t Snake[55] = {0};
  
  write_snakeMas(Snake, 0, Left);
  write_snakeMas(Snake, 1, Left);
  
  uint32_t current_time, prev_time1, prev_time2;
  
  
  Direction_typedef direction = Right;
  KEY_STATE current_key = None, prev_key = None;
  
  struct additional_data states;
  states.IsDirectionChanged = 0;
  states.speed = 7; 
  states.needApple = 1;
  
  current_time = prev_time1 = prev_time2 = millis();
  
  ADC_CR1_ADON = 1;
  ADC_CSR_CH = 0x03;
  ADC_CR1_ADON = 1;
  while (!ADC_CSR_EOC);
  uint16_t seed = ADC_DRH << 8;
  seed |= ADC_DRL;
  srand(seed);
  ADC_CR1_ADON = 0;
  
  nokia_clearBuffer();
  
  while(1){
      
      current_time =  millis();
      
      current_key = read_button();
      
      if (current_time - prev_time1 >= update_interval_button) {
        
        if (current_key == None && prev_key != None && !states.IsDirectionChanged) {
          if (prev_key == RIGHT_KEY){
            if (direction == Up) direction = Right;
            else direction++;
          } 
          else if (prev_key == LEFT_KEY){
            if (direction == Right) direction = Up;
            else direction--;
          }
          
          states.IsDirectionChanged = 1;
          
        }
        prev_key = current_key;
      }
      
     if (current_time - prev_time2 >= update_interval - states.speed * 10) { 
        
        prev_time2 = millis();
        states.IsDirectionChanged = 0;
        
        Direction_typedef direction_snake;
        
        switch (direction) {
        case Right: {
          x ++; direction_snake = Left;
        }
            break;
        case Down: {
          y++; direction_snake = Up;
        }
            break;
        case Left: {
          x--; direction_snake = Right;
        }
            break;
        case Up: {
          y--; direction_snake = Down;
        }
            break;
        }
        
        if (checkBorder(x, y) == Collision || checkSnakeCollision(x, y) == Collision) break;
        if (apple_x == x && apple_y == y){
          score++; states.needApple = 1; 
        }
        
        if (states.needApple) {
          generate_apple(&apple_x, &apple_y); states.needApple = 0; 
        }
        
        move_snake(Snake, score, direction_snake);
        
        nokia_clearBuffer();
        drawRect(LEFT_BORDER, UP_BORDER, RIGHT_BORDER,  DOWN_BORDER + 1, 1); //draw play Area
        draw_snake(x, y, Snake, score);
        draw_apple(apple_x, apple_y);
        nokia_update();
      }
  }
  
  nokia_clearBuffer();
  drawScore(40,20, score - 3);
  nokia_update();
  
  current_time = prev_time1 = millis();
  while (current_time - prev_time1 <= 1000) {
    current_time = millis();
  }


}
