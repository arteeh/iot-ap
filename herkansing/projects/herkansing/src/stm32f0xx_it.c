#include "stm32f0xx_it.h"

void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
  
  gameUpdate();
}

void EXTI0_1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	// Restart the game
	gameInit();
	TIM_Cmd(TIM2, ENABLE);
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void NMI_Handler(void){}
void SVC_Handler(void){}
void PendSV_Handler(void){}
void SysTick_Handler(void){}
