#include "stm32f0xx_it.h"

volatile uint32_t DMA_EndOfTransfer = 0;

void DMA1_Channel1_IRQHandler(void)
{
	// Test on DMA1 Channel1 Transfer Complete interrupt
	if(DMA_GetITStatus(DMA1_IT_TC1))
	{
		// DMA1 finished the transfer of SrcBuffer
		DMA_EndOfTransfer = 1;
		
		// Clear DMA1 Channel1 Half Transfer, Transfer Complete and Global
		// interrupt pending bits
		DMA_ClearITPendingBit(DMA1_IT_GL1);
	}
}

void SysTick_Handler(void)
{
	static char * morseVal; // string of 1 to 4 characters
	static bool reading = true; // 1 time period between every character
	
	if(reading)
	{
		reading = false;
		
		morseVal = morse[buffer[0]-30];
		if(morseVal == '.')		SysTick_Config(MILLISECONDE*100);
		else if(morseVal == '-')	SysTick_Config(MILLISECONDE*300);
		else				SysTick_Config(MILLISECONDE*100);
	}
	else
	{
		reading = true;
	}
}

void TIM3_IRQHandler(void){}

// Push button interrupt handler
void EXTI0_1_IRQHandler(void){}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void ADC1_COMP_IRQHandler(void)
{
  if(ADC_GetITStatus(ADC1, ADC_IT_AWD) != RESET)
  {
    ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
    
    // Turn on Window Watchdog indicator
    STM_EVAL_LEDOn(LED4);
  }
}

void NMI_Handler(void){}
void SVC_Handler(void){}
void PendSV_Handler(void){}
