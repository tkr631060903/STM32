#include "dma.h"

void DMA_MTM_config(void)
{
    /* 定义aSRC_Const_Buffer数组作为DMA传输数据源
     * const关键字将aSRC_Const_Buffer数组变量定义为常量类型
     * 表示数据存储在内部的FLASH中
     */
    uint32_t aSRC_Const_Buffer[BUFFER_SIZE] =
        {
            0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
            0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
            0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
            0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
            0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
            0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
            0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
            0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80};
    /* 定义DMA传输目标存储器
     * 存储在内部的SRAM中
     */
    uint32_t aDST_Buffer[BUFFER_SIZE];
    DMA_InitTypeDef DMA_InitStructure;
    // 开启DMA时钟
    RCC_AHBPeriphClockCmd(DMA_CLOCK_MTM, ENABLE);
    // 源数据地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
    // 目标地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer;
    // 方向：外设到存储器（这里的外设是内部的FLASH）
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    // 传输大小
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;
    // 外设（内部的FLASH）地址递增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    // 内存地址递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // 外设数据单位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    // 内存数据单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    // DMA模式，一次或者循环模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    // DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //  优先级：高
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    // 使能内存到内存的传输
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    // 配置DMA通道
    DMA_Init(DMA_CHANNEL, &DMA_InitStructure);
    // 使能DMA
    DMA_Cmd(DMA_CHANNEL, ENABLE);
}

void USARTx_DMA_Config(void)
{
    uint8_t SendBuff[SENDBUFF_SIZE] = {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'};

    DMA_InitTypeDef DMA_InitStructure;

    // 开启DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 设置DMA源地址：串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_ADDRESS;
    // 内存地址(要传输的变量的指针)
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;
    // 方向：从内存到外设
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // 传输大小
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    // 外设地址不增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // 内存地址自增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // 外设数据单位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // 内存数据单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // DMA模式，一次或者循环模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    // DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //  优先级：中
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    // 禁止内存到内存的传输
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    // 配置DMA通道
    DMA_Init(USART_TX_DMA_CHANNEL, &DMA_InitStructure);
    // 使能DMA
    DMA_Cmd(USART_TX_DMA_CHANNEL, ENABLE);
}
