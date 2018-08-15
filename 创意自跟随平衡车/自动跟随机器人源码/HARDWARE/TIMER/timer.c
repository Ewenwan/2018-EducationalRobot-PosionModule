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
#include "timer.h"
#include "encode.h" 
#include "motor.h"
#include "usart.h"	
#include "adc.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 


extern u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //����ң����صı���

u8 jishi;

extern int Encoder_Left,Encoder_Right;
extern float pitch,roll,yaw; 		//ŷ����
extern short aacx,aacy,aacz;			//���ٶȴ�����ԭʼ����
extern short gyrox,gyroy,gyroz;	//������ԭʼ����
extern short temp;								//�¶�	
extern int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
extern int distance1,distance2;
extern int Moto1,Moto2; 
extern int Voltage; 
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����							 
}

int TIM3_IRQHandler(void)   //TIM3�ж�
{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			jishi++;
			if(jishi==500)
			{
				LED=!LED;
				jishi=0;
			}
			Encoder_Left =Read_Encoder(2);   											//===��ȡ��������ֵ
			Encoder_Right=Read_Encoder(4);   											//===��ȡ��������ֵ
			Balance_Pwm =balance(roll,gyrox); 								 		//===ֱ����PID����	
			Velocity_Pwm=velocity(Encoder_Left,Encoder_Right); 		//===�ٶȻ�PID����	 ��ס���ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��
			Turn_Pwm    =turn(Encoder_Left,Encoder_Right,gyroz);  //===ת��PID����     
			Moto1=Balance_Pwm+Velocity_Pwm-Turn_Pwm;           		//===�������ֵ������PWM
			Moto2=Balance_Pwm+Velocity_Pwm+Turn_Pwm;           		//===�������ֵ������PWM		
			Xianfu_Pwm();	
			if(roll>50||roll<-50) Moto1=0,Moto2=0;
			Set_Pwm(Moto1,Moto2);																								
		}
}