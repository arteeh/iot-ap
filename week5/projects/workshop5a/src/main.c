#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"

#define SECOND (SystemCoreClock/8)

void initGpio(void);
void initPwm(void);
void initDac(void);

int main(void)
{
	initGpio();
	initUsart();
	
	uClearScreen();
	
	initPwm();

	while(1)
	{
		// Enter sleep mode and Wait For Interrupt
		PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
		uPutString("test\n");
		//delay(SECOND);
	}
	
	return 0;
}

// Set pin A4 to alternate function mode
// A9 and A10 for USART
void initGpio(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	gpio.GPIO_Mode				= GPIO_Mode_AF;
	gpio.GPIO_Pin				= GPIO_Pin_4 | GPIO_Pin_9 | GPIO_Pin_10;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz; // OR GPIO_Speed_Level_2; ?
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
}

// Set up the PWM interrupt
void initPwm(void)
{
	TIM_TimeBaseInitTypeDef tim3;
	TIM_TimeBaseInitTypeDef tim14;

	TIM_TimeBaseStructInit(&tim3);
	TIM_TimeBaseStructInit(&tim14);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	// 8khz
	// prescaler 0
	// preload 5999
	
	tim3.TIM_ClockDivision		= TIM_CKD_DIV1;
	tim3.TIM_CounterMode		= TIM_CounterMode_Up;
	tim3.TIM_Period				= 5999;
	tim3.TIM_Prescaler			= 0;
	TIM_TimeBaseInit(TIM3,&tim3);
	
	// 60khz
	// prescaler 0
	// preload 799
	
	tim14.TIM_ClockDivision		= TIM_CKD_DIV1;
	tim14.TIM_CounterMode		= TIM_CounterMode_Up;
	tim14.TIM_Period			= 799;
	tim14.TIM_Prescaler			= 0;
	TIM_TimeBaseInit(TIM14,&tim14);
	
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM14,ENABLE);
}

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
