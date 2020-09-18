#ifndef BUTTONS_H
#define BUTTONS_H

typedef enum KEY_STATE {
  None,
  LEFT_KEY,
  DOWN_KEY,
  RIGHT_KEY,
  UP_KEY
} KEY_STATE;

void init_buttons();
KEY_STATE read_button();

#endif