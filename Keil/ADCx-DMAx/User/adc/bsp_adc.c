#include "includes.h"

__IO uint32_t ADC_ConvertedValue[NOFCHANNEL];

static void ADCx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    //  ADC1 GPIO初始化
    RCC_APB2PeriphClockCmd(ADCx_1_GPIO_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = ADCx_1_PIN1;
    GPIO_InitStruct.GPIO_Pin = ADCx_1_PIN2;
    GPIO_InitStruct.GPIO_Pin = ADCx_1_PIN3;
    GPIO_InitStruct.GPIO_Mode = ADCx_1_GPIO_MODE;

    GPIO_Init(ADCx_1_GPIO_PORT, &GPIO_InitStruct);

    // ADC2 GPIO初始化
    RCC_APB2PeriphClockCmd(ADCx_2_GPIO_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = ADCx_2_PIN1;
    GPIO_InitStruct.GPIO_Pin = ADCx_2_PIN2;
    GPIO_InitStruct.GPIO_Pin = ADCx_2_PIN3;
    GPIO_InitStruct.GPIO_Mode = ADCx_2_GPIO_MODE;

    GPIO_Init(ADCx_2_GPIO_PORT, &GPIO_InitStruct);
}

static void ADCx_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStruct;

    // ADC1初始化
    RCC_APB2PeriphClockCmd(ADCx_1_CLK, ENABLE);

    ADC_DeInit(ADCx_1_Init);

    ADC_InitStruct.ADC_Mode = ADC_Mode_RegSimult;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = NOFCHANNEL;

    ADC_Init(ADCx_1_Init, &ADC_InitStruct);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_RegularChannelConfig(ADCx_1_Init, ADCx_1_Channel1, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADCx_1_Init, ADCx_1_Channel2, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADCx_1_Init, ADCx_1_Channel3, 3, ADC_SampleTime_55Cycles5);

    // ADC2初始化
    RCC_APB2PeriphClockCmd(ADCx_2_CLK, ENABLE);

    ADC_DeInit(ADCx_2_Init);

    ADC_InitStruct.ADC_Mode = ADC_Mode_RegSimult;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = NOFCHANNEL;

    ADC_Init(ADCx_2_Init, &ADC_InitStruct);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_RegularChannelConfig(ADCx_2_Init, ADCx_2_Channel1, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADCx_2_Init, ADCx_2_Channel2, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADCx_2_Init, ADCx_2_Channel3, 3, ADC_SampleTime_55Cycles5);

    // ADC1校准
    ADC_Cmd(ADCx_1_Init, ENABLE);
    ADC_StartCalibration(ADCx_1_Init); // ADC开始校准
    while (ADC_GetCalibrationStatus(ADCx_1_Init))
        ; //等待校准完成

    // ADC2校准
    ADC_Cmd(ADCx_2_Init, ENABLE);
    ADC_StartCalibration(ADCx_2_Init); // ADC开始校准
    while (ADC_GetCalibrationStatus(ADCx_2_Init))
        ; //等待校准完成

    ADC_ExternalTrigConvCmd(ADCx_2_Init, ENABLE); //使能ADC2外部触发
    ADC_SoftwareStartConvCmd(ADCx_1_Init, ENABLE);

    ADC_DMACmd(ADCx_1_Init, ENABLE);
}

static void ADCx_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);

    DMA_DeInit(ADC_DMA_Channel);

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(ADCx_1_Init->DR));
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = NOFCHANNEL;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
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
