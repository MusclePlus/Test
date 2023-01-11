#include "Usart.h"




void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* Ƕ�������жϿ�������ѡ�� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitStruct.NVIC_IRQChannel =  DEBUG_USART_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStruct);
}


void Usart_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    USART_InitTypeDef USART_InstStruct;
    
    RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK,ENABLE);//����GPIOʱ�Ӻʹ���ʱ��
    RCC_APB2PeriphClockCmd(USARTx_CLK,ENABLE);
    
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStruct.GPIO_Pin = USARTx_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(USARTx_TX_GPIO_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = USARTx_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(USARTx_RX_GPIO_PORT,&GPIO_InitStruct);

    GPIO_PinAFConfig(USARTx_TX_GPIO_PORT,USARTx_TX_SOURCE,USARTx_TX_AF);//��GPIO�����������õĴ���
    GPIO_PinAFConfig(USARTx_RX_GPIO_PORT,USARTx_RX_SOURCE,USARTx_RX_AF);
    
    USART_InstStruct.USART_BaudRate = USARTx_BAUDRATE;
    USART_InstStruct.USART_WordLength = USART_WordLength_8b;
    USART_InstStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InstStruct.USART_Parity = USART_Parity_No;
    USART_InstStruct.USART_StopBits = USART_StopBits_1;
    USART_InstStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    USART_Init(DEBUG_USART,&USART_InstStruct);
    
    NVIC_Configuration();
    
    USART_ITConfig(DEBUG_USART,USART_IT_RXNE,ENABLE);
    USART_ITConfig ( DEBUG_USART, USART_IT_IDLE, ENABLE ); //ʹ�ܴ������߿����ж� 	

    USART_Cmd(DEBUG_USART,ENABLE);
}

//����һ��byte
void Usart_Send_Byte(USART_TypeDef * pUSARTx, uint8_t ch)
{
    USART_SendData(pUSARTx,ch);
    
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
}
//�����ַ���
void Usart_Send_String(USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k = 0;
    do{
        Usart_Send_Byte(pUSARTx, *(str + k));
        k++;
    }while(*(str + k) != '\0');
    
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
}
///�ض���c�⺯��printf������DBUG_USARTx���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DBUG_USARTx */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������DBUG_USARTx����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/


