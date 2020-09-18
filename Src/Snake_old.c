#include <Snake.h>
#include <iostm8s103f3.h>
#include <nokia.h>
#include <nokia_gfx.h>
#include <nokia_fastGFX.h>
#include <Buttons.h>
#include <tim4millis.h>
#include <stdlib.h>

#define _step 4

#define LEFT_BORDER 1
#define RIGHT_BORDER 81
#define UP_BORDER 0
#define DOWN_BORDER 45

#define APPLE_AMOUNT 5

const uint8_t max_score = 90;
const uint32_t update_interval = 200;

const uint8_t Score_Text_Sprite_24x8[]  = {
	0x39, 0x17, 0xDF, 
	0x45, 0x14, 0x04, 
	0x40, 0x94, 0x04, 
	0x40, 0x77, 0xC4, 
	0x40, 0x17, 0xC4, 
	0x40, 0x14, 0x04, 
	0x44, 0x14, 0x04, 
	0x38, 0x17, 0xC4, 
};

const uint8_t Apple_Sprite_4x4[] = {
	0x60, 
	0x90, 
	0x90, 
	0x60, 
};

const uint8_t Snake_Sprite_4x4[]  = {
	0xF0, 
	0xF0, 
	0xF0, 
	0xF0, 
};

uint16_t apples[APPLE_AMOUNT] = {
  762, 1110, 1798, 450, 2794
};

typedef enum Direction_enum {
  Right = 1,
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
  uint8_t speed : 4;
  uint8_t none : 3;
};

inline void drawSnake(uint16_t snake[], const uint8_t* score){
  for (int i = 0; i < *score; i++) {
    uint8_t x = snake[i] % 84;
    uint8_t y = snake[i] / 84;
    //drawRect(x, y, _step, _step, 1);
    drawBitmap(x, y, Snake_Sprite_4x4, 4, 4, 1); 
  }
}

inline void moveSnake(uint16_t snake[], const uint8_t* score, const uint8_t* new_x, const uint8_t* new_y){
  for (uint8_t i = *score - 1; i > 0; i--)
    snake[i] = snake[i-1];
  snake[0] = *new_y * 84 + *new_x;
}

inline Events_typedef checkBorder(const uint8_t* x, const uint8_t* y) {
  if (*x >= RIGHT_BORDER || *x <= LEFT_BORDER || *y >= DOWN_BORDER || *y <= UP_BORDER)
    return Collision;
  return No;
}

inline Events_typedef checkSnakeCollision(uint16_t snake[], const uint8_t* score, const uint8_t* x, const uint8_t* y){
  uint16_t number = *y*84 + *x;
  
  for (uint8_t i = 0; i < *score; i++) 
    if (number == snake[i]) return Collision;
  return No;
}

inline uint16_t generateApple(uint16_t snake[], const uint8_t* score) {
  uint8_t apple_number;
  uint8_t isCollised = 0;
  uint16_t apple;
  
  ADC_CR1_ADON = 1;
  while (!ADC_CSR_EOC);
  apple_number = ADC_DRH % APPLE_AMOUNT;
  
  do {
    apple = apples[apple_number];
    isCollised = 0;
    for (uint8_t i = 0; i < *score; i++)
      if (apple == snake[i]){
        isCollised = 1;
        apple_number++;
        if (apple_number == APPLE_AMOUNT) apple_number = 0;
      }
  } while(isCollised);
  

  return apple;
}

inline void drawApple(const uint16_t* number) {
  uint8_t x = *number % 84;
  uint8_t y = *number / 84;
  //drawRect(x, y, _step, _step, 1);
  drawBitmap(x, y, Apple_Sprite_4x4, 4, 4, 1);
}

void start_snake(void){
 
  uint8_t score = 1;
  //uint8_t isGame = 1;
  uint8_t x = 14, y = 5;
  uint8_t add_time = 0;
  uint16_t* snake = (uint16_t*)malloc(max_score * sizeof(uint16_t));
  uint16_t apple;
  uint32_t current_time, prev_time;
  
  
  Direction_typedef direction = Right;
  KEY_STATE current_key = None, prev_key = None;
  struct additional_data states;
  
  states.IsDirectionChanged = 0;
  
  snake[0] = y * 84 + x;

  states.speed = 11; 

  
  drawApple(&apple);
  
  current_time = prev_time = millis();
  
  ADC_CR1_ADON = 1;
  ADC_CSR_CH = 0x03;
  
  apple = generateApple(snake, &score);
  
  nokia_clear();
  while(1){
      nokia_clearBuffer();
      drawRect(1, 0, 82, 46, 1); //draw play Area
      
      current_time =  millis();
      
      current_key = read_button();
      
      
      if (current_time - prev_time - add_time >= update_interval / 40) {
        add_time += update_interval / 40;
        if (current_key != None && prev_key == None && !states.IsDirectionChanged) {
          if (current_key == RIGHT_KEY)
            direction++;
          else if (current_key == LEFT_KEY)
            direction--;
          
          states.IsDirectionChanged = 1;
          
          if (direction == Up + 1) direction = Right;
          else if (direction == Right - 1) direction = Up;
          
        }
        prev_key = current_key;
      }
      
      if (current_time - prev_time >= update_interval - states.speed * 10) { 
        add_time = 0;
        prev_time = millis();
        
        switch (direction) {
          case Right: x += _step;
            break;
          case Down: y += _step;
            break;
          case Left: x -= _step;
            break;
          case Up: y -= _step;
            break;
        }
 
        if (checkBorder(&x, &y) == Collision) break;
        else if (checkSnakeCollision(snake, &score, &x, &y) == Collision) break;
        
        if (y*84 + x == apple) {
          score++;
          if (score == max_score) break;
          moveSnake(snake, &score, &x, &y);
          apple = generateApple(snake, &score);
          //prev_time -= update_interval - states.speed * 10;
        }
        else moveSnake(snake, &score, &x, &y);
          
        states.IsDirectionChanged = 0;
      }
      
      
      
      drawSnake(snake, &score);
      drawApple(&apple);
      nokia_update();
  }
  free(snake);
  snake = NULL;
  
  ADC_CR1_ADON = 0;
}
