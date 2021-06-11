#include "stm32f0xx_it.h"
#include "STM32F0_discovery.h"

// Timer 3 interrupt handler
void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

// Push button interrupt handler
void EXTI0_1_IRQHandler(void)
{
	// Clear the EXTI line 0 pending bit
	EXTI_ClearITPendingBit(EXTI_Line0);
}

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
void SysTick_Handler(void){}
