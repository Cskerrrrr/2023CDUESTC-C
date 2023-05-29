#include "usart3.h"
#include "usart.h"
#include "stm32f10x_usart.h"

u8     USART3SendBuff[USART3BuffSize];        
u8     USART3ReceBuff[USART3BuffSize];        
u16    USART3ReceIn = 0;
u8     USART3ReceFullFlag = 0;

int start,location;

void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO ,ENABLE);
	
	 GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3, ENABLE);
}


void USART3_SendData(u8 data)
{
	while((USART3->SR&0X40)==0); 
	USART3->DR = data;
}

void USART3_SendString(u8 *DAT,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
	 	USART3_SendData(*DAT++);
	}	
}



int luxian1_flag = 0, luxian2_flag = 0,luxian3_flag = 0,luxian4_flag = 0,luxian5_flag = 0,luxian6_flag = 0,luxian7_flag = 0,luxian8_flag = 0;



void USART3_IRQHandler(void)
{
  u8 res; // 接受的临时变量
	static u8 RxCounter=0; // 索引值
	static u16 RxBuffer[5]; // 接受缓冲区
	static u8 RxState = 0; // 接收标志位	
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)   //接收到数据了
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);   //清除中断标志
//		res =USART_ReceiveData(USART3);//(USART3->DR);	//读取接收到的数据
//	  //0x1C帧头	
//		if(RxState==0 && res==0x1C)  RxState=1;				
//    //0x2C帧头
//		else if(RxState==1 && res==0x2C)  
//		{
//				RxState=2;
//				RxCounter=0;
//		}
//		// 开始接受有效数据
//		else if(RxState==2) 
//		{  
//				RxBuffer[RxCounter++]=res;
//				if(RxCounter > 3)  //RxBuffer1接受满了,接收数据结束
//				{	
//           RxState=0;
//           if(RxBuffer[2] == 0xFF)//帧尾		
//           {						 
//             location = RxBuffer[0];
//						 start = RxBuffer[1];	 
//					 }
//				}	  	
//		}
//		//接收异常
//    else  RxState = 0;
//    		
  } 
	
}





