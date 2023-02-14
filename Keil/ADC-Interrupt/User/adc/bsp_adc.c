#include "includes.h"

static void ADCx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(ADC_GPIO_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = ADC_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_ADC_Mode;

    GPIO_Init(GPIO_ADC_PORT, &GPIO_InitStruct);
}

static void ADCx_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStruct;

    RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);

    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 1;

    ADC_Init(ADC_x_Init, &ADC_InitStruct);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_RegularChannelConfig(ADC_x_Init, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    ADC_ITConfig(ADC_x_Init, ADC_IT_EOC, ENABLE);
    ADC_Cmd(ADC_x_Init, ENABLE);
    ADC_StartCalibration(ADC_x_Init); // ADC开始校准
    while (ADC_GetCalibrationStatus(ADC_x_Init))
        ; //等待校准完成
    ADC_SoftwareStartConvCmd(ADC_x_Init, ENABLE);
}

static void ADCx_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStruct);
}

void ADCx_Init(void)
{
    ADCx_NVIC_Config();
    ADCx_GPIO_Config();
    ADCx_Mode_Config();
}
