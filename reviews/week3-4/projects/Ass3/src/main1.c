/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

#define ONE_TENTH_SECOND   (SystemCoreClock/8/10)
#define ONE_SECOND         (SystemCoreClock/8)

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
  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED_GREEN);
  STM_EVAL_LEDInit(LED_BLUE);
  
  // Initialize User_Button on STM32F0-Discovery
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
  while(1)
  {
    // Blink LED's
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOff(LED4);

    // Button pressed?
    if(STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)
    {
      delay(ONE_TENTH_SECOND);
    }
    else
    {
      delay(ONE_SECOND);
    }
    
    // Blink LED's
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOn(LED4);

    // Button pressed?
    if(STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)
    {
      delay(ONE_TENTH_SECOND);
    }
    else
    {
      delay(ONE_SECOND);
    }
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

