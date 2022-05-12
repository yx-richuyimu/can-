#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"

#define EEPROM_ADDE 0xA0
/**
	* IIC EEPROM的引脚定义
	*/

//IIC1
#define EEPROM_I2C                                 I2C1
#define EEPROM_I2C_CLK                             RCC_APB1Periph_I2C1
#define EEPROM_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd

// IIC GPIO 引脚定义
#define EEPROM_SCL_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define EEPROM_Sda_GPIO_CLK                        RCC_APB2Periph_GPIOB

#define EEPROM_I2C_GPIO_APBxClkCmd                 RCC_APB2PeriphClockCmd

#define EEPROM_I2C_SCL_GPIO_PORT                   GPIOB
#define EEPROM_I2C_SCL_GPIO_PIN                    GPIO_Pin_6

#define EEPROM_I2C_SDA_GPIO_PORT                   GPIOB
#define EEPROM_I2C_SDA_GPIO_PIN                    GPIO_Pin_7


/* STM32 I2C 快速模式 */
#define I2C_Speed                                  400000  //*
#define STM32_I2C_OWN_ADDRESS                      0x5F


void I2F_EE_Config(void);
void EEPROM_Byte_Write(uint8_t addr, uint8_t data);
void EEPROM_Read(uint8_t addr, uint8_t *data,uint8_t numByteToRead);
void EEPROM_WaitForWriteEnd(void);
void EEPROM_Page_Write(uint8_t addr, uint8_t *data,uint8_t numByteToWrite);
#endif /* __BSP_I2C_H */
