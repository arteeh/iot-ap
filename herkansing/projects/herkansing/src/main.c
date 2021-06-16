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
	//timer3();
	timer14();
	button();
	adc();
	
	// display.c
	displayInit();
	
	// game.c
	gameInit();
	
	while(1)
	{
		// Stop mode, enable when interrupt goes off.
		// Go back to sleep when there's no interrupt.
		//PWR_EnterSleepMode(PWR_STOPEntry_WFI);
	}
}
