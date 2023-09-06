#include "i2c_ee.h"

// I2C GPIO引脚的初始化函数
static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能与 I2C 有关的时钟 */
    EEPROM_I2C_APBxClock_FUN(EEPROM_I2C_CLK, ENABLE);
    EEPROM_I2C_GPIO_APBxClock_FUN(EEPROM_I2C_GPIO_CLK, ENABLE);

    /* I2C_SCL、I2C_SDA*/
    GPIO_InitStructure.GPIO_Pin   = EEPROM_I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD; // 开漏输出
    GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = EEPROM_I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD; // 开漏输出
    GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);
}

// I2C 工作模式配置
static void I2C_Mode_Config(void)
{
    I2C_InitTypeDef I2C_InitStructure;

    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

    /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;

    I2C_InitStructure.I2C_OwnAddress1 = I2Cx_OWN_ADDRESS7;
    I2C_InitStructure.I2C_Ack         = I2C_Ack_Enable;

    /* I2C的寻址模式 */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    /* 通信速率 */
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

    /* I2C 初始化 */
    I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);

    /* 使能 I2C */
    I2C_Cmd(EEPROM_I2Cx, ENABLE);
}

// I2C 外设(EEPROM)初始化
void EEPROM_I2C_Init(void)
{
    I2C_GPIO_Config();

    I2C_Mode_Config();
}

// 写一个字节到EEPROM
uint8_t EEPROM_WriteByte(uint8_t addr, uint8_t data)
{
    uint32_t timeout = 0;
    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY)) {}
    /* 产生I2C起始信号 */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* 检测 EV5 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}
    /* 发送EEPROM设备地址 */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* 检测 EV6 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    /* 发送要写入的EEPROM内部地址(即EEPROM内部存储器的地址) */
    I2C_SendData(EEPROM_I2Cx, addr);
    /* 检测 EV8 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
    /* 发送一字节要写入的数据 */
    I2C_SendData(EEPROM_I2Cx, data);
    /* 检测 EV8 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
    /* 发送停止信号 */
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
    DEBUG_DEBUG("write data 0x%x\r\n", data);
    return 1;
}

// 在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数不能超过EEPROM页的大小，AT24C02每页有8个字节
uint8_t I2C_EE_PageWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
    if (NumByteToWrite > 8 || WriteAddr % 8 != 0) {
        return 0;
    }
    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY)) {}
    /* 产生I2C起始信号 */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* 检测 EV5 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}
    /* 发送EEPROM设备地址 */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* 检测 EV6 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    /* 发送要写入的EEPROM内部地址(即EEPROM内部存储器的地址) */
    I2C_SendData(EEPROM_I2Cx, WriteAddr);
    /* 检测 EV8 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
    while (NumByteToWrite) {
        /* 发送一字节要写入的数据 */
        I2C_SendData(EEPROM_I2Cx, *pBuffer);
        while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
        pBuffer++;
        NumByteToWrite--;
    }
    /* 发送停止信号 */
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
    return 1;
}

// 连续写多个字节
uint8_t I2C_EE_BufferWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
    uint8_t firstNumByteToWrite = 8 - (WriteAddr % 8);
    I2C_EE_PageWrite(pBuffer, WriteAddr, firstNumByteToWrite);
    WriteAddr += firstNumByteToWrite;
    NumByteToWrite -= firstNumByteToWrite;
    DEBUG_DEBUG("%d",firstNumByteToWrite);
    DEBUG_DEBUG("%d",WriteAddr);
    DEBUG_DEBUG("%d",NumByteToWrite);
    // while (NumByteToWrite) {
    //     I2C_EE_PageWrite(pBuffer, WriteAddr, 8);
    // }
    
    // while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY)) {}
    // I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    // while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}
    // I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    // while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    // I2C_SendData(EEPROM_I2Cx, WriteAddr);
    // while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
    // while (NumByteToWrite) {
    //     I2C_SendData(EEPROM_I2Cx, *pBuffer);
    //     while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
    //     pBuffer++;
    //     NumByteToWrite--;
    // }
    // I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
    // return 1;
}

// 从EEPROM读一个字节
uint8_t EEPROM_ReadByte(uint8_t addr)
{
    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY)) {}
    /* 产生I2C起始信号 */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* 检测 EV5 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}
    /* 发送EEPROM设备地址 */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* 检测 EV6 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    /*通过重新设置PE位清除EV6事件 */
    I2C_Cmd(EEPROM_I2Cx, ENABLE);
    /* 发送要写入的EEPROM内部地址(即EEPROM内部存储器的地址) */
    I2C_SendData(EEPROM_I2Cx, addr);
    /* 检测 EV8 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
    /* 产生I2C起始信号 */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* 检测 EV5 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}
    /* 发送EEPROM设备地址 */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);
    /* 检测 EV6 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
    /* 检测 EV7 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    // 发送非应答信号
    I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);
    // 获取EEPROM数据
    uint8_t data = I2C_ReceiveData(EEPROM_I2Cx);
    DEBUG_DEBUG("read data 0x%x\r\n", data);
    // 发送停止信号
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
    // 恢复应答信号
    I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);
    return data;
}

// 连续读EEPROM数据
uint8_t I2C_EE_BufferRead(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY)) {}
    /* 产生I2C起始信号 */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* 检测 EV5 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}
    /* 发送EEPROM设备地址 */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* 检测 EV6 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    /*通过重新设置PE位清除EV6事件 */
    I2C_Cmd(EEPROM_I2Cx, ENABLE);
    /* 发送要写入的EEPROM内部地址(即EEPROM内部存储器的地址) */
    I2C_SendData(EEPROM_I2Cx, ReadAddr);
    /* 检测 EV8 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {}
    /* 产生I2C起始信号 */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* 检测 EV5 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {}
    /* 发送EEPROM设备地址 */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);
    /* 检测 EV6 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
    /* 检测 EV7 事件并清除标志*/
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
    while (NumByteToRead) {
        /*若NumByteToRead=1，表示已经接收到最后一个数据了，发送非应答信号，结束传输*/
        if (NumByteToRead == 1) {
            // 发送非应答信号
            I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);
            // 发送停止信号
            I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
        }
        while (I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == 0) {}
        {
            /*通过I2C，从设备中读取一个字节的数据 */
            *pBuffer = I2C_ReceiveData(EEPROM_I2Cx);
            /* 存储数据的指针指向下一个地址 */
            pBuffer++;
            /* 接收数据自减 */
            NumByteToRead--;
        }
    }
    // 恢复应答信号
    I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);
    return 1;
}
