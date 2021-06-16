// init.c contains initialisation code for gpio, spi, nvic, exti peripherals
// display.c contains functions for controlling the max7219 display
// game.c contains the game logic

#include "init.h"
#include "display.h"
#include "game.h"

int main(void)
{
	// init.c
	gpio();
	spi();
	timer2();
	timer3();
	timer14();
	button();
	adc();
	
	// display.c
	displayInit();
	
	// game.c
	gameInit();
	
	while(1)
	{
		// Stop mode; turn back on when there's an interrupt
		// Go back to sleep when the interrupt is done.
		PWR_EnterSleepMode(PWR_STOPEntry_WFI);
	}
}
