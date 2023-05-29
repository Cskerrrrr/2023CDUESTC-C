#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

int mode = 2,mode_1 = 1,mode_2 = 1,menu = 0,OK = 0;
extern float cmA;
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_13;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE4,3

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

	
	
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
}



void Mode_screen(void)
{
	if(OK == 1)//一级菜单
	{
		 change_mode(); 
		 Gui_StrCenter(0,2,BLACK,WHITE,"初",16,1);
		 Gui_StrCenter(0,23,BLACK,WHITE,"light",16,1);
		 Gui_StrCenter(0,44,BLACK,WHITE,"化",16,1);
		 Gui_StrCenter(0,65,BLACK,WHITE,"完",16,1);
		 Gui_StrCenter(0,86,BLACK,WHITE,"成",16,1);
		 Gui_StrCenter(0,107,BLACK,WHITE,"初始化完成",16,1);	
	}
	else if(mode_2 == 3 && OK == 2)//二级菜单
		{
		 change_mode();
		 Gui_StrCenter(0,2,BLACK,WHITE,"1",16,1);
		 Gui_StrCenter(0,23,BLACK,WHITE,"light",16,1);
		 Gui_StrCenter(0,44,BLACK,WHITE,"3",16,1);
		 Gui_StrCenter(0,65,BLACK,WHITE,"4",16,1);
		 Gui_StrCenter(0,86,BLACK,WHITE,"5",16,1);
		 Gui_StrCenter(0,107,BLACK,WHITE,"back",16,1);
		}
	else if(mode_2 == 3 && OK == 3)//子菜单
		{
						
			LCD_Clear(WHITE);
			Show_Str(25,60,BLACK,WHITE,"距离 =   mm",16,1);
			LCD_ShowNum(80,63,cmA,2,12);

						
		}
	else if(mode_2 == 7 && OK == 3)//子菜单
		{
    		 OK = 1;
		}
//	else
//    {
//			   OK = 1;
//		}
  ok_mode();
}

void change_mode(void)
{
		if(KEY_Scan(1) == KEY0_PRES)
		{
		 mode++;
		 mode_1 = mode;
		 LCD_Clear(BLUE);
		 LCD_Fill(0,(mode-2)*21,224,(mode-1)*21,WHITE);
		 if(mode > 6)
		 {
			  mode = 1;
		 }
//		 printf("%d \r\n",mode);
		}
}

void ok_mode(void)
{
		if(KEY_Scan(1) == KEY1_PRES)
		{
     OK++;
		 mode = 2;
		 mode_2 = mode_1;
		 LCD_Clear(BLUE);
		 LCD_Fill(0,0,224,21,WHITE);
		}
}

