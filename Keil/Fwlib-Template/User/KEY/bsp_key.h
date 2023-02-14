#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "stm32f10x.h"

#define KEY1_GPIO_PIN    GPIO_Pin_0
#define KEY1_GPIO_Port   GPIOA
#define KEY1_GPIO_CLK    RCC_APB2Periph_GPIOA

#define KEY_ON           1
#define KEY_OFF          0

void    KEY_GPIO_Config(void);
uint8_t KEY_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif
