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
#include "util.h"
#include "led.h"
#include "exit.h"
#include "systick.h"
#include "usart.h"
#include "i2c_ee.h"
#include "spi_flash.h"
// #include "diskio.h"
#include "ff.h"

void DMA(void);
void USART(void);
void GPIO_INPUT(void);
void GPIO_OUT(void);
void System_Reset(void);
void delay(int x);
void I2C_EE(void);
void SPI_FLASH(void);
void FatFs(void);

int main()
{
    // System_Reset();
    LED_Init();
    EXIT_config();
    USART_Config();
    DEBUG_INFO("fatfs_test\n\r");
    // DMA();
    // I2C_EE();
    // SPI_FLASH();
    FatFs();
    while (1) {
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
    DMA_MTM_config();
    extern uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
    extern uint32_t aDST_Buffer[BUFFER_SIZE];
    USARTx_DMA_Config();
    USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
}

void USART(void)
{
    Usart_SendByte(DEBUG_USARTx, 't');
    SysTick_Delay_ms(1000);

    Usart_SendHalfWord(DEBUG_USARTx, 'es');
    SysTick_Delay_ms(1000);

    Usart_SendString(DEBUG_USARTx, "test\n");
    SysTick_Delay_ms(1000);
}

void I2C_EE(void)
{
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
    SPI_FLASH_Init();
    uint32_t JEDECID = SPI_FLASH_ReadJEDECID();
    DEBUG_DEBUG("JEDECID = 0x%x\n\r", JEDECID);
    uint32_t SectorAddr = 0x00000;
    DEBUG_DEBUG("SPI_FLASH_SectorErase\n\r");
    SPI_FLASH_SectorErase(SectorAddr);
    // SPI_FLASH_ChipErase();
    uint32_t FLASH_ReadAddress = 0x00000;
    uint16_t BufferSize        = 16;
    uint8_t Rx_Buffer[16];

    SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
    for (uint8_t i = 0; i < BufferSize; i++) {
        DEBUG_DEBUG("0x%02X ", Rx_Buffer[i]);
    }

    uint32_t FLASH_WriteAddress = 0x00000;
    uint8_t Tx_Buffer[16];
    for (uint8_t i = 0; i < BufferSize; i++) {
        Tx_Buffer[i] = i;
    }
    DEBUG_DEBUG("\n\rSPI_FLASH_PageWrite\n\r");
    SPI_FLASH_PageWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
    // SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
    SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
    for (uint8_t i = 0; i < BufferSize; i++) {
        DEBUG_DEBUG("0x%02X ", Rx_Buffer[i]);
    }
}

FATFS fs;             /* FatFs文件系统对象 */
FRESULT res_flash;    /* 文件操作结果 */
BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
void FatFs(void)
{
    res_flash = f_mount(&fs, "1:", 1);
    if (res_flash == FR_NO_FILESYSTEM) {
        printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
        // 格式化
        res_flash = f_mkfs("1:", 0, work, sizeof work);
        if (res_flash == FR_OK) {
            printf("》FLASH已成功格式化文件系统。\r\n");
            /* 格式化后，先取消挂载 */
            res_flash = f_mount(NULL, "1:", 1);
            /* 重新挂载 */
            res_flash = f_mount(&fs, "1:", 1);
        } else {
            LED_R();
            printf("《《格式化失败。》》\r\n");
            while (1) {}
        }
    } else if (res_flash != FR_OK) {
        printf("！！外部Flash挂载文件系统失败。(%d)\r\n", res_flash);
        printf("！！可能原因：SPI Flash初始化不成功。\r\n");
        while (1) {}
    } else {
        printf("》文件系统挂载成功，可以进行读写测试\r\n");
    }
}

// 软件复位函数
void System_Reset(void)
{
    __set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 进行软件复位
}

void delay(int x)
{
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < 1000; j++)
            __NOP();
    }
}
