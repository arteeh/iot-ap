/******************************************************************************
 * File           : Main program - DMA USART to RAM example
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "helper.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
uint8_t RxBuffer[11] = {0}; // Make sure last character is '\0'

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  // Initialize User Button on STM32F0-Discovery
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
  USART_Setup();
  USART_Clearscreen();
  USART_Putstr("Workshop 6b: DMA - USART to RAM\n");

  while(1)
  {
    USART_Putstr("Transfer 10 bytes: ");
    
    // ----------------------------------------------------------------------
    //  Transfer 10 bytes with DMA
    // ----------------------------------------------------------------------
    // Enable DMA1 peripheral
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // De-initialize DMA1 Channel 3    
    DMA_DeInit(DMA1_Channel3);

    // DMA channel Rx of USART Configuration
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)RxBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = 10;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable; 
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    
    // Enable USART1_Rx DMA interface
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    
    // Enable DMA1 Channel 3 (USART1_Rx)
    DMA_Cmd(DMA1_Channel3, ENABLE);
        
    // Wait for the USART DMA Rx transfer to complete
    while(DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET){;}
        
    // Display received characters
    USART_Putstr((char *)RxBuffer);
    USART_Putstr("\n");
  }
}
