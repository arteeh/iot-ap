/******************************************************************************
 * File           : Main program
 * Studentnummer  : 1125465
 *****************************************************************************/
#include "stm32f0xx.h"
#include <stdbool.h>
 
#define DELAY_100MILLISECONDS delay(SystemCoreClock/80)
#define DELAY_10MILLISECONDS delay(SystemCoreClock/800)
#define NUMBEROFLEDS 10
#define FIRSTLED 0
#define LASTLED 9

//Global variables

// init functions
void init_Clock(void);
void init_Mode(void);
void init_Type(void);
void init_Speed(void);
void init_Pupd(void);
void init(void);

//behavioural functions
void delay(const int d);
void checkButton(bool *p_prevState, bool *p_changeLeft);
void checkDirection(bool *p_changeLeft, int *p_ledPosition);
void checkBorders(bool *p_changeLeft, int *p_ledPosition);
int moveToRight(int position);
int moveToLeft(int position);

int main(void)
{
	//variable that holds the direction of the led bar -- false is move right -- true is move left --
	static bool changeLeft = false;
	//variable that holds the previous state of the button -- false button not pressed yet -- true button was pressed --
	static bool prev_state = false;
	//variable that holds the current ledposition
	static int ledPosition = FIRSTLED;
	
	//initialize the microcontroller
	init();
	
 /*
	* While loop to check:
	*  1. If button is pressed
	*  2. If we have to change direction, depending on the borders or button.
	*/
	
  while(1)
  {	
		checkButton(&prev_state, &changeLeft);
		
		checkDirection(&changeLeft, &ledPosition);
		
		DELAY_100MILLISECONDS;		
  }
}

void checkDirection(bool *p_changeLeft, int *p_ledPosition) {
	// if led is moving to the left change position to the left
		if(*p_changeLeft) {
			*p_ledPosition = moveToLeft(*p_ledPosition);
		}else { // if moving to the right change position to the right
			*p_ledPosition = moveToRight(*p_ledPosition);
		}	
		
		if(*p_ledPosition <= FIRSTLED) { //if cleared all the leds, than change direction to right
			*p_changeLeft = false;
			*p_ledPosition = FIRSTLED; // set the position to the first led to discourage bugs
		}else if(*p_ledPosition > LASTLED) { // if filled all the leds, than change direction to left
			*p_changeLeft = true; 
			*p_ledPosition = LASTLED; //set the position to the last led to discourage bugs
		}	
}

void checkButton(bool *p_prevState, bool *p_changeLeft) {
	if(GPIOA->IDR & GPIO_IDR_0) { //if pressed
		DELAY_10MILLISECONDS; //wait
		if((GPIOA->IDR & GPIO_IDR_0) && !*p_prevState) { 
			*p_prevState = true; //previous state of the button is true
			*p_changeLeft = !*p_changeLeft; // change the direction
		}
	}else {//else previous state is false
		*p_prevState = false; 
	}
}

int moveToLeft(int position) {
	//array that turns the corresponding led off
	const uint32_t turnLedOff[NUMBEROFLEDS] = { GPIO_BSRR_BR_1,
																							GPIO_BSRR_BR_2,
																							GPIO_BSRR_BR_3,
																							GPIO_BSRR_BR_4,
																							GPIO_BSRR_BR_5,
																							GPIO_BSRR_BR_6,
																							GPIO_BSRR_BR_7,
																							GPIO_BSRR_BR_8,
																							GPIO_BSRR_BR_9,
																							GPIO_BSRR_BR_10 };
	GPIOC->BSRR = turnLedOff[position];
	position--; //decrease position for next led
	return position;
}	

int moveToRight(int position) {
	//array that turns the corresponding led on
	const uint32_t turnLedOn[NUMBEROFLEDS] = { GPIO_BSRR_BS_1,
																						 GPIO_BSRR_BS_2,
																						 GPIO_BSRR_BS_3,
																						 GPIO_BSRR_BS_4,
																						 GPIO_BSRR_BS_5,
																						 GPIO_BSRR_BS_6,
																						 GPIO_BSRR_BS_7,
																						 GPIO_BSRR_BS_8,
																						 GPIO_BSRR_BS_9,
																						 GPIO_BSRR_BS_10 };		
	GPIOC->BSRR = turnLedOn[position];
	position++; //increase position for next led
	return position;
}

void init_Clock() {
	//enable bus for gpioa and gpioc
	RCC->AHBENR |= (RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN);
}

void init_Mode() {
	//set pc1 ... pc10 as output
	GPIOC->MODER |= ( GPIO_MODER_MODER1_0 |
									  GPIO_MODER_MODER2_0 |
									  GPIO_MODER_MODER3_0 |
									  GPIO_MODER_MODER4_0 |
									  GPIO_MODER_MODER5_0 |
									  GPIO_MODER_MODER6_0 |
									  GPIO_MODER_MODER7_0 |
									  GPIO_MODER_MODER8_0 |
									  GPIO_MODER_MODER9_0 |
									  GPIO_MODER_MODER10_0 );
	//set pin pa0 as input
	GPIOA->MODER &= ~(GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1);
	
}

void init_Type() {
	//push pull mode selected
	GPIOC->OTYPER &= ~( GPIO_OTYPER_OT_1 |
											GPIO_OTYPER_OT_2 |
											GPIO_OTYPER_OT_3 |
											GPIO_OTYPER_OT_4 |
											GPIO_OTYPER_OT_5 |
											GPIO_OTYPER_OT_6 |
											GPIO_OTYPER_OT_7 |
											GPIO_OTYPER_OT_8 |
											GPIO_OTYPER_OT_9 |
											GPIO_OTYPER_OT_10 );
}	

void init_Speed() {
	//maximum speed setting
	GPIOC->OSPEEDR |= ( GPIO_OSPEEDER_OSPEEDR1 |
											GPIO_OSPEEDER_OSPEEDR2 |
											GPIO_OSPEEDER_OSPEEDR3 |
											GPIO_OSPEEDER_OSPEEDR4 |
											GPIO_OSPEEDER_OSPEEDR5 |
											GPIO_OSPEEDER_OSPEEDR6 |
											GPIO_OSPEEDER_OSPEEDR7 |
											GPIO_OSPEEDER_OSPEEDR8 |
											GPIO_OSPEEDER_OSPEEDR9 |
											GPIO_OSPEEDER_OSPEEDR10 );
}

void init_Pupd() {
	//disable pull up/pull down resistor
	GPIOC->PUPDR &= ~( 	GPIO_PUPDR_PUPDR1 |
											GPIO_PUPDR_PUPDR2 | 
											GPIO_PUPDR_PUPDR3 |
											GPIO_PUPDR_PUPDR4 |
											GPIO_PUPDR_PUPDR5 |
											GPIO_PUPDR_PUPDR6 |
											GPIO_PUPDR_PUPDR7 |
											GPIO_PUPDR_PUPDR8 |
											GPIO_PUPDR_PUPDR9 |
											GPIO_PUPDR_PUPDR10 );
}

void init() { 
	init_Clock();
	init_Mode();
	init_Type();
	init_Speed();
	init_Pupd();
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


