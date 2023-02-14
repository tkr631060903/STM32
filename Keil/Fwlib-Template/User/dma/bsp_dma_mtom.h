#ifndef _BSP_DMA_MTOM_H
#define _BSP_DMA_MTOM_H

#include "stm32f10x.h"

// 要发送的数据大小
#define BUFFER_SIZE 32

#define MTOM_DMA_CLK RCC_AHBPeriph_DMA1
#define MTOM_DMA_CHANNEL DMA1_Channel4
#define MTOM_DMA_FLAG_TC DMA1_FLAG_TC4

void DMA_MTOM_Config(void);
uint8_t Buffercmp(const uint32_t *pBuffer, uint32_t *pBuffer1, uint16_t BufferLength);

#endif //_BSP_DMA_M_M_H
