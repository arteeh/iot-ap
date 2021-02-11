// Dit is versie B van de eerste opdracht. Succes met checken :)

// Voor specifieke integer types
#include <stdint.h>
// Voor booleans
#include <stdbool.h>

#include "stm32f0xx.h"

#define SECONDE (SystemCoreClock/8)
#define TIENDESECONDE (SystemCoreClock/80 )
 
void delay(const int d);

int main(void)
{
	// Current houdt de registerwaarde bij waar we momenteel zijn. Begin bij led 1.
	uint32_t current = GPIO_BSRR_BS_1;
	uint32_t back;
	uint32_t front;
	// goingForward houdt bij of we vooruit of achteruit gaan.
	bool goingForward = true;
	
	// GPIOA Periph clock aan, voor de blauwe knop
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	// GPIOC Periph clock aan, voor de leds
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	
	// Zet PC1 tot en met PC10 in output mode
	// GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | etc...
	// ...komt uit op 0x155554, dus dat vul ik hier direct in
	GPIOC->MODER |= ((uint32_t)0x155554);
	
	// Push pull mode geselecteerd
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

	// Vol gas
	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);

	// Pull-up en pull-down weerstanden uit
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
	
	while(1)
	{
		// Toggle de goingForward boolean als de blauwe knop is ingedrukt
		if(GPIOA->IDR & GPIO_IDR_0) goingForward = !goingForward;
		
		// We willen ledjes clearen voor en na de twee brandende ledjes.
		// Welke we moeten clearen houden we bij met de front en back variabelen.
		// De clear versie van een set waarde is 16 stappen naar links.
		// Het patroon is altijd (back (clear) , current (set) , current<<1 (set) , front (clear))
		
		// Back is de clear versie van current, gedeeld door twee zodat ie een stap achter current is
		back = (current << 16) / 2;
		// Front is de clear versie van current, keer vier zodat ie twee stappen voor current is
		front = ((current << 16) * 2) * 2;
		
		// Handel speciale gevallen af, zoals van eind naar begin overgaan en andersom
		if(goingForward)
		{
			// Als we weer bij het begin zijn, blijft 10 aanstaan dus zet die hier uit
			if(current == GPIO_BSRR_BS_1) GPIOC->BSRR = GPIO_BSRR_BR_10;
			
			// Aan het eind, zet current naar het begin.
			// 9 blijft zo aanstaan, dus die zetten we hier ook uit
			else if(current == GPIO_BSRR_BS_10)
			{
				current = GPIO_BSRR_BS_0;
				GPIOC->BSRR = GPIO_BSRR_BR_9;
			}
		}
		else
		{
			// Als we achteruit gaan en we bij het begin komen, zet current naar het eind
			if(current == GPIO_BSRR_BS_0) current = GPIO_BSRR_BS_10;
			
			// Als we vorige ronde aan het eind zijn geplaatst,
			// blijft led 1 aanstaan dus die zetten we hier uit.
			else if(current == GPIO_BSRR_BS_9) GPIOC->BSRR = GPIO_BSRR_BR_1;
		}

		// Set en clear alles volgens onze variabelen
		GPIOC->BSRR = back;
		GPIOC->BSRR = front;
		GPIOC->BSRR = current;
		GPIOC->BSRR = current << 1;
		
		// Update current naar links of rechts, afhankelijk van de richting
		if(goingForward) current = current << 1;
		else current = current >> 1;
		
		delay(TIENDESECONDE);
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


