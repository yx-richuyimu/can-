#include "bsp_i2c.h"

/**
	* IIC EEPROM  配置，工作参数配置
	*/
void I2F_EE_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef  I2C_InitStruct;
	
	//打开I2C GPIO的时钟,同时初始化
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_SCL_GPIO_CLK|EEPROM_Sda_GPIO_CLK, ENABLE);
	//打开I2C的外设时钟
	EEPROM_I2C_APBxClock_FUN(EEPROM_I2C_CLK, ENABLE);
	
	//将I2C SCL的GPIO配置为推挽复用模式
	GPIO_InitStruct.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT,&GPIO_InitStruct);
	
	//将I2C SDA的GPIO配置为推挽复用模式
	GPIO_InitStruct.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT,&GPIO_InitStruct);

	// 配置I2C的工作参数
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; //使能应答
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //使用7为地址模式
	I2C_InitStruct.I2C_ClockSpeed = I2C_Speed; //配置SCL时钟频率
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = STM32_I2C_OWN_ADDRESS; //这是stm32  I2C上自身设备的地址，只要在总线上唯一即可
	
	I2C_Init(EEPROM_I2C, &I2C_InitStruct);
	
	I2C_Cmd(EEPROM_I2C, ENABLE);
}

//向EEPROM写入一个字节
void EEPROM_Byte_Write(uint8_t addr, uint8_t data)
{
	//参考I2C协议的过程
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5时间检测
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//发送EEPROM的地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	//EV6事件
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	//第一次发送的数据是存储单元地址
	I2C_SendData(EEPROM_I2C,addr);
	//EV8
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	//发送要存储的数据
	I2C_SendData(EEPROM_I2C,data);
	//EV8_2
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	//数据传输完成
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

//向EEPROM写入多个字节（页写入），每次写入不能超过8个字节
void EEPROM_Page_Write(uint8_t addr, uint8_t *data,uint8_t numByteToWrite)
{
	//参考I2C协议的过程
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5时间检测
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//发送EEPROM的地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	//EV6事件
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	//第一次发送的数据是存储单元地址
	I2C_SendData(EEPROM_I2C,addr);
	//EV8
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	
	while(numByteToWrite){
		//发送要存储的数据
		I2C_SendData(EEPROM_I2C, *data);
		//EV8_2
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
		
		data++;
		numByteToWrite--;
	}
	//数据传输完成
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}


//向EEPROM读取数据,uint8_t *data只有用指针才能修改参数的数值
void EEPROM_Read(uint8_t addr, uint8_t *data,uint8_t numByteToRead)
{
	//参考I2C协议的过程
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5时间检测
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//发送EEPROM的地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	//EV6事件,如果写数据时，不增加延时，会卡死在这里，即EEPROM还在写数据的时候，就去读了
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	//第一次发送的数据是存储单元地址
	I2C_SendData(EEPROM_I2C,addr);
	//EV8
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	//发送要存储的数据
	
	
	
	//第二次起始信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5时间检测
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//发送EEPROM的地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Receiver);
	//EV6事件
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR);
	
	
	while(numByteToRead){
		if(numByteToRead == 1){
			//如果最后一个字节，先产生非应答信号
			I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
			//*data = I2C_ReceiveData(EEPROM_I2C);
		}
		//开始接收数据
		//EV7事件
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR);
		//读取寄存器数据，存储到地址里
		*data = I2C_ReceiveData(EEPROM_I2C);
		//指针地址+1指向下一个地址
		data++;
		
		numByteToRead--;
	}

	
	
	//数据传输完成
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	// 重新使能响应,以便下次通讯
	I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
}



//等待EEPROM内部时序完成
void EEPROM_WaitForWriteEnd(void)
{
	do{
		//参考I2C协议的过程
		//产生起始信号
		I2C_GenerateSTART(EEPROM_I2C, ENABLE);
		//时间检测,这里不能使用I2C_CheckEvent函数，因为I2C_CheckEvent函数会有其他操作，导致无法达到效果
		while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_SB) == RESET);
		//发送EEPROM的地址
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	}//这里不能使用I2C_CheckEvent函数，因为I2C_CheckEvent函数会有其他操作，导致无法达到效果
	while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_ADDR) == RESET);
	
	//EEPROM内部时序完成传输完成
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}
