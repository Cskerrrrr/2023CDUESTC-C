#include "motor.h"

void Motor_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7|GPIO_Pin_8 |GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	TIM_TimeBaseStruct.TIM_Period = arr;
	TIM_TimeBaseStruct.TIM_Prescaler =psc;
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_Pulse = 0;
	
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_Cmd(TIM4, ENABLE);
}




void Limit(int *motoA,int *motoB)
{
	if(*motoA>7199)*motoA=7199;
	if(*motoA<-7199)*motoA=-7199;
	
	if(*motoB>7199)*motoB=7199;
	if(*motoB<-7199)*motoB=-7199;
}


int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}






void SETPWM(int motor_left, int motor_right)
{
	if(motor_left<0)	   	TIM_SetCompare1(TIM4,GFP_abs(motor_left)),			TIM_SetCompare2(TIM4,0); //hou
	if(motor_left>0)      TIM_SetCompare1(TIM4,0),				TIM_SetCompare2(TIM4,GFP_abs(motor_left)); //qian
	if(motor_left==0)     TIM_SetCompare1(TIM4,7199),TIM_SetCompare2(TIM4,7199);
	
	if(motor_right<0)			TIM_SetCompare3(TIM4,0),			TIM_SetCompare4(TIM4,GFP_abs(motor_right));	//hou
  if(motor_right>0)		  TIM_SetCompare3(TIM4,GFP_abs(motor_right)),TIM_SetCompare4(TIM4,0);//qian
  if(motor_right==0)    TIM_SetCompare3(TIM4,7199),TIM_SetCompare4(TIM4,7199);
}




