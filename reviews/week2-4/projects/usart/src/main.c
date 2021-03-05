/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "main.h"
#include "stm32f0xx.h"
#include "usart.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
	
  init();
  // Setup USART1 (PA9 & PA10)
  USART_init();
  USART_clearscreen();
	USART_putstr("This is assignment 2, get ready to type!\n");
	
  while(1)
  {
		updateSerial();
		delay(DELAY_100MS);
	}
}

// ----------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------
void init(void)
{	
	//Defining global variables and buffer, change bufferLength to increase buffer, if you do this also change bufferEnd to bufferLength - 1
	bufferHead 		= 0;
	bufferBegin 	= 0;
	bufferTail 		= 9;
	bufferEnd 		= 9;
	bufferLength 	= 10;
	offset 				= 1;
	RXBuffer 			= (char *)malloc(sizeof(char) * (bufferLength));	
	
	// --------------------------------------------------------------------------
  // Setup PC8 (blue LED)
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  // PC8 and PC9 in output mode
  GPIOC->MODER |= (GPIO_MODER_MODER8_0) ;
  // Push pull mode selected
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8) ;
  // Maximum speed setting (even though it is unnecessary)
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8);
  // Pull-up and pull-down resistors disabled
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8);
	
	
}

/* 	@Author: Anonymous
 *	@Date: 18-02-2021
 *  @Functionality: This functions is called in the main while loop, here it loops through the character buffer and prints de character from the bufferTail position.
 */
void updateSerial(void)
{
	if(!(bufferHead == bufferTail))
	{
		USART_putc(RXBuffer[bufferTail]);
		bufferTail++;
	}
	if(bufferTail > bufferEnd)
	{
		bufferTail = bufferBegin;
	}
}

#pragma push
#pragma O3
void delay(const int d)
{
  volatile int i;
  for(i=d; i>0; i--){ ; }
  return;
}
#pragma pop

