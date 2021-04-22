#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0_discovery.h"
#include "usart.h"

#define HONDERD_MS (SystemCoreClock / 10)

void initGpio(void);
void initTimer(void);
void initDma(void);

int main(void)
{
	// Configureer de ingebouwde ledjes
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	
	initUsart();
	initGpio();
	initTimer();
	initDma();

	uClearScreen();
	
	uPutString("\nHallo!\n\n");
	
	// Roep de Systick interrupt handler elke 100 milliseconden aan
	SysTick_Config(HONDERD_MS);
	
	while(1)
	{
		// Stop mode, schiet aan bij systick interrupt.
		PWR_EnterSleepMode(PWR_STOPEntry_WFI);
	}
}

// PC7 in alternate function mode
void initGpio(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	gpio.GPIO_Mode				= GPIO_Mode_AF;
	gpio.GPIO_Pin				= GPIO_Pin_7;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_1);
}

// Timer 3 op 1khz voor speaker bliep
void initTimer(void)
{
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef timOC;
	
	TIM_TimeBaseStructInit(&tim);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// 1khz
	tim.TIM_ClockDivision		= TIM_CKD_DIV1;
	tim.TIM_CounterMode			= TIM_CounterMode_Up;
	tim.TIM_Period				= 5999;
	tim.TIM_Prescaler			= 8;
	TIM_TimeBaseInit(TIM3,&tim);
	
	// Timer 3 Output Compare (PWM)
	timOC.TIM_OCMode			= TIM_OCMode_PWM1;
	timOC.TIM_OCIdleState		= 2;
	timOC.TIM_OutputState		= TIM_OutputState_Enable;
	timOC.TIM_Pulse				= 0;
	timOC.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &timOC);
	
	TIM_Cmd(TIM3,ENABLE);
}

// DMA helaas niet op tijd kunnen implementeren
void initDma(void)
{
	DMA_InitTypeDef dma;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel3);
	
	// DMA kanaal om USART te ontvangen
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
	
	// USART1_Rx DMA aan
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	// DMA1 kanaal 3 aan (USART1_Rx)
	DMA_Cmd(DMA1_Channel3, ENABLE);
}
