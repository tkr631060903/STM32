#include "exit.h"

static void NVIC_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* 配置NVIC为优先级组1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}

// 外部中断配置
void EXIT_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO), ENABLE);
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO), ENABLE);
    NVIC_config();
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(EXIT_INPUT_A_PORT, &GPIO_InitStructure);

    EXTI_InitTypeDef EXIT_InitStructure;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXIT_InitStructure.EXTI_Line = EXTI_Line0;
    EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXIT_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXIT_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin  = EXIT_INPUT_PC13;
    GPIO_Init(EXIT_INPUT_C_PORT, &GPIO_InitStructure);
    GPIO_EXTILineConfig(EXIT_INPUT_PC13_GPIO_PortSourceGPIO, EXIT_INPUT_PC13_GPIO_PinSource);
    EXIT_InitStructure.EXTI_Line = EXIT_INPUT_PC13_EXTI_Line;
    EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXIT_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXIT_InitStructure);
}
