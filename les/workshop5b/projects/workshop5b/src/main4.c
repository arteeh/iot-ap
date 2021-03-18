/******************************************************************************
 * File           : Main program - I2C 2x16 LCD BTHQ21605V
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "se24lc512.h"    //serial eeprom
#include "helper.h"
#include "string.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
extern uint32_t SE_24LC512_CommStatus;

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  uint16_t startaddress = 0x1234;   //arbitrary address
  uint16_t addr = startaddress;
  uint8_t  data;
  uint8_t  i;
	char     letter;
  
  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  // Initialize Usaddrer Button on STM32F0-Discovery
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

  I2C_Setup();
	
  while(1)
  {
	  char* writestring = "Hello";
	  
	  for (i=0; i<strlen(writestring); i++)
	  {
			letter = writestring[i];
  	  SE24LC512_WriteData(addr, letter);
	    Delay(SystemCoreClock/8/200);    //after write byte wait 5 ms!!

			data = SE24LC512_ReadData( addr );
		
      // In case of I2C error, blink LED3 very fast
      if ( SE_24LC512_CommStatus != SE_24LC512_COMM_OK )
      {
        while(1)
        {
          STM_EVAL_LEDToggle(LED3);
          Delay(SystemCoreClock/8/20);
        }
      }
		
			// In case of data verification error (read gives different value than write)	
			if ( data != letter )
			{
        while(1)
        {
          STM_EVAL_LEDToggle(LED3);
          Delay(SystemCoreClock/8/5);   //blink LED3 slowly
        }
			}
			addr++;
	  }
	  	  
	  startaddress += 128;  //next page
		
		//toggle LED very slowly to show that all is right
		STM_EVAL_LEDToggle(LED3);
    Delay(SystemCoreClock/8);
  }
}
