#include "rx_data_queue.h"
//���λ������������ڽ����ⲿ����ʱ��������


//ʵ�����ڵ���������
QUEUE_DATA_TYPE  node_data[QUEUE_NODE_NUM]; 
//ʵ������������
QueueBuffer rx_queue;

//���л��������ڴ��
__align(4) char node_buff[QUEUE_NODE_NUM][QUEUE_NODE_DATA_LEN] ;



/*���λ������*/

/**
  * @brief  ��ʼ���������
  * @param  cb:������нṹ��
  * @param  size: ������е�Ԫ�ظ���
  * @note 	��ʼ��ʱ����Ҫ��cb->elemsָ�븳ֵ
  */
void cbInit(QueueBuffer *cb, int size) 
{
    cb->size  = size;	/* maximum number of elements           */
    cb->read = 0; 		/* index of oldest element              */
    cb->write   = 0; 	 	/* index at which to write new element  */
//    cb->elems = (uint8_t *)calloc(cb->size, sizeof(uint8_t));  //elems Ҫ�����ʼ��
}
 
//(�˺����ĳ��˺꣬��ͷ�ļ�)
/**
  * @brief  ���������е�ǰ��״̬��Ϣ
  * @param  cb:������нṹ��
  */
//void cbPrint(QueueBuffer *cb) 
//{
//    DATA_QUEUE_LOG("size=0x%x, read=%d, write=%d\n", cb->size, cb->read, cb->write);
//	  DATA_QUEUE_LOG("size=0x%x, read_using=%d, write_using=%d\n", cb->size, cb->read_using, cb->write_using);
//}
 
/**
  * @brief  �жϻ��������(1)��(0)����
  * @param  cb:������нṹ��
  */
int cbIsFull(QueueBuffer *cb) 
{
    return cb->write == (cb->read ^ cb->size);  //�⽫�ڱȽ�֮ǰ��ת��ȡ������Ҫ��λ
}
 
/**
  * @brief  �жϻ��������(1)��(0)ȫ��
  * @param  cb:������нṹ��
  */		
int cbIsEmpty(QueueBuffer *cb) 
{
    return cb->write == cb->read; 
}

/**
  * @brief  �Ի�����е�ָ���1
  * @param  cb:������нṹ��
  * @param  p��Ҫ��1��ָ��
  * @return  ���ؼ�1�Ľ��
  */	
int cbIncr(QueueBuffer *cb, int p) 
{
    return (p + 1)&(2*cb->size-1); /* ��дָ��������ǰ�2*sizeȡģ��ɵ� */
    //             (2*cb->size-1) == 3 == 0000 0000 0000 0000 0000 0000 0000 0011  ��size = 2�� ��ֹp+1Խ��  QUEUE_NODE_NUM = 2
}
 
/**
  * @brief  ��ȡ��д��Ļ�����ָ��
  * @param  cb:������нṹ��
  * @return  �ɽ���д��Ļ�����ָ��
  * @note  �õ�ָ���ɽ���д���������дָ�벻��������1��
           д������ʱ��Ӧ����cbWriteFinish��дָ���1
  */ //write_using�仯����Ϊ��0-3 ѭ�� ��Ϊ3ʱ��ִ��cbIncr()����,(p + 1)&(2*cb->size-1)=(3+1)&3=0  (size=2)
QUEUE_DATA_TYPE* cbWrite(QueueBuffer *cb) 
{
    if (cbIsFull(cb)) /* ���������ƶ���ָ�� */
    {
			return NULL;
		}		
		else
		{
			//��wriet��write_using���ʱ����ʾ��һ����������д����ϣ���Ҫ��дָ���1
			if(cb->write == cb->write_using)
			{
				cb->write_using = cbIncr(cb, cb->write); //δ����������1
			}
		}
		
	return  cb->elems[cb->write_using&(cb->size-1)];
}



/**
  * @brief ����д����ϣ�����дָ�뵽����ṹ��
  * @param  cb:������нṹ��
  */
void cbWriteFinish(QueueBuffer *cb)
{
    cb->write = cb->write_using;
}
 
/**
  * @brief  ��ȡ�ɶ�ȡ�Ļ�����ָ��
  * @param  cb:������нṹ��
  * @return  �ɽ��ж�ȡ�Ļ�����ָ��
  * @note  �õ�ָ���ɽ����ȡ����������ָ�벻��������1��
					 ��ȡ������ʱ��Ӧ����cbReadFinish�Զ�ָ���1
  */
QUEUE_DATA_TYPE* cbRead(QueueBuffer *cb) 
{
		if(cbIsEmpty(cb))
			return NULL;
		
	//��read��read_using���ʱ����ʾ��һ���������Ѷ�ȡ���(���ѵ���cbReadFinish)��
	//��Ҫ��дָ���1
	if(cb->read == cb->read_using)	
		cb->read_using = cbIncr(cb, cb->read);
	
	return cb->elems[cb->read_using&(cb->size-1)];
}


/**
  * @brief ���ݶ�ȡ��ϣ����¶�ָ�뵽����ṹ��
  * @param  cb:������нṹ��
  */
void cbReadFinish(QueueBuffer *cb) 
{	
		//���õ�ǰ��������ݽڵ�ĳ���
		cb->elems[cb->read_using&(cb->size-1)]->len = 0;
	
    cb->read = cb->read_using;
}



//���е�ָ��ָ��Ļ�����ȫ������
void camera_queue_free(void)
{
    uint32_t i = 0;

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        if(node_data[i].head != NULL)
        {
					//���Ƕ�̬����Ŀռ��Ҫfree
//            free(node_data[i].head);
            node_data[i].head = NULL;
        }
    }

    return;
}


/**
  * @brief  ������г�ʼ���������ڴ�,ʹ�û������ʱ��
  * @param  ��
  * @retval ��
  */
void rx_queue_init(void) //QUEUE_NODE_NUM == 2
{
  uint32_t i = 0;

  memset(node_data, 0, sizeof(node_data));
		 
	/*��ʼ���������*/
	cbInit(&rx_queue,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_data[i].head = node_buff[i];       //���ڴ��node_buff[]�����ڵ�node_data[]
        
        /*��ʼ�����л���ָ�룬ָ��ʵ�ʵ��ڴ�*/
        rx_queue.elems[i] = &node_data[i];      //���ڵ�node_data[]�������л���ָ��rx_queue.elems[]
        
        DATA_QUEUE_LOG("node_data[i].head=0x%x,\r\nrx_queue.elems[i] =0x%x", (uint32_t)node_data[i].head,(uint32_t)rx_queue.elems[i]->head);

        memset(node_data[i].head, 0, QUEUE_NODE_DATA_LEN);
    }
		
	cbPrint(rx_queue);	
}



/**
  * @brief  ��������д�����ݵ�����
  * *src_dat ����д����ַ�������
  *  src_len �����ַ����Ĵ�С 
  */
void push_data_to_queue(char *src_dat,uint16_t src_len)//���������� ÿ���������ɴ�2048byte���� ��һ��дΪ��һ��������0  �ڶ���дΪ�ڶ���������1 0��1Ϊ�ڴ�صĶ�ά�����±�
{
	QUEUE_DATA_TYPE *data_p;
	uint8_t i;
	
	for(i=0;i<src_len;i++)
	{
		/*��ȡд������ָ�룬׼��д��������*/
		data_p = cbWrite(&rx_queue);
		
		if (data_p != NULL)	//���������δ������ʼ����
		{		
			//��������д�����ݣ���ʹ�ô��ڽ��ա�dmaд��ȷ�ʽ
			*(data_p->head + i) = src_dat[i];
				data_p->len++;
			printf("\r\ndata_p->len =%d",data_p->len);
		}else return;	
		
		cbPrint(rx_queue);	
	}	
	
	/*д�뻺�������*/
	cbWriteFinish(&rx_queue);
	
	cbPrint(rx_queue);	

}


/**
  * @brief  �Ӷ�����ȡ���ݵ�����
  */
void pull_data_from_queue(void)
{
	QUEUE_DATA_TYPE *rx_data;	
		
	/*�ӻ�������ȡ���ݣ����д���*/
	rx_data = cbRead(&rx_queue); 

	if(rx_data != NULL)//������зǿ�
	{		
		//�����ַ���������������ֱ������ַ���
		*(rx_data->head+rx_data->len) = '\0';
		
		QUEUE_DEBUG("���յ������ݣ�%s",rx_data->head);
		QUEUE_DEBUG_ARRAY((uint8_t*)rx_data->head,rx_data->len);

		//ʹ�������ݱ������cbReadFinish���¶�ָ��
		cbReadFinish(&rx_queue);
	}
}
