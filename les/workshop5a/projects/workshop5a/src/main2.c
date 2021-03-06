/******************************************************************************
 * File           : Main program - ADC Continuous Conversion mode
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "helper.h"

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void  ADC_Setup(void);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  uint16_t adc;
  char str[10];
  
  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  // Initialize User Button on STM32F0-Discovery
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
  USART_Setup();
  USART_Clearscreen();
  USART_Putstr("Workshop 5: ADC Continuous Conversion mode\n");
  
  ADC_Setup();
  
  // Configure channel 11 GPIOC I/O-pin 1
  ADC_ChannelConfig(ADC1, ADC_Channel_11 , ADC_SampleTime_239_5Cycles);
  
  // Start the first conversion
  ADC_StartOfConversion(ADC1);

  while(1)
  {
    // Delay ~0.2 sec.
    Delay(SystemCoreClock/8/5);
    
    // Get the conversion result
    adc = ADC_GetConversionValue(ADC1);
    
    // Output the value    
    USART_Putstr("ADC value: ");
    USART_itoa((int16_t)adc, str);
    USART_Putstr(str);
    USART_Putstr("\r");
  }
}

/**
  * @brief  This function sets PC1 to analog mode, initializes and
  *         calibrates ADC1.
  * @param  None
  * @retval None
  */
void ADC_Setup(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef  ADC_InitStructure;
 
  //(#) Enable the ADC interface clock using 
  //    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  //(#) ADC pins configuration
  //   (++) Enable the clock for the ADC GPIOs using the following function:
  //        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOx, ENABLE);   
  //   (++) Configure these ADC pins in analog mode using GPIO_Init();  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //(#) Configure the ADC conversion resolution, data alignment, external
  //    trigger and edge, scan direction and Enable/Disable the continuous mode
  //    using the ADC_Init() function.
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  // Calibrate ADC before enabling
  ADC_GetCalibrationFactor(ADC1);

  //(#) Activate the ADC peripheral using ADC_Cmd() function.
  ADC_Cmd(ADC1, ENABLE);  
  
  // Wait until ADC enabled
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET); 
}
