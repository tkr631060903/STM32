#include "spi_flash.h"

void SPI_FLASH_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 使能SPI时钟 */
    FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK, ENABLE);
    /* 使能SPI引脚相关的时钟 */
    FLASH_SPI_CS_APBxClock_FUN(FLASH_SPI_CS_CLK, ENABLE);
    FLASH_SPI_SCK_APBxClock_FUN(FLASH_SPI_SCK_CLK, ENABLE);
    FLASH_SPI_MISO_APBxClock_FUN(FLASH_SPI_MISO_CLK, ENABLE);
    FLASH_SPI_MOSI_APBxClock_FUN(FLASH_SPI_MOSI_CLK, ENABLE);
    // FLASH_SPI_CS_APBxClock_FUN(FLASH_SPI_CS_CLK | FLASH_SPI_SCK_CLK | FLASH_SPI_MISO_PIN | FLASH_SPI_MOSI_PIN, ENABLE);
    /* 配置SPI的 CS引脚，普通IO即可 */
    GPIO_InitStructure.GPIO_Pin   = FLASH_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);
    /* 配置SPI的 SCK引脚*/
    GPIO_InitStructure.GPIO_Pin  = FLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);
    /* 配置SPI的 MISO引脚*/
    GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
    GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);
    /* 配置SPI的 MOSI引脚*/
    GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
    GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);
    /* 停止信号 FLASH: CS引脚高电平*/
    SPI_FLASH_CS_HIGH();

    /* SPI 模式配置 */
    // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex; // 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工模式
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                 // 设置SPI操控操地方式:SPI操控操地方式为主操地方式
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                 // 设置SPI数据位数:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(FLASH_SPIx, &SPI_InitStructure);
    /* 使能 SPI  */
    SPI_Cmd(FLASH_SPIx, ENABLE);
}

// 使用SPI发送一个字节的数据
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    /* 双同步信号 */
    SPI_I2S_SendData(FLASH_SPIx, byte);
    while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_TXE) == RESET) {}
    while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_RXNE) == RESET) {}
    return SPI_I2S_ReceiveData(FLASH_SPIx);
}

// 使用SPI读取一个字节的数据
uint8_t SPI_FLASH_ReadByte(uint8_t byte)
{
    return SPI_FLASH_SendByte(Dummy_Byte);
}

// 读取FLASH JEDECID
uint32_t SPI_FLASH_ReadJEDECID(void)
{
    /* 开始通讯：CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 发送JEDEC指令，读取ID */
    SPI_FLASH_SendByte(W25X_JedecDeviceID);
    /* 返回ID */
    uint8_t JEDECID_0 = SPI_FLASH_SendByte(Dummy_Byte);
    uint8_t JEDECID_1 = SPI_FLASH_SendByte(Dummy_Byte);
    uint8_t JEDECID_2 = SPI_FLASH_SendByte(Dummy_Byte);
    /* 停止通讯：CS高电平 */
    SPI_FLASH_CS_HIGH();
    /*把数据组合起来，作为函数的返回值*/
    return (JEDECID_0 << 16) | (JEDECID_1 << 8) | JEDECID_2;
}

// 读取FLASH数据
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /* 开始通讯：CS低电平 */
    SPI_FLASH_CS_LOW();
    // 发送读命令
    SPI_FLASH_SendByte(W25X_ReadData);
    /* 发送 读 地址高位 */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* 发送 读 地址中位 */
    SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
    /* 发送 读 地址低位 */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
    // 读数据
    while (NumByteToRead--) {
        // *pBuffer++ = SPI_FLASH_SendByte(Dummy_Byte);
        *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
        pBuffer++;
    }
    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();
}

// 向FLASH发送 写使能 命令
void SPI_FLASH_WriteEnable(void)
{
    /* 开始通讯：CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 发送写使能命令 */
    SPI_FLASH_SendByte(W25X_WriteEnable);
    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();
}

// 等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
void SPI_FLASH_WaitForWriteEnd(void)
{
    /* 选择 FLASH: CS 低 */
    SPI_FLASH_CS_LOW();
    /* 发送 读状态寄存器 命令 */
    SPI_FLASH_SendByte(W25X_ReadStatusReg);
    /* 若FLASH忙碌，则等待 */
    while ((SPI_FLASH_SendByte(Dummy_Byte) & 0x01) == SET) {}
    /* 停止信号  FLASH: CS 高 */
    SPI_FLASH_CS_HIGH();
}

// 擦除FLASH扇区
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
    /* 发送FLASH写使能命令 */
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    /* 擦除扇区 */
    /* 选择FLASH: CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 发送扇区擦除指令*/
    SPI_FLASH_SendByte(W25X_SectorErase);
    /*发送擦除扇区地址的高位*/
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* 发送擦除扇区地址的中位 */
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* 发送擦除扇区地址的低位 */
    SPI_FLASH_SendByte(SectorAddr & 0xFF);
    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();
    /* 等待擦除完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

// 对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    if (NumByteToWrite > SPI_FLASH_PerWritePageSize) {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
        DEBUG_ERROR("SPI_FLASH_PageWrite too large!");
        return;
    }
    /* 发送FLASH写使能命令 */
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    /* 开始通讯：CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 发送写数据指令 */
    SPI_FLASH_SendByte(W25X_PageProgram);
    /* 发送写地址 */
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(WriteAddr & 0xFF);
    /* 发送数据 */
    while (NumByteToWrite--) {
        SPI_FLASH_SendByte(*pBuffer);
        pBuffer++;
    }
    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();
    /* 等待写入完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
 * @param   pBuffer，要写入数据的指针
 * @param  WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度
 * @retval 无
 */
void SPI_FLASH_BufferWrite(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
    Addr = WriteAddr % SPI_FLASH_PageSize;

    /*差count个数据值，刚好可以对齐到页地址*/
    count = SPI_FLASH_PageSize - Addr;
    /*计算出要写多少整数页*/
    NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
    /*mod运算求余，计算出剩余不满一页的字节数*/
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
    if (Addr == 0) {
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0) {
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        } else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            /*先把整数页都写了*/
            while (NumOfPage--) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    /* 若地址与 SPI_FLASH_PageSize 不对齐  */
    else {
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0) {
            /*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
            if (NumOfSingle > count) {
                temp = NumOfSingle - count;
                /*先写满当前页*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

                WriteAddr += count;
                pBuffer += count;
                /*再写剩余的数据*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            } else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        } else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            /*地址不对齐多出的count分开处理，不加入这个运算*/
            NumByteToWrite -= count;
            NumOfPage   = NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            /* 先写完count个数据，为的是让下一次要写的地址对齐 */
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

            /* 接下来就重复地址对齐的情况 */
            WriteAddr += count;
            pBuffer += count;
            /*把整数页都写了*/
            while (NumOfPage--) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            if (NumOfSingle != 0) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

// 全片擦除
void SPI_FLASH_ChipErase(void)
{
    /* 发送FLASH写使能命令 */
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    /* 选择FLASH: CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 发送扇区擦除指令*/
    SPI_FLASH_SendByte(W25X_ChipErase);
    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();
    /* 等待擦除完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

// 唤醒
void SPI_Flash_WAKEUP(void)
{
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(W25X_ReleasePowerDown);
    SPI_FLASH_CS_HIGH();
}
