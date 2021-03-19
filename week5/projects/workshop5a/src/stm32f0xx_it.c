#include "stm32f0xx_it.h"
#include "STM32F0_discovery.h"
#include "usart.h"
#include "oing.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

void TIM3_IRQHandler(void)
{
	//uPutString("irq\n");
	
	//if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	//{
	//	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	//}
	
	//if(samplePointer > oingLength) samplePointer = 0;
	
	//TIM_SetAutoreload(TIM3,oingData[samplePointer]);
	//samplePointer++;
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
