#include "./can/bsp_can.h"

/**
	* 1.��ʼ��CAN���裬�����ʣ�λ�����
	* 2.����ɸѡ���������������
	* 3.�������ݲ����ܣ�ʹ�ûػ�ģʽ����
	*/
void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��CAN GPIO��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	//ʹ��CAN������ص�ʱ��
	RCC_APB2PeriphClockCmd(CAN_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);

	//ʹ��PA8,9�ĵڶ�����,��Ҫ����RCC_APB2Periph_AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	
	/* ��ʼ��CAN������*/
	GPIO_InitStruct.GPIO_Pin = CAN_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_TX_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = CAN_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_RX_GPIO_PORT,&GPIO_InitStruct);
	
}

void CAN_Mode_Config(void)
{
	CAN_InitTypeDef CAN_InitTypeStruct;
	
	CAN_InitTypeStruct.CAN_ABOM = ENABLE;
	CAN_InitTypeStruct.CAN_AWUM = ENABLE;
	CAN_InitTypeStruct.CAN_Mode = CAN_Mode_LoopBack;//���Խ���ʹ�ûػ�ģʽ���������ʹ��NORMAL
	CAN_InitTypeStruct.CAN_NART = ENABLE; //�����ش�
	CAN_InitTypeStruct.CAN_RFLM = ENABLE;
	CAN_InitTypeStruct.CAN_TTCM = DISABLE;
	CAN_InitTypeStruct.CAN_TXFP = DISABLE;//��ID���ȼ�����
	
	//���ó�1Mbps
	CAN_InitTypeStruct.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitTypeStruct.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitTypeStruct.CAN_SJW = CAN_SJW_2tq;
	CAN_InitTypeStruct.CAN_Prescaler = 4;
	
	CAN_Init(CAN1,&CAN_InitTypeStruct);
}


void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterNumber=0;
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_32bit;
	
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterIdHigh=((PASS_ID << 3) | CAN_Id_Extended|CAN_RTR_Data) >> 16;
	CAN_FilterInitStruct.CAN_FilterIdLow=((PASS_ID << 3) | CAN_Id_Extended|CAN_RTR_Data)&0xFFFF;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0xFFFF;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=0xFFFF;
	
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}


void CAN_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�����ж����ȼ���
	NVIC_InitStruct.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void CAN_Config(void)
{
	CAN_GPIO_Config();
	CAN_Mode_Config();
	CAN_Filter_Config();
	CAN_NVIC_Config();
}
