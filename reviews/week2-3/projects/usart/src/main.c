/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "usart.h"

#define TENTH_OF_A_SECOND (SystemCoreClock/8/10)


// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void delay(const int d);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  char c1=0, c2=1; 
   
  // --------------------------------------------------------------------------
	
  // Setup PC8 (blue LED) and PC9 (green LED)
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  																	// GPIOC Periph clock enable
  GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0) ;					// PC8 and PC9 in output mode
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9) ;							// Push pull mode selected
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);	// Maximum speed setting (even though it is unnecessary)
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);							// Pull-up and pull-down resistors disabled

  // --------------------------------------------------------------------------
  // Setup USART1 (PA9 & PA10)
  USART_init();
  USART_clearscreen();
  USART_putstr("This is STM32F0 microcontroller lesson 2.\n");
  USART_putstr("Press DIFFERENT characters.\n\n");
  USART_putstr("Why is the green LED not blinking continuously??\n\n");
  
  while(1)
  {
    // Blink the green LED
    GPIOC->BSRR = GPIO_BSRR_BS_9;				// Set PC9
    delay(TENTH_OF_A_SECOND);						// Delay ~ 0.1 sec.
    GPIOC->BSRR = GPIO_BSRR_BR_9;				// Reset PC9
    delay(TENTH_OF_A_SECOND);						// Delay ~ 0.1 sec.
		NVIC_DisableIRQ(USART1_IRQn);				//disable the interrupt temporarily
		
		c1 = USART_getc();
    
    // If different data is received and data is not null
    if(c1 != c2 && c1)
    {
      c2=c1;
      USART_putstr("You pressed: ");
      USART_putc(c1);
      USART_putstr("\n");
    }
		
		//enable the interrupt
		NVIC_EnableIRQ(USART1_IRQn);
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

