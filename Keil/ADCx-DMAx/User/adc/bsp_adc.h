#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#define ADC_GPIO_APBxCLK RCC_APB2PeriphClockCmd

// ADC1初始化宏定义
#define ADCx_1_GPIO_CLK RCC_APB2Periph_GPIOC
#define ADCx_1_PIN1 GPIO_Pin_1
#define ADCx_1_PIN2 GPIO_Pin_2
#define ADCx_1_PIN3 GPIO_Pin_3
#define ADCx_1_GPIO_MODE GPIO_Mode_AIN
#define ADCx_1_GPIO_PORT GPIOC
#define ADCx_1_CLK RCC_APB2Periph_ADC1
#define ADCx_1_Init ADC1
#define ADCx_1_Channel1 ADC_Channel_11
#define ADCx_1_Channel2 ADC_Channel_12
#define ADCx_1_Channel3 ADC_Channel_13

// ADC2初始化宏定义
#define ADCx_2_GPIO_CLK RCC_APB2Periph_GPIOC
#define ADCx_2_PIN1 GPIO_Pin_4
#define ADCx_2_PIN2 GPIO_Pin_5
#define ADCx_2_PIN3 GPIO_Pin_0
#define ADCx_2_GPIO_MODE GPIO_Mode_AIN
#define ADCx_2_GPIO_PORT GPIOC
#define ADCx_2_CLK RCC_APB2Periph_ADC2
#define ADCx_2_Init ADC2
#define ADCx_2_Channel1 ADC_Channel_14
#define ADCx_2_Channel2 ADC_Channel_15
#define ADCx_2_Channel3 ADC_Channel_10

#define NOFCHANNEL 3 // ADC1和ADC2都只用了一个通道，若用了x个通道则定义为x

// ADC相关中断服务宏定义
#define ADC_IRQ ADC1_2_IRQn
#define ADC_IRQHandler ADC1_2_IRQHandler

// DMA宏定义
#define DMAx_CLK RCC_AHBPeriph_DMA1
#define ADC_DMA_Channel DMA1_Channel1

void ADCx_DMA_Config(void);
void ADCx_GPIO_Config(void);
void ADCx_Mode_Config(void);
void ADCx_Init(void);

#endif //_BSP_ADC_H
