/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   控制多彩流水灯
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "Usart.h"
void Delay(__IO u32 nCount); 



int main(void)
{
  /* 初始化 USART 配置模式为 115200 8-N-1，中断接收 */  
    Usart_Init();
    rx_queue_init();

    /* 发送一个字符串 */
    Usart_Send_String(DEBUG_USART,"这是一个串口接收回显实验！！！\n");
    printf("欢迎使用野火STM32开发板\n\n\n\n");

    while(1)
    {
      //获取数据并输出
      //实际应用中可参考pull data的方式获取数据进行处理
      pull_data_from_queue();
    }
}



//int main(void)
//{
//	/* LED 端口初始化 */
//	LED_GPIO_Config();

//	/* 控制LED灯 */
//	while (1)
//	{
//		LED1( ON );			 // 亮 
//		Delay(0xFFFFFF);
//		LED1( OFF );		  // 灭

//		LED2( ON );			// 亮 
//		Delay(0xFFFFFF);
//		LED2( OFF );		  // 灭

//		LED3( ON );			 // 亮 
//		Delay(0xFFFFFF);
//		LED3( OFF );		  // 灭	

//		/*轮流显示 红绿蓝黄紫青白 颜色*/
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

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

