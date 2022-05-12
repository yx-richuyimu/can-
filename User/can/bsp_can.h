#ifndef __BSP_CAN_H

#define __BSP_CAN_H

#include "stm32f10x.h"
#include <stdio.h>


#define CAN_TX_GPIO_PORT                 GPIOB
#define CAN_TX_GPIO_PIN                  GPIO_Pin_9

#define CAN_RX_GPIO_PORT                 GPIOB
#define CAN_RX_GPIO_PIN                  GPIO_Pin_8

#define CAN_GPIO_CLK                     RCC_APB2Periph_GPIOB

#define PASS_ID   ((uint32_t)0x1314)


void CAN_Config(void);


/*ÐÅÏ¢Êä³ö*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-CAN-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-CAN-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-CAN-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#endif /* __BSP_CAN_H */
	
