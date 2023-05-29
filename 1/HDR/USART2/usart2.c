#include "usart2.h"
#include "usart.h"
#include "stm32f10x_usart.h"

int X,Y,flag;

int RXdeta(int m,int n);

void USART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);
}


void USART2_SendData(u8 data)
{
	while((USART2->SR&0X40)==0); 
	USART2->DR = data;
}

void USART2_SendString(u8 *DAT,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
	 	USART2_SendData(*DAT++);
	}	
}





void USART2_IRQHandler(void)
{
  u8 res; // 接受的临时变量
	static u8 RxCounter1=0; // 索引值
	static u16 RxBuffer1[6]; // 接受缓冲区
	static u8 RxState = 0; // 接收标志位	
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   //接收到数据了
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);   //清除中断标志
		res =USART_ReceiveData(USART2);//(USART3->DR);	//读取接收到的数据
	  //0x1C帧头	
		if(RxState==0 && res==0x1C)  RxState=1;				
    //0x2C帧头
		else if(RxState==1 && res==0x2C)  
		{
				RxState=2;
				RxCounter1=0;
		}
		// 开始接受有效数据
		else if(RxState==2) 
		{  
				RxBuffer1[RxCounter1++]=res;
				if(RxCounter1 > 4)  //RxBuffer1接受满了,接收数据结束
				{	
           RxState=0;
           if(RxBuffer1[3] == 0xFF)//帧尾		
           {						 

						 X = RxBuffer1[0]; 
						 Y = RxBuffer1[1];
						 flag = RxBuffer1[2];
					//坐标	
						 
					 }
				}	  	
		}
		//接收异常
    else  RxState = 0;
    		
  } 
	

}



