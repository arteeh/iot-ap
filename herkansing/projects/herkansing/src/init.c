#include "init.h"

void gpio(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	
	gpio.GPIO_Mode	= GPIO_Mode_AF;
	gpio.GPIO_Pin	= GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	gpio.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed	= GPIO_Speed_50MHz;
	
	// Enable GPIOA clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	// Alternate Function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4 | GPIO_PinSource5 | GPIO_PinSource7, GPIO_AF_0);
	
	GPIO_Init(GPIOA,&gpio);
}

void spi(void)
{
	SPI_InitTypeDef spi;
	SPI_StructInit(&spi);

	spi.SPI_Direction			= SPI_Direction_1Line_Tx;
	spi.SPI_Mode				= SPI_Mode_Master;
	spi.SPI_DataSize			= SPI_DataSize_16b;
	spi.SPI_CPOL				= SPI_CPOL_High;
	spi.SPI_CPHA				= SPI_CPHA_1Edge;
	spi.SPI_NSS					= SPI_NSS_Hard;
	spi.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_128;
	spi.SPI_FirstBit 			= SPI_FirstBit_MSB;
	
	// Enable SPI clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_Init(SPI1,&spi);
	
	SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_NSSPulseModeCmd(SPI1, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
}

void timer2(void)
{
	TIM_TimeBaseInitTypeDef	tim2;
	NVIC_InitTypeDef		nvic;
	
	// Enable TIM2's clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	tim2.TIM_CounterMode	= TIM_CounterMode_Up;
	tim2.TIM_Period			= SPEED_INITIAL - 1;
	tim2.TIM_Prescaler		= (SystemCoreClock / 1000) - 1;
	TIM_TimeBaseInit(TIM2, &tim2);
	
	nvic.NVIC_IRQChannel			= TIM2_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&nvic);
	
	// Enable the interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// Start the counter
	TIM_Cmd(TIM2, ENABLE);
}

void timer3(void)
{
	
}

void timer14(void)
{
	
}

void button(void)
{
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	gpio.GPIO_Mode			= GPIO_Mode_IN;
	gpio.GPIO_PuPd			= GPIO_PuPd_NOPULL;
	gpio.GPIO_Pin			= GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpio);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	exti.EXTI_Line			= EXTI_Line0;
	exti.EXTI_Mode			= EXTI_Mode_Interrupt;
	exti.EXTI_Trigger		= EXTI_Trigger_Falling;
	exti.EXTI_LineCmd		= ENABLE;
	EXTI_Init(&exti);
	
	nvic.NVIC_IRQChannel			= EXTI0_1_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&nvic);
}
