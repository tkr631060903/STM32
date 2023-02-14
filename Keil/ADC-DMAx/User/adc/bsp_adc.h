#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#define ADC_GPIO_APBxCLK RCC_APB2PeriphClockCmd

// GPIO初始化宏定义
#define ADC_GPIO_CLK RCC_APB2Periph_GPIOC
#define ADC_PIN1 GPIO_Pin_0
#define ADC_PIN2 GPIO_Pin_1
#define ADC_PIN3 GPIO_Pin_2
#define ADC_PIN4 GPIO_Pin_3
#define ADC_PIN5 GPIO_Pin_4
#define ADC_PIN6 GPIO_Pin_5
#define GPIO_ADC_Mode GPIO_Mode_AIN
#define GPIO_ADC_PORT GPIOC

// ADC初始化宏定义
#define NOFCHANNEL 6
#define ADC_CLK RCC_APB2Periph_ADC1
#define ADC_x_Init ADC1
#define ADC_Channel1 ADC_Channel_10
#define ADC_Channel2 ADC_Channel_11
#define ADC_Channel3 ADC_Channel_12
#define ADC_Channel4 ADC_Channel_13
#define ADC_Channel5 ADC_Channel_14
#define ADC_Channel6 ADC_Channel_15

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
