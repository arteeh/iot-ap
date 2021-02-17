/******************************************************************************
 * File           : Main program
 *****************************************************************************/
 /*versie 3 
x
xx
xxx
xx
x
*/
#include "stm32f0xx.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
void delay(const int d);
void ledAan( volatile int led);
void ledUit( volatile int ledniet);
#define DELAY_1_SEC (SystemCoreClock/8)
void delay(const int d);

int main(void)
{
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 

  // PC10 tm PC1 in output mode
  GPIOC->MODER |= (GPIO_MODER_MODER10_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER3_0| GPIO_MODER_MODER2_0| GPIO_MODER_MODER1_0);

  // Push pull mode selected
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_10 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_7 |GPIO_OTYPER_OT_6 |GPIO_OTYPER_OT_5 |GPIO_OTYPER_OT_4 |GPIO_OTYPER_OT_3 |GPIO_OTYPER_OT_2 |GPIO_OTYPER_OT_1);

  // Maximum speed setting
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR9 |GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR1);

  // Pull-up and pull-down resistors disabled
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR9 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR3 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR1);
	
	// GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
  while(1)
  {
		bool start = true;
		int startpunt = 11;
		if(start){
			ledAan(startpunt);
			
		}
	}
}

// compile the delay function with optimization level 3
// When compiled with maximum optimization level (-O3), this delay function
// produces the following assembly instructions (start the debugger to see).
// When we know the number of cycles for each instruction, the timing of one
// delay loop is easy to calculate.
//
// 8 cycles per loop => 8 * (1/SystemCoreClock) sec.
//
// Ref: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0432c/CHDCICDF.html
//
//                                              Cycles required
// 0x080000E2 9000      STR      r0,[sp,#0x00]  2
// 0x080000E4 2800      CMP      r0,#0x00       1
// 0x080000E6 DD01      BLE      0x080000EC     1
// 0x080000E8 1E40      SUBS     r0,r0,#1       1
// 0x080000EA E7FA      B        0x080000E2     3

#pragma push
#pragma O3

void ledAan(volatile int led){

		bool state, prevstate;
		//Led bar aanzetten van 10 naar 0
		while(led > 0&& state){
			if(GPIOA -> IDR & GPIO_IDR_0){
				state =! prevstate;
				state = true;
				ledUit(0);
				delay(SystemCoreClock/40);
			}
				GPIOC->BSRR = (1<<led);
				led--;
				delay(SystemCoreClock/40);
		}
		ledUit(0);
		
}	

void ledUit( volatile int ledniet){
		bool stateUit, prevstateUit;	
		//ledbar uitzettten.
		while(ledniet < 12&& stateUit){
			if(GPIOA -> IDR & GPIO_IDR_0){
				stateUit =! prevstateUit;
				stateUit = true;
				ledAan(11);
				delay(SystemCoreClock/40);
			}
				GPIOC->BSRR = (~0 << ledniet);
				ledniet++;
				delay(SystemCoreClock/40);
		}
		ledAan(11);

}
void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }
  return;
}
#pragma pop

