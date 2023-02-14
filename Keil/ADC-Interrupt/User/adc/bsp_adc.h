#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#define ADC_GPIO_APBxCLK RCC_APB2PeriphClockCmd

// GPIO初始化宏定义
#define ADC_GPIO_CLK RCC_APB2Periph_GPIOC
#define ADC_PIN GPIO_Pin_1
#define GPIO_ADC_Mode GPIO_Mode_AIN
#define GPIO_ADC_PORT GPIOC

// ADC初始化宏定义
#define ADC_CLK RCC_APB2Periph_ADC2
#define ADC_x_Init ADC2
#define ADC_Channel ADC_Channel_11

// ADC相关中断服务宏定义
#define ADC_IRQ ADC1_2_IRQn
#define ADC_IRQHandler ADC1_2_IRQHandler

void ADCx_NVIC_Config(void);
void ADCx_GPIO_Config(void);
void ADCx_Mode_Config(void);
void ADCx_Init(void);

#endif //_BSP_ADC_H
