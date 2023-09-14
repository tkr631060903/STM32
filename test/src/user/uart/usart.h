#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

// 串口1
#define DEBUG_USARTx           USART1
#define DEBUG_USART_CLK        RCC_APB2Periph_USART1
#define DEBUG_USART_APBxClkCmd RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRATE   115200

// USART GPIO 引脚宏定义
#define DEBUG_USART_GPIO_CLK        (RCC_APB2Periph_GPIOA)
#define DEBUG_USART_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd

#define DEBUG_USART_TX_GPIO_PORT    GPIOA
#define DEBUG_USART_TX_GPIO_PIN     GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT    GPIOA
#define DEBUG_USART_RX_GPIO_PIN     GPIO_Pin_10

#define DEBUG_USART_IRQ             USART1_IRQn
#define DEBUG_USART_IRQHandler      USART1_IRQHandler

// 调试信息输出
#define DEBUG_ON                 1
#define DEBUG_INFO(fmt, arg...)  printf("<<-DEBUG-INFO->> " fmt "\n", ##arg)
#define DEBUG_ERROR(fmt, arg...) printf("<<-DEBUG-ERROR->> " fmt "\n", ##arg)
#define DEBUG_DEBUG(fmt, arg...)                                                \
    do {                                                                        \
        if (DEBUG_ON)                                                           \
            printf("<<-DEBUG->> [%s][%d]" fmt "\n", __FILE__, __LINE__, ##arg); \
    } while (0)

void NVIC_config_Usart(void);
void USART_Config(void);
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);

#endif // !__USART_H