#ifndef __ENCODE_H
#define __ENCODE_H
#include <sys.h>	 
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
#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);
#endif

