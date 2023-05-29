#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

int mode = 2,mode_1 = 1,mode_2 = 1,menu = 0,OK = 0;
extern float cmA;
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_13;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE4,3

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

	
	
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}



void Mode_screen(void)
{
	if(OK == 1)//һ���˵�
	{
		 change_mode(); 
		 Gui_StrCenter(0,2,BLACK,WHITE,"��",16,1);
		 Gui_StrCenter(0,23,BLACK,WHITE,"light",16,1);
		 Gui_StrCenter(0,44,BLACK,WHITE,"��",16,1);
		 Gui_StrCenter(0,65,BLACK,WHITE,"��",16,1);
		 Gui_StrCenter(0,86,BLACK,WHITE,"��",16,1);
		 Gui_StrCenter(0,107,BLACK,WHITE,"��ʼ�����",16,1);	
	}
	else if(mode_2 == 3 && OK == 2)//�����˵�
		{
		 change_mode();
		 Gui_StrCenter(0,2,BLACK,WHITE,"1",16,1);
		 Gui_StrCenter(0,23,BLACK,WHITE,"light",16,1);
		 Gui_StrCenter(0,44,BLACK,WHITE,"3",16,1);
		 Gui_StrCenter(0,65,BLACK,WHITE,"4",16,1);
		 Gui_StrCenter(0,86,BLACK,WHITE,"5",16,1);
		 Gui_StrCenter(0,107,BLACK,WHITE,"back",16,1);
		}
	else if(mode_2 == 3 && OK == 3)//�Ӳ˵�
		{
						
			LCD_Clear(WHITE);
			Show_Str(25,60,BLACK,WHITE,"���� =   mm",16,1);
			LCD_ShowNum(80,63,cmA,2,12);

						
		}
	else if(mode_2 == 7 && OK == 3)//�Ӳ˵�
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

