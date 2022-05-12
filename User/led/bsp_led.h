#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_G_GPIO_PIN       GPIO_Pin_0
#define LED_B_GPIO_PIN       GPIO_Pin_1
#define LED_R_GPIO_PIN       GPIO_Pin_5
#define LED_GPIO_PORT        GPIOB
#define LED_GPIO_CLK         RCC_APB2Periph_GPIOB

#define   ON     1
#define   OFF    0

// \  C语言里面叫续行符，后面不能有任何的东西
#define  LED_G(a)  if(a) \
	                       GPIO_ResetBits(LED_GPIO_PORT, LED_G_GPIO_PIN); \
                   else \
										     GPIO_SetBits(LED_GPIO_PORT, LED_G_GPIO_PIN);
#define  LED_B(a)  if(a) \
	                       GPIO_ResetBits(LED_GPIO_PORT, LED_B_GPIO_PIN); \
                   else \
										     GPIO_SetBits(LED_GPIO_PORT, LED_B_GPIO_PIN);
#define  LED_R(a)  if(a) \
	                       GPIO_ResetBits(LED_GPIO_PORT, LED_R_GPIO_PIN); \
                   else \
										     GPIO_SetBits(LED_GPIO_PORT, LED_R_GPIO_PIN);

void LED_GPIO_Config(uint16_t LED_GPIO_PIN);

#endif /* __BSP_LED_H */
