#include "display.h"

// A buffer to keep track of what's on the display
// without having to read the device itself
uint8_t buffer[8];

// Initialize the max7219 device
void display(void)
{
	GPIO_InitTypeDef gpio;
	SPI_InitTypeDef spi;
	
	GPIO_StructInit(&gpio);
	SPI_StructInit(&spi);
	
	// Enable GPIOA clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	gpio.GPIO_Mode	= GPIO_Mode_AF;													// Alternative Function
	gpio.GPIO_Pin		= GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;	// Enable pins 4 5 and 7
	gpio.GPIO_PuPd	= GPIO_PuPd_NOPULL;											// No pull
	gpio.GPIO_Speed	= GPIO_Speed_50MHz;											// Fast
	GPIO_Init(GPIOA,&gpio);
	
	// GPIO alternate function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4 | GPIO_PinSource5 | GPIO_PinSource7, GPIO_AF_0);

	spi.SPI_Direction					= SPI_Direction_1Line_Tx;			// Direction only transmit
	spi.SPI_Mode							= SPI_Mode_Master;						// Master mode
	spi.SPI_DataSize					= SPI_DataSize_16b;						// Data size: 16 bits
	spi.SPI_CPOL							= SPI_CPOL_High;							// Clock polarity: High
	spi.SPI_CPHA							= SPI_CPHA_1Edge;							// Clock phase: 1 Edge
	spi.SPI_NSS								= SPI_NSS_Hard;								// Slave Select: Hard
	spi.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_128;	// Baud rate prescaler: 128
	spi.SPI_FirstBit					= SPI_FirstBit_MSB;						// First bit: MSB
	SPI_Init(SPI1,&spi);
	
	// Enable SPI1 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// Not sure what this means, but it works
	SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_NSSPulseModeCmd(SPI1, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
	
	// Set the display's registers
	displayWrite(displayRegDecodeMode, 0x00);							// No decoding
	displayWrite(displayRegIntensity, displayIntensity);	// Brightness intensity
	displayWrite(displayRegScanLimit, 0x07);							// Scan limit = 8 LEDs
	displayWrite(displayRegShutdown, 0x01);								// Power down = 0, normal = 1
	displayWrite(displayRegDisplayTest, 0x00);						// No test display
}

// Clear the buffer and the display
void displayClear(void)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		buffer[i] = 0;
	}
	displayUpdate();
}

// Push the buffer to the display
void displayUpdate(void)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		displayWrite(i+1,buffer[i]);
	}
}

// Write something to SPI
void displayWrite(uint8_t address, uint8_t data)
{
	// The address and data bytes need to be combined into a 16 bit variable to send.
	// Address most significant, data least significant
	uint16_t combined = ((uint16_t)address << 8) | data;
	
	// Write to the display
	SPI_I2S_SendData16(SPI1,combined);
	
	// Sending messages too fast breaks everything, so wait a thousandth of a second.
	// This is the smallest delay that works for me.
	//delay(SECOND/10000);
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
