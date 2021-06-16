#include "init.h"

// GPIO init for SPI pins
void gpio(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	
	// PA4, PA5 & PA7 to alternate function mode (SPI).
	// Lowest speed because we don't need more
	gpio.GPIO_Mode	= GPIO_Mode_AF;
	gpio.GPIO_Pin	= GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	gpio.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed	= GPIO_Speed_Level_2;
	
	// Enable GPIOA clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	// Alternate function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4 | GPIO_PinSource5 | GPIO_PinSource7, GPIO_AF_0);
	
	GPIO_Init(GPIOA,&gpio);
}

// SPI init for max7219 display
void spi(void)
{
	SPI_InitTypeDef spi;
	SPI_StructInit(&spi);

	// Master only, 16 bits, hardware CS, 128 baud rate prescaler
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

// ADC init for potentiometer reading
void adc(void)
{
	GPIO_InitTypeDef gpio;
	ADC_InitTypeDef adc;
	NVIC_InitTypeDef nvic;

	// Enable ADC and GPIOC clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	// Set PC1 to analog input
	gpio.GPIO_Pin					= GPIO_Pin_1;
	gpio.GPIO_Mode					= GPIO_Mode_AN;
	gpio.GPIO_PuPd					= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio);
	
	// We don't need more accuracy than 3 bits (0 to 7)
	// So choose the smallest resolution, which is 6 bits (0 to 63)
	// Smaller resolution means faster conversions.
	// Be sure to call an interrupt handler when conversion is finished.
	adc.ADC_Resolution				= ADC_Resolution_6b;
	adc.ADC_ContinuousConvMode		= DISABLE;
	adc.ADC_ExternalTrigConv		= ADC_IER_ADRDYIE;
	adc.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;
	adc.ADC_DataAlign				= ADC_DataAlign_Right;
	adc.ADC_ScanDirection			= ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &adc);
	
	// Calibrate the ADC
	ADC_GetCalibrationFactor(ADC1);
	
	// Start
	ADC_Cmd(ADC1, ENABLE);
	
	// Wait till ADC is ready
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);
	
	// Set the channel (PC1 = channel 11)
	ADC_ChannelConfig(ADC1, ADC_Channel_11 , ADC_SampleTime_239_5Cycles);
	
	// Set up the ADC interrupt handler
	nvic.NVIC_IRQChannel = ADC1_COMP_IRQn;
	nvic.NVIC_IRQChannelPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
    
	NVIC_Init(&nvic);
    
	//Enable End of Interrupt
	ADC_ITConfig(ADC1, ADC_IT_EOSEQ, ENABLE);
	
	// Initial conversion
	ADC_StartOfConversion(ADC1);
}

// Initializer for the blue pushbutton to call an interrupt handler
void button(void)
{
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	
	// Make sure GPIOA and EXTI clocks are enabled
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// Set PA0 to input
	gpio.GPIO_Mode			= GPIO_Mode_IN;
	gpio.GPIO_PuPd			= GPIO_PuPd_NOPULL;
	gpio.GPIO_Pin			= GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpio);
	
	// Enable EXTI for PA0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	exti.EXTI_Line			= EXTI_Line0;
	exti.EXTI_Mode			= EXTI_Mode_Interrupt;
	exti.EXTI_Trigger		= EXTI_Trigger_Falling;
	exti.EXTI_LineCmd		= ENABLE;
	EXTI_Init(&exti);
	
	// Call an interrupt handler when the button is pressed
	nvic.NVIC_IRQChannel			= EXTI0_1_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&nvic);
}

// Timer to update the game's ball
void timer2(void)
{
	TIM_TimeBaseInitTypeDef	tim2;
	NVIC_InitTypeDef		nvic;
	
	// Enable TIM2's clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 1khz, Period is equivalent to how often
	// the interrupt should get called in milliseconds.
	// See game.h for initial value
	tim2.TIM_CounterMode	= TIM_CounterMode_Up;
	tim2.TIM_Period			= SPEED_INITIAL - 1;
	tim2.TIM_Prescaler		= (SystemCoreClock / 1000) - 1;
	TIM_TimeBaseInit(TIM2, &tim2);
	
	// Call the handler on interrupt
	nvic.NVIC_IRQChannel			= TIM2_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&nvic);
	
	// Enable the interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// Start the counter
	TIM_Cmd(TIM2, ENABLE);
}

// Timer to generate a beep for the speaker
void timer3(void)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim3;
	TIM_OCInitTypeDef tim3OC;
	
	GPIO_StructInit(&gpio);
	TIM_TimeBaseStructInit(&tim3);
	TIM_OCStructInit(&tim3OC);
	
	// Make sure GPIOC and timer 3 clocks are enabled
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// Set PC7 to alternate function
	// Pull down to reduce static noise
	gpio.GPIO_Mode				= GPIO_Mode_AF;
	gpio.GPIO_Pin				= GPIO_Pin_7;
	gpio.GPIO_PuPd				= GPIO_PuPd_DOWN;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
	
	// We don't need to set a prescaler and autoreload here,
	// that is done in game.c (beepHit & beepLose) when the
	// beep actually happens
	tim3.TIM_ClockDivision		= TIM_CKD_DIV1;
	tim3.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&tim3);
	
	tim3OC.TIM_OCMode			= TIM_OCMode_PWM1;
	tim3OC.TIM_OutputState		= TIM_OutputState_Enable;
	tim3OC.TIM_Pulse			= 0;
	tim3OC.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &tim3OC);
	
	// Enable alternate function
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_1);
	
	// Start
	TIM_Cmd(TIM3,ENABLE);
}

// Timer to update the paddle location in the game
void timer14(void)
{
	TIM_TimeBaseInitTypeDef	tim14;
	NVIC_InitTypeDef		nvic;
	
	// Enable TIM14's clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	// prescaler 1khz, period 50 for 50ms
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
