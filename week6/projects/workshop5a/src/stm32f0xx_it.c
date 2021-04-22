#include "stm32f0xx_it.h"
#include "morse.h"
#include "usart.h"

// Niet gebruikt, DMA niet kunnen implementeren
volatile uint32_t DMA_EndOfTransfer = 0;

// Vanwege geen DMA USART input heb ik als demonstratie SOS in de buffer gezet
uint8_t buffer[11] = {'s','o','s'};

// Deze interrupt wordt elke 100ms aangeroepen.
// 1 loop door deze interrupt is het begin van een 'time period'
void SysTick_Handler(void)
{
	static char morseVal;
	// houd bij of de speaker piept
	static bool enabled = 0;
	// Afteller om bij te houden hoeveel time
	// periods de piep nog aan moet blijven
	static uint8_t disableIn = 0;
	// Pointer om door een morse string heen te gaan
	static uint8_t morsePointer = 0;
	// Pointer om door de buffer heen te gaan
	static uint8_t bufferPointer = 0;
	
	// Wanneer de piep aanstaat, moeten we kijken of deze nog
	// aan moet blijven. Wanneer ie uit kan, zet hem uit
	if(enabled)
	{
		if(disableIn == 0)
		{
			// Zet het blauwe ledje uit
			STM_EVAL_LEDOff(LED4);
			
			// Zet de speaker uit
			TIM_SetCompare2(TIM3, 0);
			
			enabled = false;
		}
		else disableIn--;
	}
	// Als de piep uitstaat, kunnen we door naar het volgende morse karakter
	else if(!enabled)
	{
		// Check of de buffer niet leeg is. Zo ja, stop
		if(bufferPointer > sizeof(buffer))
		{
			return;
		}
		
		// Pak een char uit de buffer en haal het bijbehorende morse karakter op
		morseVal = morse[buffer[bufferPointer] - 97][morsePointer];
		
		// Zet de piep aan voor 1 time period
		if(morseVal == '.')
		{
			uPutString(".");
			
			// Zet het blauwe ledje aan
			STM_EVAL_LEDOn(LED4);
			
			// Zet de speaker aan
			TIM_SetCompare2(TIM3, 3000);
			
			enabled = true;
			disableIn = 1;
			morsePointer++;
		}
		// Zet de piep aan voor 3 time periods
		else if(morseVal == '-')
		{
			uPutString("-");
			
			// Zet het blauwe ledje aan
			STM_EVAL_LEDOn(LED4);
			
			// Zet de speaker aan
			TIM_SetCompare2(TIM3, 3000);
			
			enabled = true;
			disableIn = 3;
			morsePointer++;
		}
		else
		{
			// Einde van de morse string bereikt, ga door naar het volgende karakter
			bufferPointer++;
			morsePointer = 0;
		}
	}
}
