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
#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 
void usart1_send(u8 data);
void uart1_init(u32 bound);

void USART1_IRQHandler(void);

void uart2_init(u32 bound);
void uart3_init(u32 bound);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
#endif	   
















