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
#include "led.h"
#include "exit.h"
#include "systick.h"
#include "usart.h"

void USART(void);
void GPIO_INPUT(void);
void GPIO_OUT(void);
void System_Reset(void);

void delay(int x)
{
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < 1000; j++)
            __NOP();
    }
}

int main()
{
    // System_Reset();
    LED_Init();
    EXIT_config();
    USART_Config();
    DMA_MTM_config();
    extern uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
    extern uint32_t aDST_Buffer[BUFFER_SIZE];
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
    Usart_SendByte(DEBUG_USARTx, 'g');
    SysTick_Delay_ms(1000);

    Usart_SendHalfWord(DEBUG_USARTx, 'us');
    SysTick_Delay_ms(1000);

    Usart_SendString(DEBUG_USARTx, "测试测试\n");
    SysTick_Delay_ms(1000);
}

//软件复位函数
void System_Reset(void) {
	__set_FAULTMASK(1); //关闭所有中断
	NVIC_SystemReset(); //进行软件复位
}

