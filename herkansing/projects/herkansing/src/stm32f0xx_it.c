#include "stm32f0xx_it.h"

void ADC1_COMP_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1, ADC_FLAG_EOSEQ) != RESET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOSEQ);
		
		// Start the next conversion
		ADC_StartOfConversion(ADC1);
	}
}

void EXTI0_1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	
	// Restart the game
	gameInit();
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		gameUpdate();
	}
}

void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
		
		// ADC value is 0 to 63, should be 0 to 7
		paddle = (ADC1->DR) * 7 / 63;
		
		// Draw the paddle to the screen
		movePaddle();
		displayUpdate();
	}
}



void HardFault_Handler(void)
{
	// Go to infinite loop when Hard Fault exception occurs
	while (1){}
}

void NMI_Handler(void){}
void SVC_Handler(void){}
void PendSV_Handler(void){}
void SysTick_Handler(void){}
