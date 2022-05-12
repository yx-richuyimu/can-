#include "bsp_i2c.h"

/**
	* IIC EEPROM  ���ã�������������
	*/
void I2F_EE_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef  I2C_InitStruct;
	
	//��I2C GPIO��ʱ��,ͬʱ��ʼ��
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_SCL_GPIO_CLK|EEPROM_Sda_GPIO_CLK, ENABLE);
	//��I2C������ʱ��
	EEPROM_I2C_APBxClock_FUN(EEPROM_I2C_CLK, ENABLE);
	
	//��I2C SCL��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStruct.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT,&GPIO_InitStruct);
	
	//��I2C SDA��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStruct.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT,&GPIO_InitStruct);

	// ����I2C�Ĺ�������
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; //ʹ��Ӧ��
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //ʹ��7Ϊ��ַģʽ
	I2C_InitStruct.I2C_ClockSpeed = I2C_Speed; //����SCLʱ��Ƶ��
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = STM32_I2C_OWN_ADDRESS; //����stm32  I2C�������豸�ĵ�ַ��ֻҪ��������Ψһ����
	
	I2C_Init(EEPROM_I2C, &I2C_InitStruct);
	
	I2C_Cmd(EEPROM_I2C, ENABLE);
}

//��EEPROMд��һ���ֽ�
void EEPROM_Byte_Write(uint8_t addr, uint8_t data)
{
	//�ο�I2CЭ��Ĺ���
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5ʱ����
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//����EEPROM�ĵ�ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	//EV6�¼�
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	//��һ�η��͵������Ǵ洢��Ԫ��ַ
	I2C_SendData(EEPROM_I2C,addr);
	//EV8
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	//����Ҫ�洢������
	I2C_SendData(EEPROM_I2C,data);
	//EV8_2
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	//���ݴ������
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

//��EEPROMд�����ֽڣ�ҳд�룩��ÿ��д�벻�ܳ���8���ֽ�
void EEPROM_Page_Write(uint8_t addr, uint8_t *data,uint8_t numByteToWrite)
{
	//�ο�I2CЭ��Ĺ���
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5ʱ����
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//����EEPROM�ĵ�ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	//EV6�¼�
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	//��һ�η��͵������Ǵ洢��Ԫ��ַ
	I2C_SendData(EEPROM_I2C,addr);
	//EV8
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	
	while(numByteToWrite){
		//����Ҫ�洢������
		I2C_SendData(EEPROM_I2C, *data);
		//EV8_2
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
		
		data++;
		numByteToWrite--;
	}
	//���ݴ������
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}


//��EEPROM��ȡ����,uint8_t *dataֻ����ָ������޸Ĳ�������ֵ
void EEPROM_Read(uint8_t addr, uint8_t *data,uint8_t numByteToRead)
{
	//�ο�I2CЭ��Ĺ���
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5ʱ����
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//����EEPROM�ĵ�ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	//EV6�¼�,���д����ʱ����������ʱ���Ῠ���������EEPROM����д���ݵ�ʱ�򣬾�ȥ����
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	//��һ�η��͵������Ǵ洢��Ԫ��ַ
	I2C_SendData(EEPROM_I2C,addr);
	//EV8
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	//����Ҫ�洢������
	
	
	
	//�ڶ�����ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//EV5ʱ����
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	//����EEPROM�ĵ�ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Receiver);
	//EV6�¼�
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR);
	
	
	while(numByteToRead){
		if(numByteToRead == 1){
			//������һ���ֽڣ��Ȳ�����Ӧ���ź�
			I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
			//*data = I2C_ReceiveData(EEPROM_I2C);
		}
		//��ʼ��������
		//EV7�¼�
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR);
		//��ȡ�Ĵ������ݣ��洢����ַ��
		*data = I2C_ReceiveData(EEPROM_I2C);
		//ָ���ַ+1ָ����һ����ַ
		data++;
		
		numByteToRead--;
	}

	
	
	//���ݴ������
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	// ����ʹ����Ӧ,�Ա��´�ͨѶ
	I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
}



//�ȴ�EEPROM�ڲ�ʱ�����
void EEPROM_WaitForWriteEnd(void)
{
	do{
		//�ο�I2CЭ��Ĺ���
		//������ʼ�ź�
		I2C_GenerateSTART(EEPROM_I2C, ENABLE);
		//ʱ����,���ﲻ��ʹ��I2C_CheckEvent��������ΪI2C_CheckEvent�����������������������޷��ﵽЧ��
		while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_SB) == RESET);
		//����EEPROM�ĵ�ַ
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDE, I2C_Direction_Transmitter);
	}//���ﲻ��ʹ��I2C_CheckEvent��������ΪI2C_CheckEvent�����������������������޷��ﵽЧ��
	while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_ADDR) == RESET);
	
	//EEPROM�ڲ�ʱ����ɴ������
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}
