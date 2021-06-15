#include "display.h"

// The buffer to keep track of what's on the display
// without having to read the device itself
uint8_t buffer[9];

// Initialize the max7219 device
void displayInit(void)
{
	displayWrite(displayRegDecodeMode, 0x00);				// no decoding
	displayWrite(displayRegIntensity, displayIntensity);	// brightness intensity
	displayWrite(displayRegScanLimit, 0x07);				// scan limit = 8 LEDs
	displayWrite(displayRegShutdown, 0x01);					// power down=0, normal=1
	displayWrite(displayRegDisplayTest, 0x00);				// no test display
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
	// The address and data bytes need to be combined
	// into a 16 bit variable to send.
	
	// Address most significant, data least significant
	uint16_t combined = ((uint16_t)address << 8) | data;
	
	 // If SPI is still sending, wait
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	// Send
	SPI_I2S_SendData16(SPI1,combined);
}
