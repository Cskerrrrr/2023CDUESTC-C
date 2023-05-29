#include "sys.h" 

float Target_Speed;						//期望速度
short gyrox,gyroy,gyroz;					//角速度
float Pitch,Roll,Yaw;						//角度
int Encoder_Left,Encoder_Right;				//编码器速度
float cmA = 0;



extern int turn_left90, turn_right90, turn,turn_back,run;




int main(void)
{ 		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//w4级抢占,4级响应。
	uart1_init(115200);
	USART2_Init(115200);
	USART3_Init(115200);
	delay_init();
	NVIC_Config();
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	Motor_Init(7199,0);
//	TIM6_init(49999,71);
	MPU_Init();
	while(mpu_dmp_init())
	{
		
	}
  MPU6050_EXTI_Init();
	LED_Init();
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	while(1)
	{
		  
//		SETPWM(7199, 7199);
	
//		measure = (Encoder_Left + Encoder_Right);
//		printf("%d %d\r\n",Encoder_Left,Encoder_Right);	
//    control(1);
//		  SETPWM(-4000,4000);
//		printf("%d %d %d\r\n",X,Y,flag);
		  

	}
		
}
		
		
		


