#ifndef __USART3_H
#define __USART3_H

#include "stm32f10x.h"
    
#define USART3BuffSize 64

extern u16    USART_RX_STA;  

extern u8     USART3SendBuff[USART3BuffSize];        
extern u8     USART3ReceBuff[USART3BuffSize]; 
extern u16    USART3ReceIn;
extern u8     USART3ReceFullFlag;


void USART3_Init(u32 bound);

void USART3_SendData(u8 data);

void USART3_SendString(u8 *DAT,u8 len);
 

#define USART_REC_LEN       200   
#define EN_USART3_RX         1      
#endif

