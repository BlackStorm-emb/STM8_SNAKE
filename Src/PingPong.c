#include <PingPong.h>
#include <iostm8s103f3.h>
#include <nokia.h>
#include <nokia_gfx.h>
#include <nokia_fastGFX.h>
#include <Buttons.h>
#include <tim4millis.h>
#include <stdlib.h>

#define PLAY_AREA_OFFSET_X 0
#define PLAY_AREA_OFFSET_Y 13
#define PLAY_AREA_W 84
#define PLAY_AREA_H 48 - PLAY_AREA_OFFSET_Y
#define PLAY_AREA_BOTTOM 47

#define PLATFORM_PLAYER_X 2
#define PLATFORM_BOT_X 81
#define PLATFORM_LENGTH 6

#define BALL_DEFAULT_X 24
#define BALL_DEFAULT_Y 25

#define move_interval 100

const uint8_t Ball_Sprite_3x3[]  = {
	0xF0, 
	0xF0, 
	0xF0 
};

typedef enum Direction_enum {
  RIGHT_UP,
  RIGHT_DOWN,
  LEFT_DOWN,
  LEFT_UP
} Direction_typedef;

struct additional_data {
  uint8_t speed : 5;
  uint8_t  BotMove : 1;
  uint8_t None : 2;
};

inline void drawArea(){
  drawRect(PLAY_AREA_OFFSET_X, PLAY_AREA_OFFSET_Y, PLAY_AREA_W , PLAY_AREA_H, 1); 
  
  for (uint8_t i = 1; i <= PLAY_AREA_H; i+=6) {
    drawFastVLine(PLAY_AREA_W/2 - 1, i + PLAY_AREA_OFFSET_Y, 3, 1);
    drawFastVLine(PLAY_AREA_W/2, i + PLAY_AREA_OFFSET_Y, 3, 1);
  }

}

inline void drawBall(const uint8_t* x, const uint8_t* y) {
  drawBitmap(*x, *y, Ball_Sprite_3x3, 3, 3, 1);
}

inline void drawPlatform(const uint8_t x, const uint8_t* y) {
  drawFastVLine(x, *y, PLATFORM_LENGTH, 1);
}

void start_pingPong() {
  uint32_t current_time, prev_time, prev_time_button;
  uint8_t x_Ball = BALL_DEFAULT_X, y_Ball = BALL_DEFAULT_Y;
  uint8_t y_Player = PLAY_AREA_OFFSET_Y + 17, y_Bot = PLAY_AREA_OFFSET_Y + 19;
  uint8_t scorePlayer = 0, scoreBot = 0;
    
  struct additional_data states;
  Direction_typedef direction = LEFT_UP;
  KEY_STATE current_key = None;
  
  states.speed = 13;
  
  ADC_CR1_ADON = 1;
  ADC_CSR_CH = 0x03;
  
  current_time = prev_time = prev_time_button = millis();
  
  while(1) {
    nokia_clearBuffer();
    
    drawArea();
    
    drawScore(12,0, scorePlayer);
    drawScore(63,0, scoreBot);
    
    current_time = millis();
    
    current_key = read_button();
    
    if (current_time - prev_time_button  >= move_interval / 5) {
       prev_time_button = millis();
          if (current_key == UP_KEY && y_Player > PLAY_AREA_OFFSET_Y + 1)
             y_Player--;
          else if (current_key == DOWN_KEY && y_Player + PLATFORM_LENGTH < PLAY_AREA_BOTTOM)
             y_Player++;
      }
    
    if (current_time - prev_time >= move_interval - states.speed * 5) {
      prev_time = millis();
      switch (direction) {
          case LEFT_UP: {
            x_Ball--; y_Ball--;
          }
            break;
          case LEFT_DOWN: {
            x_Ball--; y_Ball++;
          }
            break;
          case RIGHT_UP: {
            x_Ball++; y_Ball--;
          }
            break;
          case RIGHT_DOWN: {
            x_Ball++; y_Ball++;
          }
            break;
        }
      
      if (y_Ball == PLAY_AREA_OFFSET_Y + 1) {
        if (direction == LEFT_UP) direction  = LEFT_DOWN;
        else direction = RIGHT_DOWN;
      }
      else if (y_Ball == PLAY_AREA_BOTTOM - 3){
        if (direction == LEFT_DOWN) direction  = LEFT_UP;
        else direction = RIGHT_UP;
      } 
      
      if (x_Ball == PLATFORM_PLAYER_X + 1){
        if (y_Ball + 2 >= y_Player  && y_Ball <= y_Player + PLATFORM_LENGTH) {
          if (direction == LEFT_DOWN) direction = RIGHT_DOWN;
          else direction = RIGHT_UP;
        }
        else {
          x_Ball = BALL_DEFAULT_X;
          y_Ball = BALL_DEFAULT_Y;
          direction = RIGHT_UP;
          scoreBot++;
        }
      }
      else if (x_Ball == PLATFORM_BOT_X - 3){
        if (y_Ball + 2 >= y_Bot && y_Ball <= y_Bot + PLATFORM_LENGTH) {
          if (direction == RIGHT_DOWN) direction = LEFT_DOWN;
          else direction = LEFT_UP;
        }
        else {
          x_Ball = BALL_DEFAULT_X;
          y_Ball = BALL_DEFAULT_Y;
          direction = LEFT_UP;
          scorePlayer++;
        }
      }
      
  
      ADC_CR1_ADON = 1;
      while (!ADC_CSR_EOC);
      states.BotMove = ADC_DRH % 2;
      
      if (states.BotMove) {
        if ( (direction == LEFT_UP || direction == RIGHT_UP) && y_Bot > PLAY_AREA_OFFSET_Y + 1) y_Bot--;
        else if ( (direction == LEFT_DOWN || direction == RIGHT_DOWN) && y_Bot + PLATFORM_LENGTH < PLAY_AREA_BOTTOM) y_Bot++;  
      }
    }
    
    drawBall(&x_Ball, &y_Ball);
    
    drawPlatform(PLATFORM_PLAYER_X, &y_Player);
    drawPlatform(PLATFORM_BOT_X, &y_Bot);
    nokia_update();
  }
  ADC_CR1_ADON = 0;
}