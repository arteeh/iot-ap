/******************************************************************************
 * File           : Main program - Timebase
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------

void init_SPI(void);
void sendData(SPI_TypeDef* SPIx, uint8_t rowNumber, uint8_t leds);
void clearDisplay(SPI_TypeDef* SPIx);
void drawLetter(SPI_TypeDef* SPIx, char letter);
void delay(const int d);


// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

#define BRIGHTNESS 100 // Define a brightness percentage
#define BIN_TO_BYTE(b7,b6,b5,b4,b3,b2,b1,b0) ((b7 << 7)+(b6 << 6)+(b5 << 5)+(b4 << 4)+(b3 << 3)+(b2 << 2)+(b1 << 1)+b0) // Assigning a bin value to variable https://www.keil.com/support/docs/1156.htm
#define MILLISECOND SystemCoreClock / 8000 // define 1 millisecond


// ----------------------------------------------------------------------------
// Init functions
// ----------------------------------------------------------------------------
void init_SPI(void)
{
	SPI_InitTypeDef SPI_InitStructure; // Generate an SPI structure
	GPIO_InitTypeDef  GPIO_InitStructure; // Generate a GPIO structure
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); // Enable GPIOA
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF; // Enable alternate functions
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7; // Set pins
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Set speed
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0); //
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0); // Set to alternate function 0
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0); //
	
	GPIO_Init(GPIOA, &GPIO_InitStructure); // Init GPIO
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); // Enable SPI
	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; // Only transmit data
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // STM is master
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; // 16 bits of data to be sent
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // SPI sending data on Rising edge
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // Set clock phase
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard; // Salve select using hardware
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // Set baudrate prescaler to 32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // First bit sent is MSB
	
	SPI_NSSPulseModeCmd(SPI1, ENABLE); // Enable pulsed slave select
	
	SPI_Init(SPI1, &SPI_InitStructure); // Init SPI
	SPI_Cmd(SPI1, ENABLE); // Enable SPI
	
}

void init_matrix(SPI_TypeDef* SPIx)
{
	uint8_t brightnessValue = (BRIGHTNESS /100) * 15;
	
	sendData(SPIx, 0x0F, 0x00); // Disable display test mode
	sendData(SPIx, 0x0B, 0x07); // Set scan limit (Enable all 8 rows)
	sendData(SPIx, 0x09, 0x00); // Set decode mode to no decoding
	clearDisplay(SPIx);
	sendData(SPIx, 0x0A, brightnessValue); // Set low brightness
	sendData(SPIx, 0x0C, 0x01); // Exit shutdown mode
}


// ----------------------------------------013------------------------------------
// Main
// ----------------------------------------------------------------------------

int main(void)
{
	init_SPI();
	init_matrix(SPI1);
	
	
  while(1)
  {
		uint8_t counter; // Set temporary counter
		uint8_t counter2; // Set secondary temporary counter
		char letter; // Set secondary temporary counter for displaying alphabet
		
		
		for(counter = 0; counter < 5; counter++) // Show a beating heart
		{
			sendData(SPI1, 0x01, 0x66); //
			sendData(SPI1, 0x02, 0x99); //
			sendData(SPI1, 0x03, 0x81); //
			sendData(SPI1, 0x04, 0x81); // Draw a big heart
			sendData(SPI1, 0x05, 0x42); //
			sendData(SPI1, 0x06, 0x24); //
			sendData(SPI1, 0x07, 0x18); //
			sendData(SPI1, 0x08, 0x00); //
			delay(MILLISECOND*1000);
			sendData(SPI1, 0x01, 0x00); //
			sendData(SPI1, 0x02, 0x24); //
			sendData(SPI1, 0x03, 0x5A); //
			sendData(SPI1, 0x04, 0x42); // Draw a small heart
			sendData(SPI1, 0x05, 0x24); //
			sendData(SPI1, 0x06, 0x18); //
			sendData(SPI1, 0x07, 0x00); //
			sendData(SPI1, 0x08, 0x00); //
			delay(MILLISECOND*200);
		}
		
		
		for(letter = 97; letter <= 122; letter++) // Show alphabet
		{
			drawLetter(SPI1, letter);
			delay(MILLISECOND*500);
			clearDisplay(SPI1);
			delay(MILLISECOND*100);
		}
		
		for(counter = 0; counter < 5; counter++) // Fade brightness
		{
			for(counter2 = 15; counter2 >= 0; counter2--)
			{
				sendData(SPI1, 0x0A, counter2); // Set brightness
				sendData(SPI1, 0x01, 0xFF); //
				sendData(SPI1, 0x02, 0xFF); //
				sendData(SPI1, 0x03, 0xFF); //
				sendData(SPI1, 0x04, 0xFF); // Turn on all LEDs
				sendData(SPI1, 0x05, 0xFF); //
				sendData(SPI1, 0x06, 0xFF); //
				sendData(SPI1, 0x07, 0xFF); //
				sendData(SPI1, 0x08, 0xFF); //
				delay(MILLISECOND*100);
			}
			
			for(counter2 = 0; counter2 <= 15; counter2++)
			{
				sendData(SPI1, 0x0A, counter2); // Set brightness
				sendData(SPI1, 0x01, 0xFF); //
				sendData(SPI1, 0x02, 0xFF); //
				sendData(SPI1, 0x03, 0xFF); //
				sendData(SPI1, 0x04, 0xFF); // Turn on all LEDs
				sendData(SPI1, 0x05, 0xFF); //
				sendData(SPI1, 0x06, 0xFF); //
				sendData(SPI1, 0x07, 0xFF); //
				sendData(SPI1, 0x08, 0xFF); //
				delay(MILLISECOND*100);
			}
		}
		
		
  }
}

void sendData(SPI_TypeDef* SPIx, uint8_t rowNumber, uint8_t leds) {
	uint16_t data = (rowNumber << 8) | leds; // Shift row number 8 bits to the left, and set LED pattern
	SPI_I2S_SendData16(SPIx, data);
	delay(SystemCoreClock/80000);
}

void clearDisplay(SPI_TypeDef* SPIx)
{
	sendData(SPIx, 0x01, 0x00);
	sendData(SPIx, 0x02, 0x00);
	sendData(SPIx, 0x03, 0x00);
	sendData(SPIx, 0x04, 0x00);
	sendData(SPIx, 0x05, 0x00);
	sendData(SPIx, 0x06, 0x00);
	sendData(SPIx, 0x07, 0x00);
	sendData(SPIx, 0x08, 0x00);
}

void drawLetter(SPI_TypeDef* SPIx, char letter)
{
	switch(letter) // haha grote alfabet functie veel plezier met lezen
	{
		case 'a':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 0, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 1));
			break;
		case 'b':
			sendData(SPIx, 0x01, BIN_TO_BYTE(1, 1, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 0, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 1, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 'c':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 'd':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 0, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 0, 1));
			break;
		case 'e':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 'f':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 1, 1, 1, 0, 0, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 1, 1, 0, 0, 0));
			break;
		case 'g':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 1, 1));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 'h':
			sendData(SPIx, 0x01, BIN_TO_BYTE(1, 1, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 0, 1, 1, 1, 0, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 1, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			break;
		case 'i':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 1, 1, 0, 0, 0));
			break;
		case 'j':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 0, 0, 0, 0, 1, 1, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 'k':
			sendData(SPIx, 0x01, BIN_TO_BYTE(1, 1, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 1, 0, 0, 1, 1, 1, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 0, 0, 0, 1, 0, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 1, 1, 1, 0, 0, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 1, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 0, 0, 1, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			break;
		case 'l':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 1, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 0, 0, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 0, 0, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 0, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 0, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 0, 0, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 'm':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 0, 1, 1, 0, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 0, 0, 1, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 1, 0, 0, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 0, 1, 0, 0, 1));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 1, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 0, 1, 1, 0, 1, 1));
			break;
		case 'n':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 0, 1, 1, 1, 0, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 1, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			break;
		case 'o':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 'p':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 0, 0, 0, 0, 0));
			break;
		case 'q':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 0, 0, 0, 0, 1, 1, 1));
			break;
		case 'r':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 0, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 1, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 0, 0, 0, 0, 0));
			break;
		case 's':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 0));
			break;
		case 't':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 1, 1, 1, 0, 0, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 0, 1, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 0, 0, 1, 1, 1, 0, 0));
			break;
		case 'u':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 0, 1, 1, 1, 1, 0, 0));
			break;
		case 'v':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 1, 0, 0, 1, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 1, 0, 0, 1, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 0, 0, 1, 0, 1, 0, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 0, 0, 0, 1, 0, 0, 0));
			break;
		case 'w':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 1, 0, 0, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 0, 1, 0, 0, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 1, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 0, 1, 0, 1, 1, 0, 0));
			break;
		case 'x':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 0));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 0, 1, 0, 0, 1, 0, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 0, 1, 1, 0, 0, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 1, 0, 0, 1, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			break;
		case 'y':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 1, 0, 0, 1, 1, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 1, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 0, 1, 1, 1, 1, 1, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 1, 0));
			sendData(SPIx, 0x08, BIN_TO_BYTE(0, 1, 1, 1, 1, 1, 0, 0));
			break;
		case 'z':
			sendData(SPIx, 0x01, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x02, BIN_TO_BYTE(0, 0, 0, 0, 0, 0, 0, 0));
			sendData(SPIx, 0x03, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 1));
			sendData(SPIx, 0x04, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x05, BIN_TO_BYTE(0, 0, 0, 0, 1, 1, 1, 0));
			sendData(SPIx, 0x06, BIN_TO_BYTE(0, 1, 1, 1, 0, 0, 0, 0));
			sendData(SPIx, 0x07, BIN_TO_BYTE(1, 0, 0, 0, 0, 0, 0, 1));
			sendData(SPIx, 0x08, BIN_TO_BYTE(1, 1, 1, 1, 1, 1, 1, 1));
			break;
	}
}


void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
