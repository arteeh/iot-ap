/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "usart.h"
#include <stdlib.h>
#include "include.h"
// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
volatile char *rx_buffer;
volatile int head = 0, tail = 0;
volatile bool isBufferFullFlag = false;
// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void delay(const int d);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  char tempData;
   
  // --------------------------------------------------------------------------
  // Setup PC8 (blue LED) and PC9 (green LED)
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
  // PC8 and PC9 in output mode
  GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0) ;
  // Push pull mode selected
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9) ;
  // Maximum speed setting (even though it is unnecessary)
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
  // Pull-up and pull-down resistors disabled
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

  // --------------------------------------------------------------------------
  // Setup USART1 (PA9 & PA10)
  USART_init();
  USART_clearscreen();
	
	//Welcome message
  USART_putstr("\x1b[32mWelcome to assignment 2! \n\nEverytime you enter a character it will get saved inside '\x1b[36mrx_buffer\x1b[32m'.\n\n");
  USART_putstr("The main loop will do its best to read all the data, once the data is read, the location of the tail will move.\n");
  USART_putstr("This means that you can put in as many caracters as you would like in this console, as long as you do not fill the buffer\n(which is set to \x1b[36m100\x1b[32m by default)\n\n");
	USART_putstr("If you would like to change the buffer size, feel free to do that inside the '\x1b[36minclude.c\x1b[32m' file.\n\n");
	USART_putstr("\x1b[35mHave fun with this tool!\x1b[0m\n\n");
	
	
  
	// Allocate BUFFER_SIZE of chars
	rx_buffer = (char *)malloc(sizeof(char)*BUFFER_SIZE); 
  while(1)
  {
    // Blink the green LED
    // Set PC9
    GPIOC->BSRR = GPIO_BSRR_BS_9;
    // Delay ~ 0.1 sec.
    delay(SystemCoreClock/8/10);
    // Reset PC9
    GPIOC->BSRR = GPIO_BSRR_BR_9;
    // Delay ~ 0.1 sec.
    delay(SystemCoreClock/8/10);
    
		//! We do not have to disable the ISR for this code to work, the rx_buffer functions do not interfere with the function inside of the ISR or with the if statement below
		// Print the character from buffer if the function readFromBuffer returns with NULL
		if((tempData = readFromBuffer()) != NULL){
				USART_putstr("Data read: \x1b[1m");
				USART_putc(tempData);
				USART_putstr("\n\x1b[0m");
		}
  }
}

#pragma push
#pragma O3
//This function was not made by me, this function is copied from the workshop project.
void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
#pragma pop

