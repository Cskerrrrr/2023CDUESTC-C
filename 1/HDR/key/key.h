#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 
#define LED0 PAout(8)// PB5
#define LED1 PDout(2)// PE5	


#define KEY0    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键0
#define KEY1    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define WKUP_PRES   3	//KEY_UP按下(即WK_UP/KEY_UP)


void KEY_Init(void);//IO初始化
void Mode_screen(void);
void change_mode(void);
void ok_mode(void);
u8 KEY_Scan(u8);  	//按键扫描函数						    
#endif
