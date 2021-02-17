/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stdbool.h"
 
void delay(const int d);
void ledON(uint8_t ledNum);
void ledOff(uint8_t ledNum);
void checkChangeDirection();
volatile bool buttonPressed = false;
volatile bool countdown = false;
//User settings
const uint16_t COUNTERMAX = 9;
const uint8_t LEDS_ON_LENGTH = 1; // real amount = input number + 1 (should not be made higher than COUNTERMAX)

int main(void)
{
	
	//main constants and variables
	const uint16_t DELAY_1_SECOND = (SystemCoreClock/8);
	const uint16_t DELAY_100_MILLIS = (SystemCoreClock/80);
	
	volatile uint16_t counter = 0;
	
	// GPIOA Periph clock enable
  //RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
	
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 

  // PC1 to PC10 in output mode
  GPIOC->MODER |= (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 
	| GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0);

  while(1)
  {
		
		// turn needed leds on
		ledON(counter);
		
		//turn needed leds off
		if(counter < LEDS_ON_LENGTH){
			ledOff(COUNTERMAX - LEDS_ON_LENGTH);
		}else{
			ledOff(counter - LEDS_ON_LENGTH);
		}
		
		//led counting logic
		if(countdown == 0){ //should the counter go up?
			counter++;
			if(counter > COUNTERMAX){
				counter = 0;
			}
		}else{
			if(counter == 0 || counter > COUNTERMAX){ // in case counter ever goes below 0 (uint overflow)
				counter = COUNTERMAX;
			}else{
				counter--;
			}
		}
		checkChangeDirection();
  }
}


void checkChangeDirection(){
	volatile static bool justPressed = false;
	
	if(buttonPressed){
		if(justPressed){
			countdown = true;
		}else{
			justPressed = true;
		}
	}else{
		justPressed = false;
		countdown = false;
	}
}

void ledON(uint8_t ledNum){
	if(ledNum < COUNTERMAX + 1){ 
		ledNum++;
		GPIOC->BSRR |= (1<<ledNum); 
	}
}

void ledOff(uint8_t ledNum){
	if(ledNum < COUNTERMAX + 1){ 
		ledNum++;
		ledNum = ledNum + 16;
		GPIOC->BSRR |= (1<<ledNum);
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


