#include "bsp_dma_mtop.h"
#include "stm32f10x_dma.h"

uint8_t SendBuff[SENDBUFF_SIZE];

// Memory -> Peripheral(USART)
void USARTx_MTOM_Config(void)
{
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(USART_TX_DMA_CLK, ENABLE);

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_ADDRESS;
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)SENDBUFF_SIZE;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;

    DMA_InitStruct.DMA_BufferSize = SENDBUFF_SIZE;

    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;

    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(USART_TX_DMA_CHANNEL, &DMA_InitStruct);
    DMA_ClearFlag(USART_TX_DMA_FLAG_TC);
    DMA_Cmd(USART_TX_DMA_CHANNEL, ENABLE);
}
