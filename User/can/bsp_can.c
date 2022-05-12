#include "./can/bsp_can.h"

/**
	* 1.初始化CAN外设，波特率，位的组成
	* 2.配置筛选器，方便接收数据
	* 3.发送数据并接受，使用回环模式测试
	*/
void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//打开CAN GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	//使能CAN引脚相关的时钟
	RCC_APB2PeriphClockCmd(CAN_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);

	//使用PA8,9的第二功能,需要开启RCC_APB2Periph_AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	
	/* 初始换CAN的引脚*/
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
	CAN_InitTypeStruct.CAN_Mode = CAN_Mode_LoopBack;//调试建议使用回环模式，调试完后使用NORMAL
	CAN_InitTypeStruct.CAN_NART = ENABLE; //错误重传
	CAN_InitTypeStruct.CAN_RFLM = ENABLE;
	CAN_InitTypeStruct.CAN_TTCM = DISABLE;
	CAN_InitTypeStruct.CAN_TXFP = DISABLE;//按ID优先级发送
	
	//配置成1Mbps
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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//配置中段优先级组
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
