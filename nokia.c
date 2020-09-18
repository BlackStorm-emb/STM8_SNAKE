#include "nokia.h"
#include <iostm8s103f3.h>
#include <intrinsics.h>
#include <stdlib.h>

#define VOP 76

#define RST PD_ODR_bit.ODR3
#define CE PC_ODR_bit.ODR4
#define DC PC_ODR_bit.ODR3
#define DIN PC_ODR_bit.ODR6
#define CLK PC_ODR_bit.ODR5

#define _BV(bit) (1 << (bit))

#define _rotation 2

uint8_t buffer[504] = {0};

inline void init_gpio(){
   PD_DDR_bit.DDR3 = 1;
   PC_DDR_bit.DDR4 = 1;
   PC_DDR_bit.DDR3 = 1;
   PC_DDR_bit.DDR6 = 1;   
   PC_DDR_bit.DDR5 = 1;
   
   PD_CR1_bit.C13 = 1;
   PC_CR1_bit.C14 = 1;
   PC_CR1_bit.C13 = 1;
   PC_CR1_bit.C16 = 1;
   PC_CR1_bit.C15 = 1;
   
   PD_CR2_bit.C23 = 1;
   PC_CR2_bit.C24 = 1;
   PC_CR2_bit.C23 = 1;
   PC_CR2_bit.C26 = 1;
   PC_CR2_bit.C25 = 1;
}

inline void spi_init(){
  //SYSCFG_RMPCR1 |= 1<<7;
  CLK_PCKENR1 |= 1<<1;
  
  SPI_CR1_bit.LSBFIRST = 0;
  SPI_CR1_SPE = 0;
  SPI_CR1_bit.BR = 1;
  SPI_CR1_bit.MSTR = 1;
  SPI_CR2_bit.BDM = 1;
  SPI_CR2_bit.BDOE = 1;
  
  SPI_CR1_bit.CPOL = 0;
  SPI_CR1_bit.SPE = 1;
  //
}

inline void pcd8544_sendbyte(uint8_t data) {
    CE = 0;
    
    for (uint8_t c = 0; c < 8; c++) {
      CLK = 0;
        if (data & 0x80)
          DIN = 1;
        else 
          DIN = 0;
      data <<= 1;
      CLK = 1;
    }
  
    CE = 1;
}

/*
inline void pcd8544_sendbyte(uint8_t data) {
    CE = 0;
    
    while (!(SPI_SR_bit.TXE));
    
    SPI_DR = data;

    CE = 1;
}
*/

inline void send_data(uint8_t data) {
  DC = 1;
  pcd8544_sendbyte(data);
}

inline void send_cmd(uint8_t cmd) {
  DC = 0;
  pcd8544_sendbyte(cmd);
}

inline void nokia_setpos(uint8_t page, uint8_t x) {
  send_cmd(0x40 | (page & 7) );
  send_cmd(0x80 | x);
}

void nokia_clear(){
  nokia_setpos(0,0);
  
  for(uint16_t i = 0; i < 504; i++)
      send_data(0x00); 
  
  nokia_setpos(0,0);
}

void nokia_clearBuffer(){
  for(uint16_t i = 0; i < 504; i++)
    buffer[i] = 0x00;
}

void nokia_clearBufArea(uint8_t start, uint8_t end){
  for(uint16_t i = start; i < end; i++)
    buffer[i] = 0;
}

void nokia_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
  if ( (x >= _width) || (y >= _height) )
    return;
  
  int16_t t;
  switch (_rotation) {
  case 1:
    t = x;
    x = y;
    y = _height - 1 - t;
    break;
  case 2:
    x = _width - 1 - x;
    y = _height - 1 - y;
    break;
  case 3:
    t = x;
    x = _width - 1 - y;
    y = t;
    break;
  }
  
  if (color)
    buffer[x + (y / 8) * _width] |= _BV(y % 8);
  else
    buffer[x + (y / 8) * _width] &= ~_BV(y % 8);
}

uint8_t nokia_read_pixel(uint8_t x, uint8_t y) {
  if ( (x >= _width) || (y >= _height) )
    return 0;
  
  int16_t t;
  switch (_rotation) {
  case 1:
    t = x;
    x = y;
    y = _height - 1 - t;
    break;
  case 2:
    x = _width - 1 - x;
    y = _height - 1 - y;
    break;
  case 3:
    t = x;
    x = _width - 1 - y;
    y = t;
    break;
  }
  
  return buffer[x + (y / 8) * _width] & _BV(y % 8) ? 1 : 0;
}

void nokia_update() {
  //nokia_setpos(0, 0);
  for (uint16_t i = 0; i < 504; i++){
    send_data(buffer[i]);
  }
}

void nokia_init(){
   init_gpio(); 
   //spi_init();
   
   //RST disp
   
   RST = 0;
   RST = 1;
   
   //first init
   
   send_cmd(0x21);//advance number of cmd
   send_cmd(0x80 + VOP);//voltage offset
   
   send_cmd(0x04);//temp correction "0"
   send_cmd(0x20);//1:48
   send_cmd(0x20);//standart
   send_cmd(0x0C);//normal mode   
}

