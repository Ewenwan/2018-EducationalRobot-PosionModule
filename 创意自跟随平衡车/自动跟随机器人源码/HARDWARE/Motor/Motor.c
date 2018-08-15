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

#include "motor.h"

extern float xianshi1; 	


extern int Moto1,Moto2; 
extern int distance1,distance2;
extern float pitch,roll,yaw; 		//ŷ����
extern short aacx,aacy,aacz;			//���ٶȴ�����ԭʼ����
extern short gyrox,gyroy,gyroz;	//������ԭʼ����
extern short temp;								//�¶�	
extern u8 mode;
extern u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu; //����ң����صı���

void Motor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
}

void Motor_Stop(void)
{
		AIN1=0;
		AIN2=0;
		BIN1=0;			
		BIN2=0;
		PWMA=0;	
		PWMB=0;	
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
    	if(moto1>0)			AIN2=0,			AIN1=1;
			else 	          AIN2=1,			AIN1=0;
			PWMA=myabs(moto1);
		  if(moto2>0)	BIN1=0,			BIN2=1;
			else        BIN1=1,			BIN2=0;
			PWMB=myabs(moto2);	
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM������7200 ������6900
//		if(Flag_Qian==1)  Moto1+=DIFFERENCE;  //DIFFERENCE��һ������ƽ��С������ͻ�е��װ�����һ��������ֱ���������������С�����и��õ�һ���ԡ�
//	  if(Flag_Hou==1)   Moto2-=DIFFERENCE;
    if(Moto1<-Amplitude) Moto1=-Amplitude;	
		if(Moto1>Amplitude)  Moto1=Amplitude;	
	  if(Moto2<-Amplitude) Moto2=-Amplitude;	
		if(Moto2>Amplitude)  Moto2=Amplitude;		
	
}

int balance(float Angle,float Gyro)
{  
   float Bias,kp=300,kd=1;
	 int balance;
	 Bias=Angle;       //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 balance=kp*Bias+Gyro*kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}
/**************************************************************************
�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
��ڲ��������ֱ����������ֱ�����
����  ֵ���ٶȿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
		static float Velocity,Encoder_Least,Encoder,Movement;
		static float Encoder_Integral,Target_Velocity;
		float kp=80,ki=0.4;
	
		float line_distance1,line_distance2;//�������ľ���
	
		float cha;
	
		float set_distance=600;//����С����600mm֮��
	
		line_distance1=distance1-set_distance;
		line_distance2=distance2-set_distance;
		//=============�Զ����沿��=======================//
		if(mode==1)	
		{
				if(cha==0) 	
				{
					if((line_distance1>0&&line_distance2>0)||(line_distance1<0&&line_distance2<0))
					{
						Movement=0.2*((line_distance1+line_distance2)/2);
					}		
				}
				if(distance1>2000||distance2>2000) Movement=0;
		}
		//=============ң��ǰ�����˲���=======================//
		if(mode==2)
		{	
			if(1==Flag_Qian)	Movement=60;	             //===���ǰ����־λ��1 λ��Ϊ��
			else if(1==Flag_Hou)	  Movement=-60;        //===������˱�־λ��1 λ��Ϊ��
			else  Movement=0;	
		}
   //=============�ٶ�PI������=======================//	
		Encoder_Least =(encoder_left+encoder_right)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===�����޷�	
		Velocity=Encoder*kp+Encoder_Integral*ki;                          //===�ٶȿ���	
		if(roll>50||roll<-50) 	Encoder_Integral=0;												//===�������	
	  return Velocity;
}


int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
	  static float Turn_Target,Encoder_temp,Turn_Convert=0.9,Turn_Count,Turn;
		float Turn_Amplitude=88/Flag_sudu,Kp=42,Kd=0;         
	  float Kp1=40,Kd1=0,ccc;  
	  float cha1;
		//=============�Զ����沿��=======================//
		if(mode==1)
		{
			cha1=distance1-distance2;
			if(distance1>20&&distance1<2000&&distance2>20&&distance2<2000&&cha1<0) 	
			{
				ccc=1*cha1;	
			}
			else if(distance1>20&&distance1<2000&&distance2>20&&distance2<2000&&cha1>0) 	
			{	
				ccc=1*cha1;
			}
			else 
			{
				ccc=0;
			}		
			Turn_Target=ccc;
			Turn=-Turn_Target*Kp1;                 //===���Z�������ǽ���PD����
			return Turn;
		}
		//=============�������Ʋ���=======================//
		if(mode==2)
		{
			if(1==Flag_Left||1==Flag_Right)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
			{
				if(++Turn_Count==1)
				Encoder_temp=myabs(encoder_left+encoder_right);
				Turn_Convert=50/Encoder_temp;
				if(Turn_Convert<0.6)Turn_Convert=0.6;
				if(Turn_Convert>3)Turn_Convert=3;
			}	
			else
			{
				Turn_Convert=0;
				Turn_Count=0;
				Encoder_temp=0;
			}			
			if(1==Flag_Left)	           Turn_Target-=Turn_Convert;
			else if(1==Flag_Right)	     Turn_Target+=Turn_Convert; 
			else Turn_Target=0;
		
			if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת���ٶ��޷�
			if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
			if(Flag_Qian==1||Flag_Hou==1)  Kd=1;        
			else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
			//=============ת��PD������=======================//
			Turn=-Turn_Target*Kp-0*gyro*Kd;                 	//===���Z�������ǽ���PD����
			return Turn;
	}
}

//int turn(int encoder_left,int encoder_right,float gyro)//ת�����
//{
//	  static float Turn_Target,Turn;
//	  float Kp=40,Kd=0,ccc;  
//	  float cha1;
//		cha1=distance1-distance2;
//		if(distance1>20&&distance1<1000&&distance2>20&&distance2<1000&&cha1<0) 	
//		{
//			ccc=1*cha1;	
//		}
//		else if(distance1>20&&distance1<1000&&distance2>20&&distance2<1000&&cha1>0) 	
//		{	
//			ccc=1*cha1;
//		}
//		else 
//		{
//			ccc=0;
//		}		
//		Turn_Target=ccc;
//		Turn=-Turn_Target*Kp;                 //===���Z�������ǽ���PD����
//	  return Turn;
//}
