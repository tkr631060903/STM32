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
    while (1)
    {
        PAout(0) = ~PAin(0);
        vTaskDelay(500);
    }
}

void Blink_LED1(void *paramPtr)
{
    while (1)
    {
        PAout(1) = ~PAin(1);
        vTaskDelay(3000);
    }
}

void setup()
{
    GPIO_InitTypeDef gpioDef;

    // init gpio
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpioDef.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpioDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &gpioDef);

    PAout(0) = 0;
    PAout(1) = 0;

    // init usart
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

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
