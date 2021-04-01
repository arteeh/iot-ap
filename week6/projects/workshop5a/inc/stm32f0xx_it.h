#ifndef STM32F0XX_IT_H
#define STM32F0XX_IT_H

#include <stdbool.h>
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "morse.h"

#define MILLISECONDE ((SystemCoreClock * 0.001)-1)

uint8_t buffer[11] = {0}; // Make sure last character is '\0'

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif // STM32F0XX_IT_H
