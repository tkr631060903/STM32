#ifndef __DMA_H
#define __DMA_H

#include "includes.h"

// 当使用存储器到存储器模式时候，通道可以随便选，没有硬性的规定
#define DMA_CHANNEL DMA1_Channel6
#define DMA_CLOCK_MTM   RCC_AHBPeriph_DMA1

// 传输完成标志
#define DMA_COMPLETE_FLAG DMA1_FLAG_TC6

// 要发送的数据大小
#define BUFFER_SIZE 32

// /* 定义aSRC_Const_Buffer数组作为DMA传输数据源
//  * const关键字将aSRC_Const_Buffer数组变量定义为常量类型
//  * 表示数据存储在内部的FLASH中
//  */
// const uint32_t aSRC_Const_Buffer[BUFFER_SIZE] =
//     {
//         0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
//         0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
//         0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
//         0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
//         0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
//         0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
//         0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
//         0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80};
// /* 定义DMA传输目标存储器
//  * 存储在内部的SRAM中
//  */
// uint32_t aDST_Buffer[BUFFER_SIZE];

void DMA_MTM_config(void);

#endif // !__DMA
