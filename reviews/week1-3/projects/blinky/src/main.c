/******************************************************************************
 * File           : Main program
 * Version				: D led bar fills and empties in same direction
 * Notice					: Please press black RESET BUTTON after loading program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stdbool.h"

#define DELAY_ONE_TENTH_SEC (SystemCoreClock/80)

#define MAX_BARS 10
#define OVERFLOW 20

void delay(const int d);

int main(void)
{
	// Changes in groups of 5 too prevent long lines
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 

  // PC1 to PC10 in output mode.
	GPIOC->MODER |= (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0);
	GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0);

  // Push pull mode selected
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_1 | GPIO_OTYPER_OT_2 | GPIO_OTYPER_OT_3 | GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5);
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10);

  // Maximum speed setting
	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5);
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10);
	

  // Pull-up and pull-down resistors disabled
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3 | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9 | GPIO_PUPDR_PUPDR10);
	
	// GPIOA Periph clock enable for USER BUTTON
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

  while(1)
  {
			static uint8_t i = 0;				// Makes sure always 1 LED stays on
			static bool dir = false;		// Direction
			static uint8_t bar = 0;			// LED bar to be toggled
				
				// Check USER BUTTON
				if(GPIOA->IDR & GPIO_IDR_0){
					dir = !dir;							// Change direction
					if(bar != 0){						
					bar = MAX_BARS - bar;
					}
					i = OVERFLOW - i;
				}
				
				if(dir){
					GPIOC->ODR ^= (GPIO_ODR_1 << bar);
					bar++;
					i++;
					if(i == OVERFLOW){			// When the end of a full cycle (.., 0XXX, 00XX, 0000X) is reached, it skips 0000 and instead goes to X000
						i = 1;
						bar = 1;
						GPIOC->ODR ^= GPIO_ODR_1;
					}
				}
				else {
					GPIOC->ODR ^= (GPIO_ODR_10 >> bar);
					bar++;
					i++;
					if(i == OVERFLOW){			// When the end of a full cycle (.., XXX0, XX00, X000) is reached, it skips 0000 and instead goes to 000X
						i = 1;
						bar = 1;
						GPIOC->ODR ^= GPIO_ODR_10;
					}
				}
				// Reset bar to 0
				if(bar == MAX_BARS){
						bar = 0;
					}
				delay(DELAY_ONE_TENTH_SEC);
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
void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
#pragma pop


