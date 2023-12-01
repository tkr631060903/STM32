/*
 * ************************************************
 *
 * STM32 blink demo
 *
 * CPU:     STM32F103C8
 * PIN:     PA1
 *
 * ************************************************
 */

#include "stm32f10x.h"
#include "includes.h"
#include "ff.h"
#include "led.h"
#include "exit.h"
#include "systick.h"
#include "usart.h"
#include "i2c_ee.h"
#include "spi_flash.h"
#include "adc.h"
#include "TIMbase.h"
#include "sdio_sdcard.h"
#include "SD_test.h"
#include "can.h"

void delay(int x);
void System_Reset(void);
void DMA(void);
void USART(void);
void GPIO_INPUT(void);
void GPIO_OUT(void);
void I2C_EE(void);
void SPI_FLASH(void);
void SPI_FLASH_FatFs(void);
void ADC(void);
void TIM_base(void);
void SDIO_SDCARD(void);
void SDIO_SDCARD_FatFs(void);
void CAN(void);

int main()
{
    // System_Reset();
    LED_Init();
    EXIT_config();
    USART_Config();
    // DMA();
    I2C_EE();
    // SPI_FLASH();
    // SPI_FLASH_FatFs();
    // ADC();
    // TIM_base();
    // SDIO_SDCARD();
    // SDIO_SDCARD_FatFs();
    // CAN();
    while (1) {
    }
}

void delay(int x)
{
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < 1000; j++)
            __NOP();
    }
}

void GPIO_OUT(void)
{
    GPIO_WriteBit(LED_PORT, LED_PIN_G, (BitAction)!GPIO_ReadInputDataBit(LED_PORT, LED_PIN_G));
    uint8_t bitstatus = GPIO_ReadInputDataBit(LED_PORT, LED_PIN_G);
    SysTick_Delay_ms(1000);
    GPIO_WriteBit(LED_PORT, LED_PIN_B, (BitAction)!GPIO_ReadInputDataBit(LED_PORT, LED_PIN_B));
    SysTick_Delay_ms(1000);
    GPIO_WriteBit(LED_PORT, LED_PIN_R, (BitAction)!GPIO_ReadInputDataBit(LED_PORT, LED_PIN_R));
    SysTick_Delay_ms(1000);
}

void GPIO_INPUT(void)
{
    btn_Input();
}

void DMA(void)
{
    DEBUG_INFO("DMA_Test\n\r");
    DMA_MTM_config();
    extern uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
    extern uint32_t aDST_Buffer[BUFFER_SIZE];
    USARTx_DMA_Config();
    USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
}

void USART(void)
{
    DEBUG_INFO("USART_Test\n\r");
    Usart_SendByte(DEBUG_USARTx, 't');
    SysTick_Delay_ms(1000);

    Usart_SendHalfWord(DEBUG_USARTx, 'es');
    SysTick_Delay_ms(1000);

    Usart_SendString(DEBUG_USARTx, "test\n");
    SysTick_Delay_ms(1000);
}

void I2C_EE(void)
{
    DEBUG_INFO("I2C_EEPROM_Test\n\r");
    EEPROM_I2C_Init();
    // EEPROM_WriteByte(0x01, 0x50);
    // uint8_t data = EEPROM_ReadByte(0x01);
    // DEBUG_DEBUG("read data 0x%x\r\n", data);
    uint8_t I2c_Buf_Write[256];
    uint8_t I2c_Buf_Read[256];
    uint16_t i;
    for (i = 0; i <= 255; i++) {
        I2c_Buf_Write[i] = i;
        // printf("0x%02X ", I2c_Buf_Write[i]);
        // if (i % 16 == 15)
        // {
        //     printf("\n\r");
        // }
    }
    I2C_EE_PageWrite(I2c_Buf_Write, 8, 8);
    // I2C_EE_BufferWrite(I2c_Buf_Write, 9, 12);
    SysTick_Delay_ms(2);
    I2C_EE_BufferRead(I2c_Buf_Read, 0x00, 256);
    for (int i = 0; i < 256; i++) {
        printf("0x%02X ", I2c_Buf_Read[i]);
        if (i % 16 == 15) {
            printf("\n\r");
        }
    }
}

void SPI_FLASH(void)
{
    DEBUG_INFO("SPI_FLASH_Test\n\r");
    SPI_FLASH_Init();
    uint32_t JEDECID = SPI_FLASH_ReadJEDECID();
    DEBUG_DEBUG("JEDECID = 0x%x\n\r", JEDECID);
    uint32_t SectorAddr = 0x00000;
    DEBUG_DEBUG("SPI_FLASH_SectorErase\n\r");
    SPI_FLASH_SectorErase(SectorAddr);
    // SPI_FLASH_ChipErase();
    uint32_t FLASH_ReadAddress = 0x00000;
    uint16_t BufferSize        = 64;
    uint8_t Rx_Buffer[64];

    SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
    for (uint8_t i = 0; i < BufferSize; i++) {
        DEBUG_DEBUG("0x%02X ", Rx_Buffer[i]);
    }

    uint32_t FLASH_WriteAddress = 0x00000;
    uint8_t Tx_Buffer[64];
    for (uint8_t i = 0; i < BufferSize; i++) {
        Tx_Buffer[i] = i;
    }
    DEBUG_DEBUG("\n\rSPI_FLASH_PageWrite\n\r");
    // SPI_FLASH_PageWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
    SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
    SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
    for (uint8_t i = 0; i < BufferSize; i++) {
        DEBUG_DEBUG("0x%02X ", Rx_Buffer[i]);
    }
}

FATFS fs;                                             /* FatFs文件系统对象 */
FRESULT res;                                          /* 文件操作结果 */
BYTE work[FF_MAX_SS];                                 /* Work area (larger is better for processing time) */
FIL file;                                             /* 文件对象 */
UINT fnum;                                            /* 文件成功读写数量 */
BYTE ReadBuffer[1024] = {0};                          /* 读缓冲区 */
BYTE WriteBuffer[]    = "我是写入数据,我是写入数据."; /* 写缓冲区*/
void SPI_FLASH_FatFs(void)
{
    DEBUG_INFO("SPI_FLASH_FatFs_Test\n\r");
    // SPI_FLASH_Init();
    // SPI_FLASH_ChipErase();
    res = f_mount(&fs, "1:", 1);
    if (res == FR_NO_FILESYSTEM) {
        printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
        // 格式化
        res = f_mkfs("1:", 0, work, sizeof work);
        if (res == FR_OK) {
            printf("》FLASH已成功格式化文件系统。\r\n");
            /* 格式化后，先取消挂载 */
            res = f_mount(NULL, "1:", 1);
            printf("》取消挂载成功。%d\r\n", res);
            /* 重新挂载 */
            res = f_mount(&fs, "1:", 1);
            printf("》重新挂载成功。%d\r\n", res);
        } else {
            LED_R();
            printf("《《格式化失败。》》\r\n");
            while (1) {}
        }
    } else if (res != FR_OK) {
        printf("！！外部Flash挂载文件系统失败。(%d)\r\n", res);
        while (1) {}
    } else {
        printf("》文件系统挂载成功，可以进行读写测试\r\n");
    }
    /* 打开文件，每次都以新的形式打开，属性为可写 */
    printf("\r\n****** 即将进行文件写入测试... ******\r\n");
    res = f_open(&file, "1:hello.txt", FA_OPEN_ALWAYS | FA_WRITE);
    // res = f_open(&file, "1:hello.txt", FA_CREATE_ALWAYS | FA_WRITE );
    if (res == FR_OK) {
        printf("》打开文件成功。%d\r\n", res);
        res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
        if (res == FR_OK) {
            printf("》文件写入成功,写入字节数据:%d\r\n", fnum);
            printf("》写入完戯后的文件数据为：%s \r\n", WriteBuffer);
        } else {
            printf("！！文件写入失败：(%d)\n", res);
            return;
        }
        /* 不再读写，关闭文件 */
        f_close(&file);
    } else {
        printf("》打开文件失败。错误代码:%d\r\n", res);
        return;
    }
    printf("\r\n****** 即将进行文件读取测试... ******\r\n");
    res = f_open(&file, "1:hello.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    if (res == FR_OK) {
        printf("》打开文件成功。%d\r\n", res);
        res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (res == FR_OK) {
            printf("》文件读取成功,读到字节数据：%d\r\n", fnum);
            printf("》读取得的文件数据为：%s \r\n", ReadBuffer);
        } else {
            printf("！！文件读取失败：(%d)\n", res);
            return;
        }
    } else {
        printf("》打开文件失败。错误代码:%d\r\n", res);
        return;
    }
    /* 不再读写，关闭文件 */
    f_close(&file);
    printf("\r\n****** 即将进行文件内容新增测试... ******\r\n");
    res = f_open(&file, "1:hello.txt", FA_OPEN_APPEND | FA_WRITE);
    if (res == FR_OK) {
        printf("》打开文件成功。%d\r\n", res);
        f_close(&file);

        /* 打开文件 */
        res = f_open(&file, "1:hello.txt", FA_WRITE);
        /* 移动读/写指针 */
        res = f_lseek(&file, f_size(&file)); // 方式2：打开后，根据文件字节大小移动读/写指针
        /* 写入数据 */
        res = f_write(&file, "2,bb,25\n", sizeof("2,bb,25\n"), &fnum);
        /* 关闭文件 */
        f_close(&file);
        /* 写入数据 */
        printf("f_write:%d\r\n", res);
        res = f_open(&file, "1:hello.txt", FA_READ);
        res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
        printf("》文件读取成功,读到字节数据：%d\r\n", fnum);
        printf("》读取得的文件数据为:");
        for (int i = 0; i < fnum; i++) {
            Usart_SendByte(DEBUG_USARTx, ReadBuffer[i]);
        }
    } else {
        printf("》打开文件失败。错误代码:%d\r\n", res);
        return;
    }
    printf("\r\n****** 即将进行文件删除测试... ******\r\n");
    // 打开文件
    res = f_open(&file, "1:hello.txt", FA_READ | FA_WRITE);
    if (res == FR_OK) {
        // 删除文件
        res = f_unlink("1:hello.txt");
        if (res == FR_OK) {
            printf("》文件删除成功。%d\r\n", res);
        } else {
            printf("》文件删除失败。%d\r\n", res);
        }
        // 关闭文件
        f_close(&file);
    } else {
        printf("》文件打开失败。%d\r\n", res);
    }

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL, "1:", 1);
}

extern __IO uint32_t ADC_ConvertedValue[NOFCHANEL];
void ADC(void)
{
    DEBUG_INFO("ADC_Test\n\r");
    ADCx_Init();
    uint16_t ADCx_1_Channel_1_temp = 0, ADCx_1_Channel_2_temp = 0, ADCx_2_Channel_1_temp = 0, ADCx_2_Channel_2_temp = 0;
    // 用于保存计算完成后的电压值
    float ADC_ConvertedValueLocal[NOFCHANEL * 2];
    while (1) {
        ADCx_1_Channel_1_temp = ADC_ConvertedValue[0] & 0xffff;
        ADCx_1_Channel_2_temp = ADC_ConvertedValue[1] & 0xffff;
        ADCx_2_Channel_1_temp = (ADC_ConvertedValue[0] & 0xffff0000) >> 16;
        ADCx_2_Channel_2_temp = (ADC_ConvertedValue[1] & 0xffff0000) >> 16;

        ADC_ConvertedValueLocal[0] = (float)ADCx_1_Channel_1_temp / 4096 * 3.3;
        ADC_ConvertedValueLocal[1] = (float)ADCx_1_Channel_2_temp / 4096 * 3.3;
        ADC_ConvertedValueLocal[2] = (float)ADCx_2_Channel_1_temp / 4096 * 3.3;
        ADC_ConvertedValueLocal[3] = (float)ADCx_2_Channel_2_temp / 4096 * 3.3;

        printf("ADCx_1_CHANNEL_1 = %f V\r\n", ADC_ConvertedValueLocal[0]);
        printf("ADCx_1_CHANNEL_2 = %f V\r\n", ADC_ConvertedValueLocal[1]);
        printf("ADCx_2_CHANNEL_1 = %f V\r\n", ADC_ConvertedValueLocal[2]);
        printf("ADCx_2_CHANNEL_2 = %f V\r\n", ADC_ConvertedValueLocal[3]);
        printf("\r\n");
        SysTick_Delay_ms(1000);
    }
}

volatile uint32_t time = 0;
void TIM_base(void)
{
    DEBUG_INFO("TIM_base_Test\n\r");
    BASIC_TIM_Init();
    while (1) {
        if (time == 1000) {
            /* 1000 * 1 ms = 1s 时间到 */
            time = 0;
            printf("1s 时间到\r\n");
        }
    }
}

void SDIO_SDCARD(void)
{
    DEBUG_INFO("SDIO_SDCARD_Test\n\r");
    SD_Test();
}

void SDIO_SDCARD_FatFs(void)
{
    DEBUG_INFO("SDIO_SDCARD_FatFs_Test\n\r");
    // 文件系统挂载时会对SD卡初始化
    res = f_mount(&fs, "0:", 1);
    /*----------------------- 格式化测试 ---------------------------*/
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if (res == FR_NO_FILESYSTEM) {
        printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
        /* 格式化 */
        res = f_mkfs("0:", 0, work, sizeof work);
        if (res == FR_OK) {
            printf("》SD卡已成功格式化文件系统。\r\n");
            /* 格式化后，先取消挂载 */
            res = f_mount(NULL, "0:", 1);
            /* 重新挂载 */
            res = f_mount(&fs, "0:", 1);
        } else {
            printf("《《格式化失败。》》\r\n");
            while (1)
                ;
        }
    } else if (res != FR_OK) {
        printf("！！SD卡挂载文件系统失败。(%d)\r\n", res);
        printf("！！可能原因：SD卡初始化不成功。\r\n");
        while (1)
            ;
    } else {
        printf("》文件系统挂载成功，可以进行读写测试\r\n");
    }
    /*--------------------- 文件系统测试：写测试 -----------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("\r\n****** 即将进行文件写入测试... ******\r\n");
    res = f_open(&file, "0:hello.txt", FA_CREATE_ALWAYS | FA_WRITE);
    // res = f_open(&file, "0:hello.txt", FA_OPEN_ALWAYS | FA_WRITE);
    if (res == FR_OK) {
        printf("》打开/hello.txt文件成功，向文件写入数据。\r\n");
        /* 将指定存储区内容写入到文件内 */
        res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
        if (res == FR_OK) {
            printf("》文件写入成功，写入字节数据：%d\n", fnum);
            printf("》向文件写入的数据为：\r\n%s\r\n", WriteBuffer);
        } else {
            printf("！！文件写入失败：(%d)\n", res);
        }
        /* 不再读写，关闭文件 */
        f_close(&file);
    } else {
        printf("！！打开/创建文件失败。\r\n");
    }
    /*------------------ 文件系统测试：读测试 --------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    res = f_open(&file, "0:hello.txt", FA_OPEN_EXISTING | FA_READ);
    if (res == FR_OK) {
        printf("》打开文件成功。\r\n");
        res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (res == FR_OK) {
            printf("》文件读取成功,读到字节数据：%d\r\n", fnum);
            printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
        } else {
            printf("！！文件读取失败：(%d)\n", res);
        }
    } else {
        printf("！！打开文件失败。\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&file);

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL, "0:", 1);
}

// RAM空间不够且没有使用CAN,可注释CAN相关代码
CanRxMsg CAN_Rece_Data;
CanTxMsg CAN_Tran_Data;
uint8_t flag = 0;
void CAN(void)
{
    DEBUG_INFO("CAN_Test\n\r");
    CAN_Config();
    while (1) {
        uint8_t box;
        CAN_Tran_Data.StdId   = 0;
        CAN_Tran_Data.ExtId   = PASS_ID;
        CAN_Tran_Data.RTR     = CAN_RTR_Data;
        CAN_Tran_Data.IDE     = CAN_Id_Extended;
        CAN_Tran_Data.DLC     = 5;  // 配置发送的数据帧
        CAN_Tran_Data.Data[0] = 255;
        CAN_Tran_Data.Data[1] = 254;
        CAN_Tran_Data.Data[2] = 253;
        CAN_Tran_Data.Data[3] = 252;
        CAN_Tran_Data.Data[4] = 251;
        CAN_Tran_Data.Data[5] = 250;
        CAN_Tran_Data.Data[6] = 249;
        CAN_Tran_Data.Data[7] = 248;

        box = CAN_Transmit(CAN1, &CAN_Tran_Data);
        while (CAN_TransmitStatus(CAN1, box) == CAN_TxStatus_Failed)
            ;
        printf("\r\n数据发送完成\r\n");
        if (flag == 1) {
            for (uint8_t i = 0; i < sizeof(CAN_Tran_Data.Data); i++)
            {
                printf("\r\n接收到的数据为:%d\r\n", CAN_Rece_Data.Data[i]);
            }
            flag = 0;
        }
        SysTick_Delay_ms(1000);
    }
}
