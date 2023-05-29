#include "led.h"



void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//����ʱ��
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//��ʼ��GPIO--PB12��PB13��PB14��PB15Ϊ�������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
}

void LED_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}


void LED_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

