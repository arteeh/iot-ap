/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

// time defines for the delay function
#define DELAY_100_US			(SystemCoreClock/8/10000)
#define DELAY_100_MS			(SystemCoreClock/8/10)
#define DELAY_10_MS				(SystemCoreClock/8/100)

// general defines
#define SHIFT_8 					8
#define DECODE_REG				0x09
#define NO_DECODE					0x00
#define INTENSITY_REG 		0x0a
#define SCAN_LIMIT				0x0b
#define LEDS_8						0x07
#define MODE_REG					0x0c
#define NORMAL_MODE				0x01
#define TEST_REG					0x0f
#define NO_TEST						0x00
#define TEST							0x01
#define MAX_INTENSITY			15
#define MIN_INTENSITY			0
#define COL_MAX						8
#define COL_MIN						1
#define ROW_MAX						8
#define ROW_MIN						1
#define CLEAR_ALL					0x00

#define ROW_FIVE 					5

typedef enum
{
	FADE_IN,
	FADE_OUT
}fader_t;

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void GPIO_SPI_init(void); // inits all the port and SPI configuration
void max7219_init(void); // inits the max7219 IC
void delay(const int d);
void write_register(uint8_t address, uint8_t data); // SPI send function
void intensity(fader_t fade); // function for the led brightness
void clear_all(void); // clears all the leds
void write_led(uint8_t led_row, uint8_t led_col);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  GPIO_SPI_init();
	max7219_init();
	clear_all();
  
	while(1)
  {
		// a small smile, neutral and, sad animation
//		uint8_t smile[8] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
//		uint8_t sad[8] = {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};
//		uint8_t neutral[8] = {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};
//		uint8_t led_row = 0;
//		
//		while(led_row < COL_MAX)
//		{
//			write_led(led_row+1, smile[led_row]);
//			led_row++;
//		}
//		led_row = 0;
//		while(led_row < COL_MAX)
//		{
//			write_led(led_row+1, neutral[led_row]);
//			led_row++;
//		}
//		led_row = 0;
//		while(led_row < COL_MAX)
//		{
//			write_led(led_row+1, sad[led_row]);
//			led_row++;
//		}
		
		uint8_t led_col = COL_MIN;
		uint8_t led_row = ROW_FIVE;
		
		while(led_col < COL_MAX)
		{
			write_led(led_row, led_col);
			led_col++;
		}
		while(led_col > COL_MIN)
		{
			write_led(led_row, led_col);
			led_col--;
		}
	}
}

void write_register(uint8_t address, uint8_t data)
{
	// msb needs to be the address thats why the address is shifted 8 bits left
	uint16_t bits = (uint16_t) (address << SHIFT_8) | data;
	
	SPI_I2S_SendData16(SPI1, bits);
	delay(DELAY_100_US); // to process data take ~100 uS
}

void write_led(uint8_t led_row, uint8_t led_col)
{
	uint8_t led_c = (1 << (led_col-1));
	
	intensity(FADE_IN);
	write_register(led_row, led_c);
	intensity(FADE_OUT);
}

void intensity(fader_t fade)
{	
	volatile int intensity;
	
	if(fade == FADE_IN)
	{
		for(intensity=MIN_INTENSITY; intensity<MAX_INTENSITY; intensity++)
		{
			write_register(INTENSITY_REG, intensity);
			delay(DELAY_10_MS);
		}
	}
	else if(fade == FADE_OUT)
	{
		for(intensity=MAX_INTENSITY; intensity>MIN_INTENSITY; intensity--)
		{
			write_register(INTENSITY_REG, intensity);
			delay(DELAY_10_MS);
		}
	}
}

void max7219_init(void)
{
	write_register(DECODE_REG, NO_DECODE); 				// no decoding
	write_register(INTENSITY_REG, MIN_INTENSITY); // brightness intensity
	write_register(SCAN_LIMIT, LEDS_8); 					// scan limit = 8 LEDs
	write_register(MODE_REG, NORMAL_MODE); 				// power down=0, normal mode=1
	write_register(TEST_REG, NO_TEST); 						// no test display
}

void GPIO_SPI_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); // GPIO clock enabled
	GPIO_StructInit(&GPIO_InitStructure);

  // Setup NSS (PA4), SCK (PA5), and MOSI (PA7) pin
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	// Sets the folowing Pins on Alternate Function 0
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); // SPI clock enabled
	
	// SPI configuration
	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_Tx;											// Direction only transmit
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;														// Master mode
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;											// Data size: 16 bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;															// Clock polarity: High
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;														// Clock phase: 1 Edge
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;																// Slave Select: Hardware
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		// BaudRate prescaler: 32 .. 256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;										  // First bit: MSB
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE); // SPI enabled
	
	// control of the NSS by hardware
	SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_NSSPulseModeCmd(SPI1, ENABLE);
}

// clears all the leds
void clear_all(void)
{
	uint8_t row;
	for(row=ROW_MIN; row<=ROW_MAX; row++)
	{
		write_register(row, CLEAR_ALL);
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

