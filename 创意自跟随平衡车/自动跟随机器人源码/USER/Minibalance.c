/************************************************
���ƣ��Զ����������
���ߣ�������
����������MDK5
��Ƭ���ͺţ�STM32
΢�Ź��ںš���������ˡ�
΢�Żظ��ؼ��֡����ӿ������ߡ���ȡ������Ʊر����
΢�Żظ��ؼ��֡�����������ϡ���ȡ����������ϰ�
΢�Żظ��ؼ��֡�PSѧϰ�̡̳�  ��ȡ����PSѧϰ�̳�
΢�Żظ��ؼ��֡�������Դ��ȡ����ȡ���ྫ����ԴŶ~

֪������������https://www.zhihu.com/people/zhang-qiao-long/activities
���˲��͡���������:http://blog.sina.com.cn/u/3829139600

��������ѧϰ���΢�ţ�best_xiaolong���߼�QQ:746175735
��ע����Ⱥ�����������Ⱥ~
************************************************/

#include "sys.h"
#include "stm32f10x.h"
#include "timer.h"
#include "encode.h"
#include "mpu6050.h"
#include "pwm.h"
#include "usart.h"	
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "motor.h"
#include "adc.h"

float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;			//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
short temp;								//�¶�	  

int Encoder_Left,Encoder_Right; //������ұ�����ֵ ������������

int Moto1,Moto2; 								//������ұ�����ֵ ������������


u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=1; //����ң����صı���


unsigned char   dat1[3],dat2[3];												//��������صı���
unsigned char   num1,num2;
int             distance1=0,distance2=0;

int Balance_Pwm,Velocity_Pwm,Turn_Pwm;									//PID����ĵ��PWM��صı���

int Voltage; 																						//adc���ѹ

u8 mode,key;

int main(void)
{  
	
	Stm32_Clock_Init(9);            //=====ϵͳʱ������
	delay_init(72);                 //=====��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	NVIC_init();										//=====��ʼ���ж����ȼ�
	LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	MPU_Init();					    				//=====��ʼ��MPU6050
	mpu_dmp_init();									//=====��ʼ��MPU6050DMPģʽ  
	OLED_Init(); 										//=====��ʼ��OLED	
	while(mpu_dmp_init())						//=====��ʼ������ ����ֵΪ0 �������������
	{
		OLED_ShowString(0,0,"MPU6050 ERROR");
		OLED_Refresh_Gram();	
		Moto1=0,Moto2=0;
	}
	OLED_ShowString(0,0,"MPU6050 OK");
	OLED_Refresh_Gram();
	TIM1_PWM_Init(7199,0);					//=====10Khz PWM
	Encoder_Init_TIM2();            //=====��ʼ��������2
  Encoder_Init_TIM4();            //=====��ʼ��������4
  KEY_Init_MY();                  //=====������ʼ��
  uart1_init(9600);            		//=====��ʼ������1
  uart2_init(115200);							//=====��ʼ������2
	uart3_init(115200);         		//=====��ʼ������3
	Motor_Init();										//=====��ʼ�����IO��
	Adc_Init();                     //=====adc��ʼ��			
	Voltage=Get_battery_volt();     //=====��ȡ��ص�ѹ	
	
	OLED_ShowString(00,40,"Volta");
	OLED_ShowString(58,40,".");
	OLED_ShowString(80,40,"V");
	OLED_ShowNumber(45,40,Voltage/100,2,12);
	OLED_ShowNumber(68,40,Voltage%100,2,12);
	if(Voltage%100<10) 	OLED_ShowNumber(62,40,0,2,12);
	
	while(1)///////////�ڸò���ȷ�������ĸ�ģʽ!
	{
		key=KEY_Scan(0);
		if(key==1)			//ģʽ1���Զ�����
		{
			mode=1;
			OLED_ShowString(0,20,"MODE:Auto Follow");	
			OLED_Refresh_Gram();			
			break;
		}else if(key==2)//ģʽ2����������
		{
			mode=2;
			OLED_ShowString(0,20,"MODE: Bluetooth");
			OLED_Refresh_Gram();
			break;
		}
		delay_ms(5);			
	}	
	Motor_Stop();    								//=====��ʼ�����IO��
	TIM3_Int_Init(99,7199); 				//=====��������10ms
	Set_Pwm(0,0);
	while(1)
	{ 		
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{
				MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			}
	} 
}

