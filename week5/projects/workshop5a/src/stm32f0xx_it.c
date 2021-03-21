#include "stm32f0xx_it.h"
#include "STM32F0_discovery.h"
#include "oing.h"

// Timer 3 interrupt handler
void TIM3_IRQHandler(void)
{
	static uint16_t samplePointer = 0;
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	if(samplePointer >= oingLength) samplePointer = 0;
	
	// For PWM, set compare to the current sample
	TIM_SetCompare1(TIM14,oingData[samplePointer]);
	
	// For DAC, send sample data to channel
	DAC_SetChannel1Data(DAC_Align_8b_R,oingData[samplePointer]);
	
	samplePointer++;
}

// Push button interrupt handler
void EXTI0_1_IRQHandler(void)
{
	static uint8_t isDac = 0;
	
	if(isDac)
	{
		DAC_Cmd(DAC_Channel_1, DISABLE);
		TIM_Cmd(TIM14, ENABLE);
		isDac = 0;
	}
	else
	{
		DAC_Cmd(DAC_Channel_1, ENABLE);
		TIM_Cmd(TIM14, DISABLE);
		isDac = 1;
	}
	
	STM_EVAL_LEDToggle(LED3);
	STM_EVAL_LEDToggle(LED4);
	
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
