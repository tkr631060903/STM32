#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f10x.h"

#define LED_G_GPIO_PIN  GPIO_Pin_0
#define LED_B_GPIO_PIN  GPIO_Pin_1
#define LED_R_GPIO_PIN  GPIO_Pin_5
#define LED_GPIO_Port   GPIOB
#define LED_GPIO_CLK    RCC_APB2Periph_GPIOB

void LED_GPIO_Config(void);
void Delay(uint32_t count);
void Light(void);
void TOGGLE(void);

#endif /*_BSP_LED_H*/
