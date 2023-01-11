/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ƶ����ˮ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "Usart.h"
void Delay(__IO u32 nCount); 



int main(void)
{
  /* ��ʼ�� USART ����ģʽΪ 115200 8-N-1���жϽ��� */  
    Usart_Init();
    rx_queue_init();

    /* ����һ���ַ��� */
    Usart_Send_String(DEBUG_USART,"����һ�����ڽ��ջ���ʵ�飡����\n");
    printf("��ӭʹ��Ұ��STM32������\n\n\n\n");

    while(1)
    {
      //��ȡ���ݲ����
      //ʵ��Ӧ���пɲο�pull data�ķ�ʽ��ȡ���ݽ��д���
      pull_data_from_queue();
    }
}



//int main(void)
//{
//	/* LED �˿ڳ�ʼ�� */
//	LED_GPIO_Config();

//	/* ����LED�� */
//	while (1)
//	{
//		LED1( ON );			 // �� 
//		Delay(0xFFFFFF);
//		LED1( OFF );		  // ��

//		LED2( ON );			// �� 
//		Delay(0xFFFFFF);
//		LED2( OFF );		  // ��

//		LED3( ON );			 // �� 
//		Delay(0xFFFFFF);
//		LED3( OFF );		  // ��	

//		/*������ʾ ������������� ��ɫ*/
//		LED_RED;
//		Delay(0xFFFFFF);
//		
//		LED_GREEN;
//		Delay(0xFFFFFF);
//		
//		LED_BLUE;
//		Delay(0xFFFFFF);
//		
//		LED_YELLOW;
//		Delay(0xFFFFFF);
//		
//		LED_PURPLE;
//		Delay(0xFFFFFF);
//				
//		LED_CYAN;
//		Delay(0xFFFFFF);
//		
//		LED_WHITE;
//		Delay(0xFFFFFF);
//		
//		LED_RGBOFF;
//		Delay(0xFFFFFF);
//	}
//}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

