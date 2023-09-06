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
    FLASH_SPI_CS_HIGH();

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
}

// 使用SPI发送一个字节的数据
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    /* 双同步信号 */
    SPI_I2S_SendData(FLASH_SPIx, byte);
    while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(FLASH_SPIx);
}
