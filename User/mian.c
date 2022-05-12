#include "stm32f10x.h" 
#include "bsp_usart.h"
#include "./can/bsp_can.h"
#include "./key/bsp_key.h"

CanRxMsg CAN_Rece_Data;
CanTxMsg CAN_Tran_Data;
uint8_t flag = 0;
uint8_t box;

static void can_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	USART_Config();
	CAN_Config();
	KEY1_GPIO_Config();
	
	printf("\r\n 野火F103-霸道 CAN通讯回环实验例程\r\n");
	
	while(1){
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			printf("\r\n key1 \r\n");
			CAN_Tran_Data.StdId = 0;
			CAN_Tran_Data.ExtId = PASS_ID;
			CAN_Tran_Data.RTR = CAN_RTR_Data;
			CAN_Tran_Data.IDE = CAN_Id_Extended;
			CAN_Tran_Data.DLC = 1;
			CAN_Tran_Data.Data[0] = 10;
			
			box = CAN_Transmit(CAN1, &CAN_Tran_Data);
			
			while(CAN_TransmitStatus(CAN1,box) == CAN_TxStatus_Failed);
			
			printf("\r\n 数据发送完成 \r\n"); //与文件的编码格式有关
			//can_delay(0xFFFFFF);
		}
		
		
		if(flag == 1)
		{
			printf("\r\n 接收到的数据0：%d \r\n", CAN_Rece_Data.Data[0]);
			flag = 0;
		}
	}
	
}

