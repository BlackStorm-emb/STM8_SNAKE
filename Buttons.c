#include <Buttons.h>
#include <iostm8.h>

void init_buttons(){
  PD_DDR_bit.DDR5 = 0; //1
  PD_DDR_bit.DDR6 = 0; //2
  PA_DDR_bit.DDR1 = 0; //3
  PA_DDR_bit.DDR2 = 0; //4
  
  PD_CR1_bit.C15 = 1;
  PD_CR1_bit.C16 = 1;
  PA_CR1_bit.C11 = 1;
  PA_CR1_bit.C12 = 1;
  
  PD_CR2_bit.C25 = 0;
  PD_CR2_bit.C26 = 0;
  PA_CR2_bit.C21 = 0;
  PA_CR2_bit.C22 = 0;
}

KEY_STATE read_button(){
  if (!PD_IDR_bit.IDR5)
    return LEFT_KEY;
  else if (!PD_IDR_bit.IDR6)
    return DOWN_KEY;
  else if (!PA_IDR_bit.IDR1)
    return RIGHT_KEY;
  else if (!PA_IDR_bit.IDR2)
    return UP_KEY;
  else return None;
}