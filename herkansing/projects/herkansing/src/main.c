#include "main.h"

int main(void)
{
	initGpio();
	initSpi();
	matrixInit();
	
	// Get rid of everything below, user timers
	gameInit();
	while(1)
	{
		gameUpdate();
		delay(SECOND);
	}
}

void initGpio(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	
	gpio.GPIO_Mode	= GPIO_Mode_AF;
	gpio.GPIO_Pin		= GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	gpio.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed	= GPIO_Speed_50MHz;
	
	// Enable GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	// Alternate Function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4 | GPIO_PinSource5 | GPIO_PinSource7, GPIO_AF_0);
	
	GPIO_Init(GPIOA,&gpio);
}

void initSpi(void)
{
	SPI_InitTypeDef spi;
	SPI_StructInit(&spi);

	spi.SPI_Direction					= SPI_Direction_1Line_Tx;
	spi.SPI_Mode							= SPI_Mode_Master;
	spi.SPI_DataSize					= SPI_DataSize_16b;
	spi.SPI_CPOL							= SPI_CPOL_High;
	spi.SPI_CPHA							= SPI_CPHA_1Edge;
	spi.SPI_NSS								= SPI_NSS_Hard;
	spi.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_128;
	spi.SPI_FirstBit 					= SPI_FirstBit_MSB;
	
	// Enable SPI clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_Init(SPI1,&spi);
	
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
