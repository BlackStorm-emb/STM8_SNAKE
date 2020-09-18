/*
Library to count milliseconds since start up, using Timer4 (8 bit) of STM8S MCUs

By ETPH 20160618, release under 

Parts of code are copyrighted by STMicroelectronics licensed under MCD-ST Liberty SW License Agreement V2
http://www.st.com/software_license_agreement_liberty_v2

This library must be used with STM8S_StdPeriph_Lib package which is provided free of charge by STMicroelectronics

*/

#include "tim4millis.h"


volatile uint32_t current_millis = 0; //--IO: volatile read/write 



void TIM4_init(void)
{
	/* TIM4 configuration:
	- TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
	clock used is 16 MHz / 128 = 125 000 Hz
	- With 125 000 Hz we can generate time base:
	  max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
	  min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
	- In this example we need to generate a time base equal to 1 ms
	so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

	/* Time base configuration */
        TIM4_PSCR = 0x07;
        TIM4_CNTR = TIM4_PERIOD;
	/* Clear TIM4 update flag */
        TIM4_SR_UIF = 0;
	/* Enable update interrupt */
        TIM4_IER_UIE = 1;
        
	/* enable interrupts */
        asm("rim");

	/* Enable TIM4 */
        TIM4_CR1_CEN = 1;
  
}


uint32_t millis(void)
{
	return current_millis;
}


//Interupt event, happen every 1 ms
#pragma vector = TIM4_OVR_UIF_vector 
__interrupt void TIM4_OVF(void)
{   
	//increase 1, for millis() function
	current_millis ++;
	
	TIM4_SR_UIF = 0;
}
