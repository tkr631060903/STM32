#include "bsp_exti.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"

static void EXTI0_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;   //NVIC_InitTypeDef NVIC初始化结构体

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //配置NVIC为优先级组1

    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;   //配置中断源按键(IO)所在NVIC_IRQChannel
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  //配置抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //配置子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    //使能中断通道

    NVIC_Init(&NVIC_InitStruct);    //NVIC_Init NVIC初始化函数
}

void EXTI0_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;   //GPIO_InitTypeDef GPIO初始化结构体
    EXTI_InitTypeDef EXTI_InitStruct;   //EXTI_InitTypeDef EXTI初始化结构体

    //配置中断优先级
    EXTI0_NVIC_Config();

    //初始化GPIO
    RCC_APB2PeriphClockCmd(KEY1_INT_CLK, ENABLE);   //开启按键GPIO口时钟
    GPIO_InitStruct.GPIO_Pin = KEY1_INT_GPIO_PIN;   //配置触发中断事件的按键GPIO
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置为浮空输入
    GPIO_Init(KEY1_INT_Port, &GPIO_InitStruct); //GPIO初始化函数

    //初始化EXTI
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //配置触发中断事件源
    //GPIO_EXTILineConfig(RCC_APB2Periph_AFIO, GPIO_PinSource13);

    EXTI_InitStruct.EXTI_Line = EXTI_Line0; //配置触发中断事件的按键GPIO所在Lines
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;    //配置EXTI为中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //配置为下降沿触发中断事件
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //使能中断
    EXTI_Init(&EXTI_InitStruct);    //EXTI_Init 外部中断初始函数 
}

static void EXTI1_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStruct);
}

void EXTI1_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;

    //配置中断优先级
    EXTI1_NVIC_Config();

    //初始化GPIO
    RCC_APB2PeriphClockCmd(KEY2_INT_CLK, ENABLE);
    GPIO_InitStruct.GPIO_Pin = KEY2_INT_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY2_INT_Port, &GPIO_InitStruct);

    //初始化EXTI
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
    //GPIO_EXTILineConfig(RCC_APB2Periph_AFIO, GPIO_PinSource13);

    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
}
