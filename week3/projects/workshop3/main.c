#include "main.h"

int main(void)
{
	initGpio();
	initSpi();
	initMatrix();
	
	while(1)
	{
		// Hello world, one word at a time
		setLetter('H');	delay(SECOND/2);
		setLetter('e');	delay(SECOND/2);
		setLetter('l');	delay(SECOND/2);
		setLetter('l');	delay(SECOND/2);
		setLetter('o');	delay(SECOND/2);
		clear();	delay(SECOND/2);
		setLetter('W');	delay(SECOND/2);
		setLetter('o');	delay(SECOND/2);
		setLetter('r');	delay(SECOND/2);
		setLetter('l');	delay(SECOND/2);
		setLetter('d');	delay(SECOND/2);
		setLetter('!');	delay(SECOND/2);

		clear();
		delay(SECOND);
		
		// Create a ball in the center
		setBall();
		
		// Flash it
		flash();
		
		// Move the ball around by shifting left, right, up and down
		shiftLeft();	delay(SECOND/10);
		shiftLeft();	delay(SECOND/10);
		shiftRight();	delay(SECOND/10);
		shiftRight();	delay(SECOND/10);
		shiftRight();	delay(SECOND/10);
		shiftRight();	delay(SECOND/10);
		shiftLeft();	delay(SECOND/10);
		shiftLeft();	delay(SECOND/10);
		shiftUp();	delay(SECOND/10);
		shiftUp();	delay(SECOND/10);
		shiftDown();	delay(SECOND/10);
		shiftDown();	delay(SECOND/10);
		shiftDown();	delay(SECOND/10);
		shiftDown();	delay(SECOND/10);
		shiftUp();	delay(SECOND/10);
		shiftUp();	delay(SECOND/10);
		
		clear();
		delay(SECOND*2);
	}
}

void initGpio(void)
{
	GPIO_InitTypeDef gpioInit;
	GPIO_StructInit(&gpioInit);
	
	// Alternative Function
	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	
	// Enable pins 4 5 and 7
	gpioInit.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	
	// Nu pull
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	// Fast
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	
	// Enable GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	// Alternative Function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4 | GPIO_PinSource5 | GPIO_PinSource7, GPIO_AF_0);
	
	GPIO_Init(GPIOA,&gpioInit);
}

void initSpi(void)
{
	SPI_InitTypeDef spiInit;
	SPI_StructInit(&spiInit);

	// Direction only transmit
	spiInit.SPI_Direction = SPI_Direction_1Line_Tx;
	
	// Master mode
	spiInit.SPI_Mode = SPI_Mode_Master;
	
	// Data size: 16 bits
	spiInit.SPI_DataSize = SPI_DataSize_16b;
	
	// Clock polarity: High
	spiInit.SPI_CPOL = SPI_CPOL_High;
	
	// Clock phase: 1 Edge
	spiInit.SPI_CPHA = SPI_CPHA_1Edge;
	
	// Slave Select: Hard
	spiInit.SPI_NSS = SPI_NSS_Hard;
	
	// Baud rate prescaler: 128
	spiInit.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	
	// First bit: MSB
	spiInit.SPI_FirstBit = SPI_FirstBit_MSB;
	
	// Enable SPI clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_Init(SPI1,&spiInit);
	
	// Not sure what this means, but it works
	SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_NSSPulseModeCmd(SPI1, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
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
