#include "stm32f10x_lib.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"

#include "delay_5883.h"
#include "IIC_5883.h"
#include "signal_5883.h"

#define	HMC5883L_Addr   0x3C

int main(void)
{ 
  Stm32_Clock_Init(9);//ϵͳʱ������
  delay_init(72);
  uart_init(72,115200); //����1��ʼ��  
  I2C_GPIO_Config();
  Single_Write(HMC5883L_Addr,0x00,0x14);//��ʼ��HMC5883L��   
  Single_Write(HMC5883L_Addr,0x02,0x00);    
  while(1)
  {
	read_hmc5883l();
  }
}

 





















