/**
 * **********************************************
 *
 * STM32F103 FreeRTOS blink demo
 *
 * MCU: STM32F103C8
 * FREQ: 72MHz
 * PIN: PA0, PA1
 * USART: uart1, PA9, PA10
 *
 * **********************************************
 */

#include "stm32f10x.h"
#include "BITBAND.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define TASK_STACK_SIZE 256U

void Blink_LED0(void *paramPtr)
{
    while (1) {
        PBout(0) = ~PBout(0);
        USART_SendData(USART1, '0');
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
        vTaskDelay(500);
    }
}

void Blink_LED1(void *paramPtr)
{
    while (1) {
        PBout(1) = ~PBout(1);
        USART_SendData(USART1, '1');
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
        vTaskDelay(3000);
    }
}

void setup()
{
    GPIO_InitTypeDef gpioDef;

    // init gpio
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    gpioDef.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpioDef.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
    gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpioDef);

    PBout(0) = 0;
    PBout(1) = 0;

    // init usart
    // 打开GPIO串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 打开串口外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    // 将USART TX配置为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 将USART RX配置为浮空输入
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置串口工作参数
    USART_InitTypeDef USART_InitStructure;
    // 波特率
    USART_InitStructure.USART_BaudRate = 115200;
    // 数据长度
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    // 停止位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    // 校验位
    USART_InitStructure.USART_Parity = USART_Parity_No;
    // 硬件流控
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    // 工作模式,收发一起
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    // 使能串口
    USART_Cmd(USART1, ENABLE);

    xTaskCreate(Blink_LED0, "Blinker_0", TASK_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(Blink_LED1, "Blinker_1", TASK_STACK_SIZE, NULL, 2, NULL);
}

int main()
{
    setup();
    vTaskStartScheduler();
    while (1)
        ;
}
