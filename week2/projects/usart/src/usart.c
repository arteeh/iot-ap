#include "stm32f0xx.h"
#include "usart.h"

#define BUFFER_SIZE (100)

// Pointer to a char array of size BUFFER_SIZE
volatile char * rx_buffer;

// Global variable to keep track of the amount of characters in the buffer
uint8_t bufferLength = 0;

void USART_BaudrateDetect(void);

void USART_init(void)
{
	int i;

	// Allocate the buffer. No need to free()
	rx_buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE);

	// Clear the buffer values
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		rx_buffer[i] = 0;
	}

	// GPIOA Periph clock enable
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// PA9 and PA10 Alternate function mode
	GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);

	// Set alternate functions AF1 for PA9 and PA10
	GPIOA->AFR[1] |= 0x00000110;

	// USART1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	// 115200 Bd @ 48 MHz
	// USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
	// BRR[15:4] = USARTDIV[15:4]
	// When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
	USART1->BRR = 0x01A0;

	// USART enable
	// Receiver enable
	// Transmitter enable
	USART1->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;

	// Default value
	USART1->CR2 = 0;
	USART1->CR3 = 0;

	// RXNE interrupt enable
	USART1->CR1 |= USART_CR1_RXNEIE;

	// USART1 interrupts enable in NVIC
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_ClearPendingIRQ(USART1_IRQn);
}

void USART_putc(char c)
{
	// Wait for Transmit data register empty
	while((USART1->ISR & USART_ISR_TXE) == 0) { }

	// Transmit data by writing to TDR, clears TXE flag
	USART1->TDR = c;
}

void USART_putstr(char *str)
{
	while(*str)
	{
		if(*str == '\n')
		{
			USART_putc('\r');
		}

		USART_putc(*str++);
	}
}

// Getc now simply uses the dequeue function to get a character from the buffer
char USART_getc(void)
{
	return(USART_dequeue());
}

void USART_getstr(char *str)
{
	// Implement this function yourself

	// No, I think I won't
}

void USART_clearscreen(void)
{
	char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
	char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home

	USART_putstr(cmd1);
	USART_putstr(cmd2);
}

// Add a char to the rx_buffer
// Code used as an example:
// https://medium.com/@charlesdobson/how-to-implement-a-simple-circular-buffer-in-c-34b7e945d30e
void USART_enqueue(char input)
{
	// Where in the array should be written to. This is always at the end of the list.
	static uint8_t writeIndex;
	
	if (bufferLength == BUFFER_SIZE)
	{
		// Buffer full! Set PC8
		GPIOC->BSRR = GPIO_BSRR_BS_8;
	}
	else
	{
		// Add to the buffer
		rx_buffer[writeIndex] = input;

		// The buffer just got one char longer
		bufferLength++;

		// Move the write variable to the next spot
		writeIndex++;

		// If overflow, start writing at the beginning of the "circle"
		if (writeIndex == BUFFER_SIZE)
		{
			writeIndex = 0;
		}
	}
}

// Dequeue a char from rx_buffer and return it
// Code used as an example:
// https://medium.com/@charlesdobson/how-to-implement-a-simple-circular-buffer-in-c-34b7e945d30e
char USART_dequeue(void)
{
	// Where in the array should be read. This is always at the beginning of the list.
	static uint8_t readIndex;

	char retVal;

	if (bufferLength == 0)
	{
		// Buffer empty! Let main() handle it.
		retVal = BUFFER_IS_EMPTY;
	}
	else
	{
		// Get the char at the front of the buffer
		retVal = rx_buffer[readIndex];

		// The buffer is now one char less
		bufferLength--;

		// Move the read variable to the next char in line
		readIndex++;

		// If overflow, start reading at the beginning of the "circle"
		if (readIndex == BUFFER_SIZE)
		{
			readIndex = 0;
		}
	}

	return retVal;
}
