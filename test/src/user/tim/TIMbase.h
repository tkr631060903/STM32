#ifndef __TIM_BASE_H
#define __TIM_BASE_H

#include "stm32f10x.h"
#include "includes.h"

/********************基本定时器TIM参数定义，只限TIM6、7************/
#define BASIC_TIM6 // 如果使用TIM7，注释掉这个宏即可

#ifdef  BASIC_TIM6 // 使用基本定时器TIM6
#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71

#else  // 使用基本定时器TIM7
#define            BASIC_TIM                   TIM7
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM7
#define            BASIC_TIM_IRQ               TIM7_IRQn
#define            BASIC_TIM_IRQHandler        TIM7_IRQHandler
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71

#endif

void BASIC_TIM_Init(void);

#endif
