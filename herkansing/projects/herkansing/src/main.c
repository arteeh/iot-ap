#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "display.h"

void button(void);
void speaker(void);
void controller(void);

int main(void)
{
	//button();
	//speaker();
	//controller();
	display();
	
	displayClear();
	displayUpdate();
	
	// Main simply sets CPU to sleep whenever there's no interrupt.
	// Will exit sleep when interrupt gets called.
	while(1)
	{
		// FIXME: STOP is better, see which one works
		PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
		//PWR_EnterSleepMode(PWR_STOPEntry_WFI);
	}
}

void button(void)
{
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;	
	NVIC_InitTypeDef nvic;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	gpio.GPIO_Mode	= GPIO_Mode_IN;
	gpio.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	gpio.GPIO_Pin		= GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpio);

	exti.EXTI_Line		= EXTI_Line0;
	exti.EXTI_Mode		= EXTI_Mode_Interrupt;
	exti.EXTI_Trigger	= EXTI_Trigger_Falling;
	exti.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel					= EXTI0_1_IRQn;
	nvic.NVIC_IRQChannelPriority	= 3;
	nvic.NVIC_IRQChannelCmd				= ENABLE;
	NVIC_Init(&nvic);
}

void speaker(void)
{
	GPIO_InitTypeDef				gpio;
	NVIC_InitTypeDef				nvic;
	TIM_TimeBaseInitTypeDef	tim3;
	TIM_TimeBaseInitTypeDef	tim14;
	TIM_OCInitTypeDef				tim3OC;
	TIM_OCInitTypeDef				tim14OC;
	
	GPIO_StructInit(&gpio);
	TIM_TimeBaseStructInit(&tim3);
	TIM_TimeBaseStructInit(&tim14);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);

	gpio.GPIO_Mode	= GPIO_Mode_AF;
	gpio.GPIO_Pin		= GPIO_Pin_7;
	gpio.GPIO_Speed	= GPIO_Speed_Level_1;
	GPIO_Init(GPIOC,&gpio);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_4);

	// Low beep (400Hz)
	// (SystemCoreClock / Prescaler) / Period
	// (48000000 / 12000) / 10 = 400Hz
	
	// High beep (1kHz)
	// (SystemCoreClock / Prescaler) / Period
	// (48000000 / 48000) / 1 = 1000Hz
	
	tim3.TIM_ClockDivision	= TIM_CKD_DIV1;
	tim3.TIM_CounterMode		= TIM_CounterMode_Up;
	tim3.TIM_Period					= 10;
	tim3.TIM_Prescaler			= 12000;
	TIM_TimeBaseInit(TIM3,&tim3);
	
	tim3OC.TIM_OCMode				= TIM_OCMode_Toggle;
	tim3OC.TIM_OutputState	= TIM_OutputState_Enable;
	tim3OC.TIM_Pulse				= 0;
	tim3OC.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &tim3OC);
	
	nvic.NVIC_IRQChannel					= TIM3_IRQn;
	nvic.NVIC_IRQChannelPriority	= 0;
	nvic.NVIC_IRQChannelCmd				= ENABLE;
	NVIC_Init(&nvic);
	
	// (SystemCoreClock / Prescaler) / Period
	// (48000000 / 24000) / 100 = 20Hz
	
	tim14.TIM_ClockDivision	= TIM_CKD_DIV1;
	tim14.TIM_CounterMode		= TIM_CounterMode_Up;
	tim14.TIM_Period				= 100;
	tim14.TIM_Prescaler			= 24000;
	TIM_TimeBaseInit(TIM14,&tim14);
	
	tim14OC.TIM_OCMode      = TIM_OCMode_PWM1;
	tim14OC.TIM_OutputState = TIM_OutputState_Enable;
	tim14OC.TIM_Pulse       = 0;
	tim14OC.TIM_OCPolarity  = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &tim14OC);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM14,ENABLE);
}

void controller(void)
{
	// ADC
}
