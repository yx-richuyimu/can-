// bsp：board support package 板级支持包
#include "bsp_led.h"

void LED_GPIO_Config(uint16_t LED_GPIO_PIN)
{
	GPIO_InitTypeDef GPIO_InitStruct; // 初始化
	
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE); //时钟
	
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}
