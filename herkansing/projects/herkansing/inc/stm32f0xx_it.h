#ifndef STM32F0XX_IT_H
#define STM32F0XX_IT_H

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "game.h"

void ADC1_COMP_IRQHandler(void);
void EXTI0_1_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM14_IRQHandler(void);
void HardFault_Handler(void);
void NMI_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif // STM32F0XX_IT_H
