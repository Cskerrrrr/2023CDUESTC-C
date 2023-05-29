#include "pid.h"
#include "xunji.h"
#include "usart2.h"
#include "usart.h"


float Velocity_Kp = 80,		//�ٶȻ�Kp
	  Velocity_Ki = 2;			//�ٶȻ�Kd

float Turn_Kp = 85,     //ת��
	  Turn_Kd = 3;  

int Vertical_Kp = 1, //jiaodu��
	  Vertical_Kd = 10;

float Kp_value_zuo = 40,//����ͷѲ��ת��
      Kp_value_you = -40;






int Vertical_out,Velocity_out,Turn_out;		//�������
int measure;//�������ٶȲ���ֵ					
int motor1, motor2;

int turn = 0,turn_left90 = 0 , turn_right90 = 0, turn_left = 0 , turn_right = 0  , turn_back = 0, stop = 1, run = 1;


int go = 0;

int Velocity_PI(int Speed_measure);
int Turn(int gyro_Z);
int Vertical_PI(int gyro_Z);



void EXTI9_5_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	{
			if(PBin(5) == 0)
			{
							if(USART_ReceiveData(USART3) == 0x01)
							{
								luxian1();
							}
							else if(USART_ReceiveData(USART3) == 0x02)
							{
								luxian2();
							}
							else if(USART_ReceiveData(USART3) == 0x03)
							{
								luxian3();
							}
							else if(USART_ReceiveData(USART3) == 0x04)
							{
								luxian4();
							}
							else if(USART_ReceiveData(USART3) == 0x05)
							{
								luxian5();
							}
							else if(USART_ReceiveData(USART3) == 0x06)
							{
								luxian6();
							}
							else if(USART_ReceiveData(USART3) == 0x07)
							{
								luxian7();
							}
							else if(USART_ReceiveData(USART3) == 0x08)
							{
								luxian8();
							}
//				    luxian1();
						Encoder_Left = Read_Spead(2);
						Encoder_Right = -Read_Spead(3);
						measure = (Encoder_Left + Encoder_Right);
						mpu_dmp_get_data(&Pitch, &Roll, &Yaw);
						MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
						Velocity_out = Velocity_PI(measure);
						Turn_out = Turn(gyroz);
						Vertical_out = Vertical_PI(gyroz);
				    if(turn_left90 == 1 || turn_right90 == 1 || turn_back == 1 || go == 1)
						{
								motor1 = Velocity_out + Turn_out - Vertical_out;
								motor2 = Velocity_out - Turn_out + Vertical_out;
						}
						else
						{
								motor1 = Velocity_out + Turn_out - Vertical_out + Kp_value_zuo*(X-112);
								motor2 = Velocity_out - Turn_out + Vertical_out + Kp_value_you*(X-112);
						}
						Limit(&motor1, &motor2);
						
						SETPWM(motor1,motor2);
						//printf("%f,%f,%f \r\n",Pitch,Roll,Yaw);
						EXTI_ClearITPendingBit(EXTI_Line5);
			}
             			
		}
		 
	
}


int Vertical_PI(int gyro_Z)//�Ƕ�
{
		static int Encoder_err, Encoder_sum;
		static int PWM_out;
		float Turn_Target=0,Vertical_Kp,Vertical_Kd;
		
//		if(GFP_abs(gyro_Z)<150)
//		{
//			gyro_Z = 0;
//		}
		if(turn_right == 1 && turn == 1)  
		{
					Turn_Target = 100,Vertical_Kp = -20,Vertical_Kd = 10;
					Encoder_err = gyro_Z-0;
					Encoder_sum += Encoder_err;
					if(GFP_abs(Encoder_sum) > 62500)//32500
					{
						Vertical_Kp = 0;
						Vertical_Kd = 0;
						Encoder_err = 0;
						Encoder_sum = 0;
						turn_left90 = 0;
						turn_right90 = 0;
						turn_back = 0;
						turn_left = 0;
						turn_right = 0;
						turn = 0;
					}
		}
		if(turn_left == 1 && turn == 1)  
		{
						Turn_Target = -100,Vertical_Kp = -20,Vertical_Kd = 10;
						Encoder_err = gyro_Z-0;
						Encoder_sum += Encoder_err;
						if(GFP_abs(Encoder_sum) > 62500)//125000
						{
							Vertical_Kp = 0;
							Vertical_Kd = 0;
							Encoder_err = 0;
							Encoder_sum = 0;
							turn_left90 = 0;
							turn_right90 = 0;
							turn_back = 0;
							turn_left = 0;
							turn_right = 0;
							turn = 0;
						}
		}
		if(turn_right90 == 1 && turn == 1)  
		{
					Turn_Target = 100,Vertical_Kp = -20,Vertical_Kd = 10;
					Encoder_err = gyro_Z-0;
					Encoder_sum += Encoder_err;
					if(GFP_abs(Encoder_sum) > 129000)//32500
					{
						Vertical_Kp = 0;
						Vertical_Kd = 0;
						Encoder_err = 0;
						Encoder_sum = 0;
						turn_left90 = 0;
						turn_right90 = 0;
						turn_back = 0;
						turn_left = 0;
						turn_right = 0;
						turn = 0;
					}
		}
		if(turn_left90 == 1 && turn == 1)  
		{
						Turn_Target = -100,Vertical_Kp = -20,Vertical_Kd = 10;
						Encoder_err = gyro_Z-0;
						Encoder_sum += Encoder_err;
						if(GFP_abs(Encoder_sum) > 129000)//125000
						{
							Vertical_Kp = 0;
							Vertical_Kd = 0;
							Encoder_err = 0;
							Encoder_sum = 0;
							turn_left90 = 0;
							turn_right90 = 0;
							turn_back = 0;
							turn_left = 0;
							turn_right = 0;
							turn = 0;
						}
		}
		if(turn_back == 1 && turn == 1)  
		{
						Turn_Target = 100,Vertical_Kp = -20,Vertical_Kd = 10;
						Encoder_err = gyro_Z-0;
						Encoder_sum += Encoder_err;
						if(GFP_abs(Encoder_sum) > 258000)
						{
							Vertical_Kp = 0;
							Vertical_Kd = 0;
							Encoder_err = 0;
							Encoder_sum = 0;
							turn_left90 = 0;
							turn_right90 = 0;
							turn_back = 0;
							turn_left = 0;
							turn_right = 0;
							turn = 0;
						}
		}
		
		
		PWM_out = Turn_Target*Vertical_Kp + gyro_Z*Vertical_Kd; 
		
		return PWM_out;					

}




int Velocity_PI(int Speed_measure)//�ٶ�
{
		static int Encoder_err, Encoder_err_low, Encoder_err_low_last, Encoder_sum;
		static int PWM_out;
		static float Target_Velocity = 8;
		
		
    if(stop || turn_left || turn_right|| turn_left90 || turn_right90) Target_Velocity = 0,Velocity_Ki = 0,Velocity_Kp = 0;
		else                                    Target_Velocity = 8,Velocity_Ki = 2,Velocity_Kp = 0;
	
		if(turn_right90)
		{
			Target_Velocity = 6,Velocity_Ki = 2;
			turn = 1;				
		}
		if(turn_left90)
		{
			Target_Velocity = 6,Velocity_Ki = 2;
			turn = 1;
		}
		if(turn_right)
		{
			Target_Velocity = 6,Velocity_Ki = 2;
			turn = 1;				
		}
		if(turn_left)
		{
			Target_Velocity = 6,Velocity_Ki = 2;
			turn = 1;
		}
		if(turn_back)
		{
			Target_Velocity = 6,Velocity_Ki = 2;
			turn = 1;
		}
		
		Encoder_err = Target_Velocity - Speed_measure;
		Encoder_err_low = 0.3 * Encoder_err + 0.7 * Encoder_err_low_last;
		Encoder_err_low_last = Encoder_err_low;
		Encoder_sum += Encoder_err_low;
		if(Encoder_sum > 10000) Encoder_sum = 10000;
		if(Encoder_sum < -10000) Encoder_sum = -10000;
		
		PWM_out = Velocity_Kp*Encoder_err + Velocity_Ki*Encoder_sum;
			
		return PWM_out;
}

//ת��

int Turn(int gyro_Z)
{
		int PWM_out;
		float Turn_Target=0;
		float Kp = Turn_Kp,Kd = Turn_Kd;			
		
		if(run == 1)  Kd = Turn_Kd;  
		
		else Kd = 0;
		
		
		PWM_out = Turn_Target*Kp + gyro_Z*Kd; 
		
		return PWM_out;
}









int blue_flag = 0,white_flag = 0;


/*************************************·��1( DCB / CBA / AGE / GED )*********************************************/

uint8_t luxian1_flag1 = 0 , luxian1_flag2 = 0 , luxian1_flag3 = 0 , luxian1_flag4 = 0;
uint8_t luxian1_suo1 = 0 , luxian1_suo2 = 0 , luxian1_suo3 = 0 , luxian1_suo4 = 0, luxian1_suo5 = 0;
int luxian1_distance_1 = 0 , luxian1_distancejifen_1 = 1200,
	  luxian1_distance_2 = 0 , luxian1_distancejifen_2 = 1500;

void luxian1(void)
{	
	 
		if(luxian1_suo1==0)//����
		{
			stop = 0;
			turn_back = 1;
			luxian1_suo1 = 1;
			luxian1_flag1 = 1;
		}
		if(flag == 243&& luxian1_flag1 == 1)//��⵽��ɫ
		{
			if(luxian1_suo2==0)//����
			{
				go = 1;
				luxian1_suo2 = 1;	
				luxian1_flag2 = 1;
			}						
		}
    if(luxian1_flag2 == 1)
		{
			luxian1_flag1 = 0;
			luxian1_distance_1 = luxian1_distance_1 + measure/2;
		} 
		if(luxian1_distance_1 >= luxian1_distancejifen_1)//�Ƿ񵽴��ɫ
		{
			luxian1_flag2 = 0;
			if(luxian1_suo3==0)//����
			{	
        turn_left = 1;				
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian1_suo3 = 1;
        luxian1_flag3 = 1;				
			}						
		}
		if(flag == 242&& luxian1_flag3 == 1)//��⵽��ɫ
		{
			if(luxian1_suo4==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian1_suo4 = 1;	
				luxian1_flag4 = 1;
			}						
		}
    if(luxian1_flag4 == 1)
		{
			luxian1_flag3 = 0;
			luxian1_distance_2 = luxian1_distance_2 + measure/2;
		} 
		if(luxian1_distance_2 >= luxian1_distancejifen_2)//�Ƿ񵽴���ɫ
		{
			luxian1_flag4 = 0;
			if(luxian1_suo5==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian1_suo5 = 1;	
			}						
		}
}


/*************************************·��2( DEG / EGA / ABC / BCD )*********************************************/

uint8_t luxian2_flag1 = 0 , luxian2_flag2 = 0 , luxian2_flag3 = 0 , luxian2_flag4 = 0;
uint8_t luxian2_suo1 = 0 , luxian2_suo2 = 0 , luxian2_suo3 = 0 , luxian2_suo4 = 0, luxian2_suo5 = 0;
int luxian2_distance_1 = 0 , luxian2_distancejifen_1 = 1200,
	  luxian2_distance_2 = 0 , luxian2_distancejifen_2 = 1500;

void luxian2(void)
{			
		if(luxian2_suo1==0)//����
		{
			turn_back = 1;
			luxian2_suo1 = 1;
			luxian2_flag1 = 1;
		}
		if(flag == 243&& luxian2_flag1 == 1)//��⵽��ɫ
		{
			if(luxian2_suo2==0)//����
			{
				go = 1;
				luxian2_suo2 = 1;	
				luxian2_flag2 = 1;
			}						
		}
    if(luxian2_flag2 == 1)
		{
			luxian2_flag1 = 0;
			luxian2_distance_1 = luxian2_distance_1 + measure/2;
		} 
		if(luxian2_distance_1 >= luxian2_distancejifen_1)//�Ƿ񵽴��ɫ
		{
			luxian2_flag2 = 0;
			if(luxian2_suo3==0)//����
			{
				
				turn_right = 1;
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian2_suo3 = 1;
        luxian2_flag3 = 1;				
			}						
		}
		if(flag == 242&& luxian2_flag3 == 1)//��⵽��ɫ
		{
			if(luxian2_suo4==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian2_suo4 = 1;	
				luxian2_flag4 = 1;
			}						
		}
    if(luxian2_flag4 == 1)
		{
			luxian2_flag3 = 0;
			luxian2_distance_2 = luxian2_distance_2 + measure/2;
		} 
		if(luxian2_distance_2 >= luxian2_distancejifen_2)//�Ƿ񵽴���ɫ
		{
			luxian2_flag4 = 0;
			if(luxian2_suo5==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian2_suo5 = 1;	
			}						
		}
}



/*************************************·��3( DCF / BAG )*********************************************/

uint8_t luxian3_flag1 = 0 , luxian3_flag2 = 0 , luxian3_flag3 = 0 , luxian3_flag4 = 0;
uint8_t luxian3_suo1 = 0 , luxian3_suo2 = 0 , luxian3_suo3 = 0 , luxian3_suo4 = 0, luxian3_suo5 = 0;
int luxian3_distance_1 = 0 , luxian3_distancejifen_1 = 1200,
	  luxian3_distance_2 = 0 , luxian3_distancejifen_2 = 1500;

void luxian3(void)
{		
		if(luxian3_suo1==0)//����
		{
			turn_back = 1;
			luxian3_suo1 = 1;
			luxian3_flag1 = 1;
		}
		if(flag == 243&& luxian3_flag1 == 1)//��⵽��ɫ
		{
			if(luxian3_suo2==0)//����
			{
				go = 1;
				luxian3_suo2 = 1;	
				luxian3_flag2 = 1;
			}						
		}
    if(luxian3_flag2 == 1)
		{
			luxian3_flag1 = 0;
			luxian3_distance_1 = luxian3_distance_1 + measure/2;
		} 
		if(luxian3_distance_1 >= luxian3_distancejifen_1)//�Ƿ񵽴��ɫ
		{
			luxian3_flag2 = 0;
			if(luxian3_suo3==0)//����
			{
				
				turn_left90 = 1;
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian3_suo3 = 1;
        luxian3_flag3 = 1;				
			}						
		}
		if(flag == 242&& luxian3_flag3 == 1)//��⵽��ɫ
		{
			if(luxian3_suo4==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian3_suo4 = 1;	
				luxian3_flag4 = 1;
			}						
		}
    if(luxian3_flag4 == 1)
		{
			luxian3_flag3 = 0;
			luxian3_distance_2 = luxian3_distance_2 + measure/2;
		} 
		if(luxian3_distance_2 >= luxian3_distancejifen_2)//�Ƿ񵽴���ɫ
		{
			luxian3_flag4 = 0;
			if(luxian3_suo5==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian3_suo5 = 1;	
			}						
		}
}


/*************************************·��4( FCD / GAB )*********************************************/

uint8_t luxian4_flag1 = 0 , luxian4_flag2 = 0 , luxian4_flag3 = 0 , luxian4_flag4 = 0;
uint8_t luxian4_suo1 = 0 , luxian4_suo2 = 0 , luxian4_suo3 = 0 , luxian4_suo4 = 0, luxian4_suo5 = 0;
int luxian4_distance_1 = 0 , luxian4_distancejifen_1 = 1200,
	  luxian4_distance_2 = 0 , luxian4_distancejifen_2 = 1500;

void luxian4(void)
{			
		if(luxian4_suo1==0)//����
		{
			stop = 0;
			turn_back = 1;
			luxian4_suo1 = 1;
			luxian4_flag1 = 1;
		}
		if(flag == 243&& luxian4_flag1 == 1)//��⵽��ɫ
		{
			if(luxian4_suo2==0)//����
			{
				go = 1;
				luxian4_suo2 = 1;	
				luxian4_flag2 = 1;
			}						
		}
    if(luxian4_flag2 == 1)
		{
			luxian4_flag1 = 0;
			luxian4_distance_1 = luxian4_distance_1 + measure/2;
		} 
		if(luxian4_distance_1 >= luxian4_distancejifen_1)//�Ƿ񵽴��ɫ
		{
			luxian4_flag2 = 0;
			if(luxian4_suo3==0)//����
			{
				
				turn_right90 = 1;
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian4_suo3 = 1;
        luxian4_flag3 = 1;				
			}						
		}
		if(flag == 242&& luxian4_flag3 == 1)//��⵽��ɫ
		{
			if(luxian4_suo4==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian4_suo4 = 1;	
				luxian4_flag4 = 1;
			}						
		}
    if(luxian4_flag4 == 1)
		{
			luxian4_flag3 = 0;
			luxian4_distance_2 = luxian4_distance_2 + measure/2;
		} 
		if(luxian4_distance_2 >= luxian4_distancejifen_2)//�Ƿ񵽴���ɫ
		{
			luxian4_flag4 = 0;
			if(luxian4_suo5==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian4_suo5 = 1;	
			}						
		}
}


/*****************************************·��5( BFE / CFG / GFC / EFB )************************/

uint8_t luxian5_flag1 = 0 , luxian5_flag2 = 0;
uint8_t luxian5_suo1 = 0 , luxian5_suo2 = 0 , luxian5_suo3 = 0;
int luxian5_distance_1 = 0 , luxian5_distancejifen_1 = 1500;

void luxian5(void)
{			
		if(luxian1_suo1==0)//����
		{
			turn_back = 1;
			luxian1_suo1 = 1;
			luxian1_flag1 = 1;
		}
		if(flag == 243&& luxian1_flag1 == 1)//��⵽��ɫ
		{
			if(luxian1_suo2==0)//����
			{
				go = 1;
				luxian1_suo2 = 1;	
				luxian1_flag2 = 1;
			}						
		}
    if(luxian1_flag2 == 1)
		{
			luxian1_flag1 = 0;
			luxian1_distance_1 = luxian1_distance_1 + measure/2;
		} 
		if(luxian1_distance_1 >= luxian1_distancejifen_1)//�Ƿ񵽴��ɫ
		{
			luxian1_flag2 = 0;
			if(luxian1_suo3==0)//����
			{
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian1_suo3 = 1;
        luxian1_flag3 = 1;				
			}						
		}
		if(flag == 242&& luxian1_flag3 == 1)//��⵽��ɫ
		{
			if(luxian1_suo4==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian1_suo4 = 1;	
				luxian1_flag4 = 1;
			}						
		}
    if(luxian1_flag4 == 1)
		{
			luxian1_flag3 = 0;
			luxian1_distance_2 = luxian1_distance_2 + measure/2;
		} 
		if(luxian1_distance_2 >= luxian1_distancejifen_2)//�Ƿ񵽴���ɫ
		{
			luxian1_flag4 = 0;
			if(luxian1_suo5==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian1_suo5 = 1;	
			}						
		}
}



/*************************************·��6( ABCD )*********************************************/

uint8_t luxian6_flag1 = 0 , luxian6_flag2 = 0 , luxian6_flag3 = 0 , luxian6_flag4 = 0, luxian6_flag5 = 0, luxian6_flag6 = 0;
uint8_t luxian6_suo1 = 0 , luxian6_suo2 = 0 , luxian6_suo3 = 0 , luxian6_suo4 = 0, luxian6_suo5 = 0, luxian6_suo6 = 0, luxian6_suo7 = 0;
int luxian6_distance_1 = 0 , luxian6_distancejifen_1 = 1200,
	  luxian6_distance_2 = 0 , luxian6_distancejifen_2 = 1200,
	  luxian6_distance_3 = 0 , luxian6_distancejifen_3 = 1500;

void luxian6(void)
{	
		if(luxian6_suo1==0)//����
		{
			turn_back = 1;
			luxian6_suo1 = 1;
			luxian6_flag1 = 1;
		}
		if(flag == 243&& luxian6_flag1 == 1)//��⵽��ɫ
		{
			if(luxian6_suo2==0)//����
			{
				go = 1;
				luxian6_suo2 = 1;	
				luxian6_flag2 = 1;
			}						
		}
    if(luxian6_flag2 == 1)
		{
			luxian6_flag1 = 0;
			luxian6_distance_1 = luxian6_distance_1 + measure/2;
		} 
		if(luxian6_distance_1 >= luxian6_distancejifen_1)//�Ƿ񵽴��ɫ
		{
			luxian6_flag2 = 0;
			if(luxian6_suo3==0)//����
			{
				
				turn_right = 1;
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian6_suo3 = 1;
        luxian6_flag3 = 1;				
			}						
		}
		if(flag == 243&& luxian6_flag3 == 1)//�ٴμ�⵽��ɫ
		{
			if(luxian6_suo4==0)//����
			{
				go = 1;
				luxian6_suo4 = 1;	
				luxian6_flag4 = 1;
			}						
		}
    if(luxian6_flag4 == 1)
		{
			luxian6_flag3 = 0;
			luxian6_distance_2 = luxian6_distance_2 + measure/2;
		} 
		if(luxian6_distance_2 >= luxian6_distancejifen_2)//�Ƿ��ٴε����ɫ
		{
			luxian6_flag4 = 0;
			if(luxian6_suo5==0)//����
			{
				
				turn_right = 1;
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian6_suo5 = 1;
        luxian6_flag5 = 1;				
			}						
		}
		if(flag == 242&& luxian6_flag5 == 1)//��⵽��ɫ
		{
			if(luxian6_suo6==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian6_suo6 = 1;	
				luxian6_flag6 = 1;
			}						
		}
    if(luxian6_flag6 == 1)
		{
			luxian6_flag5 = 0;
			luxian6_distance_3 = luxian6_distance_3 + measure/2;
		} 
		if(luxian6_distance_3 >= luxian6_distancejifen_3)//�Ƿ񵽴���ɫ
		{
			luxian6_flag6 = 0;
			if(luxian6_suo7==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian6_suo7 = 1;	
			}						
		}	
}

/*************************************·��7( DCBA )*********************************************/

uint8_t luxian7_flag1 = 0 , luxian7_flag2 = 0 , luxian7_flag3 = 0 , luxian7_flag4 = 0, luxian7_flag5 = 0, luxian7_flag6 = 0;
uint8_t luxian7_suo1 = 0 , luxian7_suo2 = 0 , luxian7_suo3 = 0 , luxian7_suo4 = 0, luxian7_suo5 = 0, luxian7_suo6 = 0, luxian7_suo7 = 0;
int luxian7_distance_1 = 0 , luxian7_distancejifen_1 = 1200,
	  luxian7_distance_2 = 0 , luxian7_distancejifen_2 = 1200,
	  luxian7_distance_3 = 0 , luxian7_distancejifen_3 = 1500;

void luxian7(void)
{			
		if(luxian7_suo1==0)//����
		{
			turn_back = 1;
			luxian7_suo1 = 1;
			luxian7_flag1 = 1;
		}
		if(flag == 243&& luxian7_flag1 == 1)//��⵽��ɫ
		{
			if(luxian7_suo2==0)//����
			{
				go = 1;
				luxian7_suo2 = 1;	
				luxian7_flag2 = 1;
			}						
		}
    if(luxian7_flag2 == 1)
		{
			luxian7_flag1 = 0;
			luxian7_distance_1 = luxian7_distance_1 + measure/2;
		} 
		if(luxian7_distance_1 >= luxian7_distancejifen_1)//�Ƿ񵽴��ɫ
		{
			luxian7_flag2 = 0;
			if(luxian7_suo3==0)//����
			{
				
				turn_left = 1;
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian7_suo3 = 1;
        luxian7_flag3 = 1;				
			}						
		}
		if(flag == 243&& luxian7_flag3 == 1)//�ٴμ�⵽��ɫ
		{
			if(luxian7_suo4==0)//����
			{
				go = 1;
				luxian7_suo4 = 1;	
				luxian7_flag4 = 1;
			}						
		}
    if(luxian7_flag4 == 1)
		{
			luxian7_flag3 = 0;
			luxian7_distance_2 = luxian7_distance_2 + measure/2;
		} 
		if(luxian7_distance_2 >= luxian7_distancejifen_2)//�Ƿ��ٴε����ɫ
		{
			luxian7_flag4 = 0;
			if(luxian7_suo5==0)//����
			{
				
				turn_left = 1;
				stop = 1;//����
				SETPWM(0,0);
				delay_ms(5000);
				stop = 0;//����
				go = 0;//����Ѳ��
				luxian7_suo5 = 1;
        luxian7_flag5 = 1;				
			}						
		}
		if(flag == 242&& luxian7_flag5 == 1)//��⵽��ɫ
		{
			if(luxian7_suo6==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian7_suo6 = 1;	
				luxian7_flag6 = 1;
			}						
		}
    if(luxian7_flag6 == 1)
		{
			luxian7_flag5 = 0;
			luxian7_distance_3 = luxian7_distance_3 + measure/2;
		} 
		if(luxian7_distance_3 >= luxian7_distancejifen_3)//�Ƿ񵽴���ɫ
		{
			luxian7_flag6 = 0;
			if(luxian7_suo7==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian7_suo7 = 1;	
			}						
		}	
}

/************·��8( AF / AG / BA / BC / BF / CB / CD / CE / CF / DC / DE / EC / ED / EF / EG / FA / FB / FC / FE / FG / GA / GE / GF )************************/

uint8_t luxian8_flag1 = 0 , luxian8_flag2 = 0;
uint8_t luxian8_suo1 = 0 , luxian8_suo2 = 0 , luxian8_suo3 = 0;
int luxian8_distance_1 = 0 , luxian8_distancejifen_1 = 1500;

void luxian8(void)
{			
		if(luxian8_suo1==0)//����
		{
			turn_back = 1;
			luxian8_suo1 = 1;
			luxian8_flag1 = 1;
		}
		if(flag == 242&& luxian8_flag1 == 1)//��⵽��ɫ
		{
			if(luxian8_suo2==0)//����
			{
				go = 1;//�ر�Ѳ��
				luxian8_suo2 = 1;	
				luxian8_flag2 = 1;
			}						
		}
    if(luxian8_flag2 == 1)
		{
			luxian8_flag1 = 0;
			luxian8_distance_1 = luxian8_distance_1 + measure/2;
		} 
		if(luxian8_distance_1 >= luxian8_distancejifen_1)//�Ƿ񵽴���ɫ
		{
			luxian8_flag2 = 0;
			if(luxian8_suo3==0)//����
			{
				go = 0;
				stop = 1;//����
				luxian8_suo3 = 1;	
			}						
		}
}





