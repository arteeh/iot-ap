#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0_discovery.h"
#include "usart.h"

void initGpio(void);
void initTim3(void);
void initTim14(void);
void initTim3Interrupt(void);
void initDac(void);
void initDma(void);

int main(void)
{	
	initUsart();
	initDac();
	initGpio();
	initTim3();
	initTim3Interrupt();
	initTim14();
	initDma();
	
	SysTick_Config(MILLISECONDE*100);
	
	while(1)
	{
		PWR_EnterSleepMode(PWR_STOPEntry_WFI);
		//PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
	}
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

void initDma(void)
{
	DMA_InitTypeDef dma;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// De-initialize DMA1 Channel 3    
	DMA_DeInit(DMA1_Channel3);
	
	// DMA channel Rx of USART Configuration
	dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->RDR);
	dma.DMA_MemoryBaseAddr     = (uint32_t)buffer;
	dma.DMA_DIR                = DMA_DIR_PeripheralSRC;
	dma.DMA_BufferSize         = 10;
	dma.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	dma.DMA_Mode               = DMA_Mode_Normal;
	dma.DMA_Priority           = DMA_Priority_High;
	dma.DMA_M2M                = DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel3, &dma);
	
	// Enable USART1_Rx DMA interface
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	// Enable DMA1 Channel 3 (USART1_Rx)
	DMA_Cmd(DMA1_Channel3, ENABLE);
}
