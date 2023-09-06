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

void USART(void);
void GPIO_INPUT(void);
void GPIO_OUT(void);
void System_Reset(void);
void delay(int x);
void I2C_EE(void);

int main()
{
    // System_Reset();
    LED_Init();
    EXIT_config();
    USART_Config();
    DEBUG_INFO("spi_test\n\r");
    // DMA_MTM_config();
    // extern uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
    // extern uint32_t aDST_Buffer[BUFFER_SIZE];
    // USARTx_DMA_Config();
    // USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
    I2C_EE();
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
    for (i = 0; i <= 255; i++)
    {
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
        if (i % 16 == 15)
        {
            printf("\n\r");
        }
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
