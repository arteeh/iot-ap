#include "stm32f0xx_it.h"

// ADC interrupt handler to start a new conversion when the previous one is done
// This way we constantly get the latest potentiometer value
void ADC1_COMP_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1, ADC_FLAG_EOSEQ) != RESET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOSEQ);
		
		// Start the next conversion
		ADC_StartOfConversion(ADC1);
	}
}

// Blue pushbutton handler
void EXTI0_1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	
	// Restart the game
	gameInit();
	TIM_Cmd(TIM2, ENABLE);
}

// Timer for updating the ball
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		moveBall();
	}
}

// Timer for updating the paddle
void TIM14_IRQHandler(void)
{	
	static uint8_t beepCountdown = 0;
	
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
		
		// ADC value is 0 to 63 because it's 6 bits
		// This should be 0 to 6, so divide by 10
		paddle = (ADC1->DR) / 10;
		
		// Draw the paddle to the screen
		movePaddle();
		
		// See if the ball hit the paddle
		switch(checkHit())
		{
			case hit:
				// High beep, wait 200ms (50ms * 4)
				beepHit();
				beepCountdown = 4;
				break;
			case west:
				// Low beep, wait 200ms (50ms * 4)
				beepLose();
				beepCountdown = 4;
				break;
			case east:
				break;
		}
		
		// Count down the beeping, turn the speaker off at 0
		if(beepCountdown > 0) beepCountdown--;
		else TIM_SetCompare2(TIM3, 0);
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
