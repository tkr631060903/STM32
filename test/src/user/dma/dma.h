#ifndef __DMA_H
#define __DMA_H

#include "includes.h"

// 当使用存储器到存储器模式时候，通道可以随便选，没有硬性的规定
#define DMA_CHANNEL   DMA1_Channel6
#define DMA_CLOCK_MTM RCC_AHBPeriph_DMA1

// 传输完成标志
#define DMA_COMPLETE_FLAG DMA1_FLAG_TC6

// 要发送的数据大小
#define BUFFER_SIZE 32

// 串口对应的DMA请求通道
#define USART_TX_DMA_CHANNEL DMA1_Channel4
// 外设寄存器地址
#define USART_DR_ADDRESS (USART1_BASE + 0x04)
// 一次发送的数据量
#define SENDBUFF_SIZE 32

void DMA_MTM_config(void);
void USARTx_DMA_Config(void);

#endif // !__DMA
