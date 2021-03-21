#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

void initButton(void);
void initGpio(void);
void initTim3(void);
void initTim14(void);
void initTim3Interrupt(void);
void initDac(void);

int main(void)
{
	initDac();
	initGpio();
	initTim3();
	initTim3Interrupt();
	initTim14();
	initButton();
	
	// Main simply sets CPU to sleep whenever an interrupt is finished.
	// Will exit sleep when interrupt gets called.
	while(1)
	{
		PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
	}
	
	return 0;
}

// Initialize the buttons and leds for switching between PWM and DAC
void initButton(void)
{
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	
	// Connect USER button to EXTI line
	// Enable the BUTTON clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// Set button pin as input
	gpio.GPIO_Mode			= GPIO_Mode_IN;
	gpio.GPIO_PuPd			= GPIO_PuPd_NOPULL;
	gpio.GPIO_Pin			= GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpio);
	
	// Connect EXTI line to Button GPIO pin
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	// Configure Button EXTI line
	exti.EXTI_Line			= EXTI_Line0;
	exti.EXTI_Mode			= EXTI_Mode_Interrupt;
	exti.EXTI_Trigger		= EXTI_Trigger_Falling;
	exti.EXTI_LineCmd		= ENABLE;
	EXTI_Init(&exti);
	
	// Configure button interrupt
	nvic.NVIC_IRQChannel = EXTI0_1_IRQn;
	nvic.NVIC_IRQChannelPriority = 3;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

// Set pin A4 to alternate function mode
void initGpio(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	gpio.GPIO_Mode				= GPIO_Mode_AF;
	gpio.GPIO_Pin				= GPIO_Pin_4;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_4);
}

// Init Timer 3 for sampling the oing at 8khz
void initTim3(void)
{
	TIM_TimeBaseInitTypeDef tim3;
	TIM_OCInitTypeDef tim3OC;
	
	TIM_TimeBaseStructInit(&tim3);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// 8khz
	// Prescaler = 0 (natte vinger werk)
	// Ticks  = SystemCoreClock / (Prescaler + 1)
	// Ticks  = 48.000.000      / (0         + 1) = 48.000.000
	// Period = Ticks           / 8khz - 1
	// Period = 48.000.000      / 8000 - 1 = 5999
	
	tim3.TIM_ClockDivision		= TIM_CKD_DIV1;
	tim3.TIM_CounterMode		= TIM_CounterMode_Up;
	tim3.TIM_Period				= 5999;
	tim3.TIM_Prescaler			= 0;
	TIM_TimeBaseInit(TIM3,&tim3);
	
	// Timer 3 Output Compare
	tim3OC.TIM_OCMode			= TIM_OCMode_Toggle;
	tim3OC.TIM_OutputState		= TIM_OutputState_Enable;
	tim3OC.TIM_Pulse			= 0;
	tim3OC.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &tim3OC);
	
	TIM_Cmd(TIM3,ENABLE);
}

// Enable the interrupt for timer 3
void initTim3Interrupt(void)
{
	NVIC_InitTypeDef initNvic;

	initNvic.NVIC_IRQChannel			= TIM3_IRQn;
	initNvic.NVIC_IRQChannelPriority	= 0;
	initNvic.NVIC_IRQChannelCmd			= ENABLE;
	
	NVIC_Init(&initNvic); 
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM3, ENABLE);
}

// Init Timer 14 for PWM signal at 60khz
void initTim14(void)
{
	TIM_TimeBaseInitTypeDef tim14;
	TIM_OCInitTypeDef tim14OC;
	
	TIM_TimeBaseStructInit(&tim14);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	// 60khz
	// Prescaler = 2 (natte vinger werk)
	// Ticks  = SystemCoreClock / (Prescaler + 1)
	// Ticks  = 48.000.000      / (2         + 1) = 16.000.000
	// Period = Ticks           / 60khz - 1
	// Period = 16.000.000      / 60000 - 1 = 265
	
	tim14.TIM_ClockDivision		= TIM_CKD_DIV1;
	tim14.TIM_CounterMode		= TIM_CounterMode_Up;
	tim14.TIM_Period			= 265;
	tim14.TIM_Prescaler			= 2;
	TIM_TimeBaseInit(TIM14,&tim14);
	
	// Timer 14 Output Compare
	tim14OC.TIM_OCMode      = TIM_OCMode_PWM1;
	tim14OC.TIM_OutputState = TIM_OutputState_Enable;
	tim14OC.TIM_Pulse       = 0;
	tim14OC.TIM_OCPolarity  = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &tim14OC);
	
	TIM_Cmd(TIM14,ENABLE);
}

// Init the DAC
void initDac(void)
{
	GPIO_InitTypeDef gpio;
	DAC_InitTypeDef	dac;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	// Set pin to analog mode
	gpio.GPIO_Pin				= GPIO_Pin_4;
	gpio.GPIO_Mode				= GPIO_Mode_AN;
	gpio.GPIO_PuPd				= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);
	
	dac.DAC_Trigger				= DAC_Trigger_None;
	dac.DAC_OutputBuffer		= DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1,&dac);
	
	DAC_Cmd(DAC_Channel_1,ENABLE);
}
