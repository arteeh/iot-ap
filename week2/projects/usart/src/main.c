// Code voor het beheren van de buffer is in twee nieuwe functies, onderaan usart.c
// BUFFER_IS_EMPTY staat in usart.h
// USART Interrupt is te vinden op regel 113 van stm32f0xx_it.c

// Ik behandel de array als een circulaire buffer,
// waarbij ik voorbeeldcode van internet heb gebruikt:
// https://medium.com/@charlesdobson/how-to-implement-a-simple-circular-buffer-in-c-34b7e945d30e

#include "stm32f0xx.h"
#include "usart.h"

extern volatile char rx_buffer;

void delay(const int d);

// Setup PC8 (blue LED) and PC9 (green LED)
void GPIO_init(void)
{
	// GPIOC Periph clock enable
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// PC8 and PC9 in output mode
	GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);

	// Push pull mode selected
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

	// Maximum speed setting (even though it is unnecessary)
	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);

	// Pull-up and pull-down resistors disabled
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}

int main(void)
{
	int c1, c2;

	GPIO_init();
	USART_init();
	USART_clearscreen();

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

		// Get a character (from the buffer)
		c1 = USART_getc();

		// Different data received? And buffer is not empty?
		if(c1 != c2 && c1 != BUFFER_IS_EMPTY)
		{
			// Yes
			c2=c1;
			USART_putstr("You pressed: ");
			USART_putc(c1);
			USART_putstr("\n");
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
