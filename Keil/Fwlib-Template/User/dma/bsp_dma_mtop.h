#ifndef _BSP_DMA_MTOP_H
#define _BSP_DMA_MTOP_H

#include "stm32f10x.h"

#define USART_TX_DMA_CLK RCC_AHBPeriph_DMA1
#define USART_TX_DMA_CHANNEL DMA1_Channel4
#define USART_TX_DMA_FLAG_TC DMA1_FLAG_TC6

// 外设寄存器地址
#define USART_DR_ADDRESS (USART1_BASE + 0x04)
#define SENDBUFF_SIZE 5000

void USARTx_MTOM_Config(void);

#endif //_BSP_DMA_MTOP_H
