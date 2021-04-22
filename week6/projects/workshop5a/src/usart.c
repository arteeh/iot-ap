#include "usart.h"

// SPDL USART code, uit andere opdrachten en workshops gehaald

void initUsart(void)
{
	GPIO_InitTypeDef  gpio;
	USART_InitTypeDef usart;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	gpio.GPIO_Mode			= GPIO_Mode_AF;
	gpio.GPIO_Pin			= GPIO_Pin_9 | GPIO_Pin_10;
	gpio.GPIO_Speed			= GPIO_Speed_50MHz;
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	
	GPIO_Init(GPIOA, &gpio);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_StructInit(&usart);
	
	usart.USART_BaudRate	= 115200;
	
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);
}

void uPutString(char *str)
{
	while(*str)
	{
		if(*str == '\n')
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){;}
			USART_SendData(USART1, '\r');
		}
		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){;}
		USART_SendData(USART1, *str++);
	}
}

void uClearScreen(void)
{
	char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
	char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home
	
	uPutString(cmd1);
	uPutString(cmd2);
}

char * uIntToAscii(int16_t i, char *p)
{
	int16_t t1, t2;
	char h[10];
	
	t1 = t2 = 0;
	
	if (i < 0)
	{
		p[t2] = '-';
		t2++;
		i = -i;
	}
	
	if(i<1000){ p[t2++]='0'; }
	if(i< 100){ p[t2++]='0'; }
	if(i<  10){ p[t2++]='0'; }
	
	do
	{
	h[t1] = i % 10 + 48;
		t1++;
		i = i / 10;
	} while (i > 0);
	
	while (t1 > 0)
	{
	p[t2++] = h[--t1];
	}
	
	p[t2] = '\0';
	
	return(p);
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
