#include "bsp_led.h"
#include "stm32f10x_it.h"
#include "bsp_systick.h"
#include "stm32f10x_gpio.h"

uint32_t KEY_TOGGLE = 1;

void Delay(uint32_t count)
{
    for (; count != 0; count --);
}

void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = LED_G_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = LED_R_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = LED_B_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    GPIO_SetBits(LED_GPIO_Port, LED_G_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_B_GPIO_PIN);
}

void Light(void)
{
    GPIO_ResetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_B_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_G_GPIO_PIN);
    //Delay(4000000);
    SysTick_Delay_Ms(1000);
    GPIO_ResetBits(LED_GPIO_Port, LED_G_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_B_GPIO_PIN);
    SysTick_Delay_Ms(1000);
    //Delay(4000000);
    GPIO_ResetBits(LED_GPIO_Port, LED_B_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_G_GPIO_PIN);
    GPIO_SetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
    SysTick_Delay_Ms(1000);
    //Delay(4000000);
}

void TOGGLE(void)
{
    if (KEY_TOGGLE == 0)
    {
        GPIO_SetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
        KEY_TOGGLE = 1;
    }
    else
    {
        GPIO_ResetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
        KEY_TOGGLE = 0;
    }
}
