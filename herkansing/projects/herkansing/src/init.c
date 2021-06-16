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

void adc(void)
{
	GPIO_InitTypeDef gpio;
	ADC_InitTypeDef adc;
	NVIC_InitTypeDef nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	gpio.GPIO_Pin					= GPIO_Pin_1;
	gpio.GPIO_Mode					= GPIO_Mode_AN;
	gpio.GPIO_PuPd					= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio);
	
	// We don't need more accuracy than 3 bits (0 to 7)
	// So choose the smallest resolution, which is 6 bits (0 to 63)
	adc.ADC_Resolution				= ADC_Resolution_6b;
	adc.ADC_ContinuousConvMode		= DISABLE;
	adc.ADC_ExternalTrigConv		= ADC_IER_ADRDYIE;
	adc.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;
	adc.ADC_DataAlign				= ADC_DataAlign_Right;
	adc.ADC_ScanDirection			= ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &adc);
	
	ADC_GetCalibrationFactor(ADC1);
	
	ADC_Cmd(ADC1, ENABLE);
	
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_11 , ADC_SampleTime_239_5Cycles);
	
	nvic.NVIC_IRQChannel = ADC1_COMP_IRQn;
	nvic.NVIC_IRQChannelPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
    
	NVIC_Init(&nvic);
    
	//Enable End of Interrupt
	ADC_ITConfig(ADC1, ADC_IT_EOSEQ, ENABLE);
	
	ADC_StartOfConversion(ADC1);
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
	TIM_TimeBaseInitTypeDef tim3;
	TIM_OCInitTypeDef tim3OC;
	NVIC_InitTypeDef nvic;
	
	TIM_TimeBaseStructInit(&tim3);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	tim3.TIM_ClockDivision		= TIM_CKD_DIV1;
	tim3.TIM_CounterMode		= TIM_CounterMode_Up;
	tim3.TIM_Period				= 1;
	tim3.TIM_Prescaler			= (SystemCoreClock / 1000) - 1;
	TIM_TimeBaseInit(TIM3,&tim3);
	
	tim3OC.TIM_OCMode			= TIM_OCMode_Toggle;
	tim3OC.TIM_OutputState		= TIM_OutputState_Enable;
	tim3OC.TIM_Pulse			= 0;
	tim3OC.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &tim3OC);

	nvic.NVIC_IRQChannel			= TIM3_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&nvic);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void timer14(void)
{
	TIM_TimeBaseInitTypeDef	tim14;
	NVIC_InitTypeDef		nvic;
	
	// Enable TIM14's clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	tim14.TIM_CounterMode	= TIM_CounterMode_Up;
	tim14.TIM_Period		= 50;
	tim14.TIM_Prescaler		= (SystemCoreClock / 1000) - 1;
	TIM_TimeBaseInit(TIM14, &tim14);
	
	nvic.NVIC_IRQChannel			= TIM14_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&nvic);
	
	// Enable the interrupt
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
	
	// Start the counter
	TIM_Cmd(TIM14, ENABLE);
}
