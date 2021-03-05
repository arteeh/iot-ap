/**
  ******************************************************************************
  * @file    stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
extern char *RXBuffer;
extern uint8_t bufferTail, bufferHead, bufferBegin, bufferEnd, bufferLength, offset;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function implements the SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	static uint32_t ticks=0;

  if (ticks++ ==  0) {GPIOC->BSRR = 0x0200;} // Green LED on
  if (ticks   == 15) {GPIOC->BRR  = 0x0200;} // Green LED off
  if (ticks   == 30) {GPIOC->BSRR = 0x0200;} // Green LED on
  if (ticks   == 45) {GPIOC->BRR  = 0x0200;} // Green LED off
  if (ticks   ==200) {ticks=0;}
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/
/* 	@Author: Anonymous
 *	@Date: 18-02-2021
 *  @Functionality: Interupt Service Routine for handling overflow on buffer. If buffer is full characters are lost and blue light will light up untill buffer has room.
 */
void USART1_IRQHandler(void)
{ 
  if(USART1->ISR & USART_ISR_RXNE)
	{	
		// Input writing over existing characters?
		if(!((bufferHead + offset == bufferTail)||((bufferHead + offset == bufferLength)&&(bufferTail == bufferBegin))))
		{
			// Input put into buffer at bufferHead location
			RXBuffer[bufferHead] = USART1->RDR;
			bufferHead++;
		}
		else
		{
			// Light up blue LED
			GPIOC->BSRR |= GPIO_BSRR_BS_8;
			// Clear registry flag
			USART1->RDR;
		}
		
		
		
		// If bufferHead reaches bufferEnd reset bufferHead to bufferBegin
		if(bufferHead > bufferEnd)
		{
			bufferHead = bufferBegin;
		}
	}
}


/**
  * @brief  This function handles PPP interrupt request
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
