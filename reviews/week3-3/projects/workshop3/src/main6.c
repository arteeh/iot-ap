#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"
#include "stm32f0_discovery.h"

#define ONE_MILLIONTH_SECOND (SystemCoreClock/8/100000)
#define _100ms (SystemCoreClock/8/10)
#define max_amount_of_shifts (8)
#define address_shift (7)
#define data_shift (1)

void writeRegister(uint8_t address, uint8_t data);
void init_GPIO(void);
void init_SPI(void);
void initMatrix(void);
void write_char(void);
void delay(const int d);

int main(void)
{
    init_GPIO();
    init_SPI();
    initMatrix();
	
    while(1)
    {
				delay(_100ms);
				write_char(); 
    }
}

void writeRegister(uint8_t address, uint8_t data)
{
    uint16_t combined = ((uint16_t)address << address_shift) | ((uint16_t)data >> data_shift);
	
    SPI_I2S_SendData16(SPI1,combined);
}

void init_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4 | GPIO_PinSource5 | GPIO_PinSource7, GPIO_AF_0);
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void init_SPI(void)
{
    SPI_InitTypeDef initSpi;
    SPI_StructInit(&initSpi);
	
    //SPI peripheral intializatie 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    initSpi.SPI_Direction = SPI_Direction_1Line_Tx;
    initSpi.SPI_Mode = SPI_Mode_Master;
    initSpi.SPI_DataSize = SPI_DataSize_16b;
    initSpi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    initSpi.SPI_CPOL = SPI_CPOL_High;
    initSpi.SPI_CPHA = SPI_CPHA_1Edge;
    initSpi.SPI_NSS = SPI_NSS_Hard;
    initSpi.SPI_FirstBit = SPI_FirstBit_MSB;
    
    SPI_Init(SPI1, &initSpi);
    
    SPI_SSOutputCmd(SPI1, ENABLE);
    SPI_NSSPulseModeCmd(SPI1, ENABLE);
    SPI_Cmd(SPI1, ENABLE);
}

void initMatrix(void)
{
    writeRegister(0x09, 0x00); // no decoding
    writeRegister(0x0a, 0x03); // brightness intensity
    writeRegister(0x0b, 0x07); // scan limit = 8 LEDs
    writeRegister(0x0c, 0x01); // power down=0, normal mode=1
    writeRegister(0x0f, 0x00); // no test display
}


void write_char(void)
{
	static uint8_t shiftRight = 7;
	static uint8_t shiftLeft = 0;
	
	uint8_t p[] = 
	{
		0x3c,
		0x22,
		0x22,
		0x22,
		0x3c,
		0x20,
		0x20,
		0x20
	};//the char P
	
	volatile uint8_t i;
	
	for(i = 0; i < max_amount_of_shifts; i++)
	{
		if(shiftRight)
		{
			writeRegister(i+1, (p[i] >> shiftRight));//plus 1 for the correct address
		}
		else if(shiftLeft < max_amount_of_shifts)
		{
			writeRegister(i+1, (p[i] << shiftLeft));//plus 1 for the correct address
		}
		
		delay(ONE_MILLIONTH_SECOND);
	}

	if(shiftRight)
	{
		shiftRight--;
	}
	else if(shiftLeft <= max_amount_of_shifts)
	{
		shiftLeft++;
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

