#ifndef _BSP_EXTI_H
#define _BSP_EXTI_H

#include "stm32f10x.h"

#define KEY1_INT_GPIO_PIN   GPIO_Pin_0
#define KEY1_INT_Port       GPIOA
#define KEY1_INT_CLK        (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)

#define KEY2_INT_GPIO_PIN   GPIO_Pin_13
#define KEY2_INT_Port       GPIOC
#define KEY2_INT_CLK        (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)

void EXTI0_Key_Config(void);
void EXTI1_Key_Config(void);

#endif //_BSP_EXTI_H
