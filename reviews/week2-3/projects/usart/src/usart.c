/******************************************************************************
 * File           : USART driver implementation file
 *****************************************************************************/
#include "stm32f0xx.h"
#include "usart.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
volatile char rx_buffer[10];
int8_t readIndex = 0;
// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
void USART_BaudrateDetect(void);

void USART_init(void)
{
	
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;																// GPIOA Periph clock enable
  GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);		 	// PA9 and PA10 Alternate function mode
  GPIOA->AFR[1] |= 0x00000110;																			// Set alternate functions AF1 for PA9 and PA10
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 														// USART1 clock enable

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
  while((USART1->ISR & USART_ISR_TXE) == 0) ;

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

char USART_getc(void)
{
	//check if the buffer on the index slot has a value
  if(rx_buffer[readIndex])
		{
		char c = rx_buffer[readIndex];		//clear the last read index
		rx_buffer[readIndex] = 0;
		readIndex++;
			
		return c;
	}
	return 0;
}

void USART_getstr(char *str)
{
	char c;
	//get the char while enter is not pressed
		while(c != '\r')
			{
				c = USART_getc();
				//place the char in the string
				*str++ = c;
		}
	}
	

// Implements the following VT100 terminal commands
// - Clear screan
// - Cursor home
void USART_clearscreen(void)
{
  char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
  char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home
  
  USART_putstr(cmd1);
  USART_putstr(cmd2);
}
