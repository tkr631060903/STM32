#include "includes.h"

__IO uint16_t ADC_ConvertedValue;

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

    ADC_DMACmd(ADC_x_Init, ENABLE);
}

static void ADCx_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);

    DMA_DeInit(ADC_DMA_Channel);

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC_x_Init->DR));
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = 1;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(ADC_DMA_Channel, &DMA_InitStruct);

    DMA_Cmd(ADC_DMA_Channel, ENABLE);
}

void ADCx_Init(void)
{
    ADCx_DMA_Config();
    ADCx_GPIO_Config();
    ADCx_Mode_Config();
}
