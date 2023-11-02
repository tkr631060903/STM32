#ifndef _CAN_H
#define _CAN_H

#include "stm32f10x.h"
#include "stm32f10x_can.h"

#define PASS_ID          ((uint32_t)0x1314)

#define CAN_TX_GPIO_PROT GPIOB
#define CAN_TX_GPIO_PIN  GPIO_Pin_9

#define CAN_RX_GPIO_PORT GPIOB
#define CAN_RX_GPIO_PIN  GPIO_Pin_8

#define CAN_GPIO_CLK     RCC_APB2Periph_GPIOB

void CAN_Config(void);  

#endif
