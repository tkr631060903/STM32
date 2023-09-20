#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"
#include "includes.h"

// 双模式时，ADC1和ADC2转换的数据都存放在ADC1的数据寄存器，
// ADC1的在低十六位，ADC2的在高十六位
// 双ADC模式的第一个ADC，必须是ADC1
// ADC1
#define ADCx_1               ADC1
#define ADCx_1_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADCx_1_CLK           RCC_APB2Periph_ADC1

// ADC1 GPIO 引脚宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define ADCx_1_GPIO_CLK        RCC_APB2Periph_GPIOC
#define ADCx_1_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd
#define ADCx_1_CHANNEL_1       ADC_Channel_11
#define ADCx_1_CHANNEL_1_PORT  GPIOC
#define ADCx_1_CHANNEL_1_PIN   GPIO_Pin_1
#define ADCx_1_CHANNEL_2       ADC_Channel_13
#define ADCx_1_CHANNEL_2_PORT  GPIOC
#define ADCx_1_CHANNEL_2_PIN   GPIO_Pin_3

// 双ADC模式的第二个ADC，必须是ADC2
// ADC2
#define ADCx_2               ADC2
#define ADCx_2_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADCx_2_CLK           RCC_APB2Periph_ADC2

// ADC2 GPIO 引脚宏定义
#define ADCx_2_GPIO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADCx_2_GPIO_CLK           RCC_APB2Periph_GPIOC
#define ADCx_2_CHANNEL_1          ADC_Channel_14
#define ADCx_2_CHANNEL_1_PORT     GPIOC
#define ADCx_2_CHANNEL_1_PIN      GPIO_Pin_4
#define ADCx_2_CHANNEL_2          ADC_Channel_15
#define ADCx_2_CHANNEL_2_PORT     GPIOC
#define ADCx_2_CHANNEL_2_PIN      GPIO_Pin_5

#define NOFCHANEL                 2

// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define ADC_DMA_CHANNEL DMA1_Channel1

void ADCx_Init(void);

#endif
