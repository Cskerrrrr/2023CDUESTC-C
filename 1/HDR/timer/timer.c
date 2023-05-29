#include "timer.h"
#include "sys.h"



void TIM6_init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef Timer_Initx;
	NVIC_InitTypeDef NVIC_Initx;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	Timer_Initx.TIM_ClockDivision = TIM_CKD_DIV1;
  Timer_Initx.TIM_CounterMode = TIM_CounterMode_Up;
  Timer_Initx.TIM_Period = arr;
  Timer_Initx.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM6,&Timer_Initx);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); 
	
	NVIC_Initx.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_Initx.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Initx.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Initx.NVIC_IRQChannelSubPriority = 2;
  NVIC_Init(&NVIC_Initx);
	
	TIM_Cmd(TIM6,ENABLE);
}


void TIM6_IRQHandler(void)
{
	 
	if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET)
  {
		
		  

		  TIM_ClearITPendingBit(TIM6,TIM_IT_Update );
  }  
	
}















