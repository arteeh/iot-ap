/******************************************************************************
 * File           : Main program - Output Compare
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void init_GPIO(void);
void init_Timer3(void);

// ----------------------------------------------------------------------------
// Configure Pin C8 to alternate function 1 (TIM3_CH3)
// ----------------------------------------------------------------------------
void init_GPIO(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;  

  //(#) Configure the TIM pins by configuring the corresponding GPIO pins
  //    This is LED4 on STM32F0-Discovery
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_1);
}

// ----------------------------------------------------------------------------
// Configure Timer 3 to drive OC3 toggle every 1 sec.
// ----------------------------------------------------------------------------
void init_Timer3(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef       TIM_OCInitStructure;
  
  //[..] To use the Timer in Output Compare mode, the following steps are mandatory:

  //(#) Enable TIM clock using 
  //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  

  //(#) Configure the Time base unit as described in the first part of this 
  //    driver, if needed, else the Timer will run with the default 
  //    configuration:
  //    (++) Autoreload value = 0xFFFF.
  //    (++) Prescaler value = 0x0000.
  //    (++) Counter mode = Up counting.
  //    (++) Clock Division = TIM_CKD_DIV1.
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period        = 1000 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler     = (SystemCoreClock / 1000) - 1;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  //(#) Fill the TIM_OCInitStruct with the desired parameters including:
  //    (++) The TIM Output Compare mode: TIM_OCMode.
  //    (++) TIM Output State: TIM_OutputState.
  //    (++) TIM Pulse value: TIM_Pulse.
  //    (++) TIM Output Compare Polarity : TIM_OCPolarity.
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 500;
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
  
  //(#) Call TIM_OCxInit(TIMx, &TIM_OCInitStruct) to configure the desired 
  //    channel with the corresponding configuration.
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
  TIM_Cmd(TIM3, ENABLE);	
}


// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  // Initialize User Button on STM32F0-Discovery
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

  init_GPIO();
  init_Timer3();
	
  while(1);
}

