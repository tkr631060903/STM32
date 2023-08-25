#ifndef __led_H
#define __led_H

#include "stm32f10x.h"

#define LED_PERIPH RCC_APB2Periph_GPIOB
#define LED_PORT   GPIOB
#define LED_PIN_G  GPIO_Pin_0
#define LED_PIN_B  GPIO_Pin_1
#define LED_PIN_R  GPIO_Pin_5

void LED_Init(void);
void btn_Input(void);
void LED_R(void);
void LED_G(void);
void LED_B(void);

#endif /* __led_H */
