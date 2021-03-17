/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "stdbool.h"
#include "setup.h"

/******************************************************************************

LET OP! je kan de onderdelen testen
door verschillende parameters in setup.h te veranderen!!

 *****************************************************************************/


// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define ONE_TENTH_SECOND   (SystemCoreClock/8/10)
#define ONE_SECOND         (SystemCoreClock/8)
#define DIM_COUNT_MAX 15
#define DIM_COUNT_MIN 0

#define MATRIX_SIZE 8
#define MATRIX_MIN_SIZE 0

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

unsigned int dim_counter = 0;
bool state = true;
bool scroll_increasing = false;

char face_smile[8]=   {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
char face_neutral[8]= {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};
char face_sad[8]=   {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};	


// ----------------------------------------------------------------------------
// Function declarations
// ----------------------------------------------------------------------------
void delay(const int d);
void init_spi(void);
void setup(void);
void init_spi_master(void);
void transfer_SPI1(uint8_t address, uint8_t data);
void init_maxChip(void);
void light_dimming(void);					 
void scrolling_image(void);
void show_image(void);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{

  init_spi_master();
	init_maxChip();
	
  while(1)
  {
		
		if(scrolling){
			scrolling_image();
		}else{
			show_image();
		}
		
		if(dimming_lights){
			light_dimming();
		}
  }
}


void init_spi_master(){
 
    GPIO_InitTypeDef GPIO_InitDef;
    SPI_InitTypeDef SPI_InitDef;
 
    // initialize init structs
    GPIO_StructInit(&GPIO_InitDef);
    SPI_StructInit(&SPI_InitDef);
 
    // initialize clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		
    // initialize A5/SCK A7 alternate function push-pull (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_AF_PP;
		GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitDef);


	  // initialize A5/SCK alternate function
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &GPIO_InitDef);
		
 
    // initialize SPI master
    SPI_InitDef.SPI_Direction = SPI_Direction_Tx;
    SPI_InitDef.SPI_Mode = SPI_Mode_Master;
    SPI_InitDef.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitDef.SPI_CPOL = SPI_CPOL_High;
    SPI_InitDef.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitDef.SPI_NSS = SPI_NSS_Hard;
    SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_Init(SPI1, &SPI_InitDef);
 
    SPI_Cmd(SPI1, ENABLE);
		
		SPI_SSOutputCmd(SPI1, ENABLE);
		SPI_NSSPulseModeCmd(SPI1, ENABLE);
}


void transfer_SPI1(uint8_t address, uint8_t data){
		
    uint16_t bits =  (uint16_t) (address << 8) | data;    
		SPI_I2S_SendData16(SPI1, bits);
	
		delay((SystemCoreClock/8)/1000);
}


void init_maxChip(){
	
	unsigned int a,b;
	
	transfer_SPI1(0x09, 0x00); // using an led matrix (not digits)
	transfer_SPI1(0x0A, 0x01); // Brightness
	transfer_SPI1(0x0B, 0x07); // scanLimit
	transfer_SPI1(0x0C, 0x01); // not in shutdown mode
	transfer_SPI1(0x0F, 0x00); // no display test	
	
	transfer_SPI1(0x0C, 0x00); // In shutdown mode
	for(a = MATRIX_MIN_SIZE; a <= MATRIX_SIZE; a++){
		for(b = MATRIX_MIN_SIZE; a <= MATRIX_SIZE; a++){
			transfer_SPI1(a,b);
		}
	}
	
	transfer_SPI1(0x0C, 0x01); // not in shutdown mode
}


void scrolling_image(){
	int face_counter = 0, scroll_counter = 0, shift_counter = 0;
	
	// scroll_increasing false == scrolling is counting up, else counting down
	if(!scroll_increasing){
		
		shift_counter = MATRIX_MIN_SIZE;
	
		for(scroll_counter = 0; scroll_counter <= MATRIX_SIZE; scroll_counter++){
				
				for(face_counter = 0;face_counter<sizeof(show_face);face_counter++){
					transfer_SPI1(face_counter+1, show_face[face_counter] >> shift_counter);
				}

			shift_counter++;
			delay((SystemCoreClock/64)/2);
			
				if(scroll_counter >= MATRIX_SIZE){
					scroll_increasing = true;
				}
		}
	
	}else{
		
		shift_counter = MATRIX_SIZE+1;
		
		for(scroll_counter = MATRIX_SIZE; scroll_counter >= MATRIX_MIN_SIZE; scroll_counter--){
				
				for(face_counter=0;face_counter<sizeof(show_face);face_counter++){
					transfer_SPI1(face_counter+1, show_face[face_counter] << shift_counter);
				}

			shift_counter--;
			delay((SystemCoreClock/64)/2);
				
			if(scroll_counter <= MATRIX_MIN_SIZE){
				scroll_increasing = false;
			}
		}
	}
}


void show_image(){
	unsigned int face_counter;

		for(face_counter=0;face_counter<sizeof(show_face);face_counter++){
			transfer_SPI1(face_counter+1, show_face[face_counter]);
		}
}


void light_dimming(){
	if(state){
		if(dim_counter < DIM_COUNT_MAX){
			transfer_SPI1(0x0A, dim_counter);
			dim_counter++;
		}else{
			state = false;
		}
	}else{
		if(dim_counter > DIM_COUNT_MIN){
			transfer_SPI1(0x0A, dim_counter);
			dim_counter--;
		}else{
			state = true;
		}
	}
	
	if(!scrolling){
		delay((SystemCoreClock/64)/2);
	}
}


void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}

