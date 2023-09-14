#include "led.h"

// LED初始化
void LED_Init(void)
{
    // GPIO配置输出
    GPIO_InitTypeDef gpioDef;
    RCC_APB2PeriphClockCmd(LED_PERIPH, ENABLE);
    gpioDef.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpioDef.GPIO_Pin   = LED_PIN_G;
    gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &gpioDef);
    gpioDef.GPIO_Pin = LED_PIN_B;
    GPIO_Init(LED_PORT, &gpioDef);
    gpioDef.GPIO_Pin = LED_PIN_R;
    GPIO_Init(LED_PORT, &gpioDef);
    // GPIO PA0配置输入
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpioDef.GPIO_Mode  = GPIO_Mode_IPU;
    gpioDef.GPIO_Pin   = GPIO_Pin_0;
    gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &gpioDef);
    // GPIO PC13配置输入
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    gpioDef.GPIO_Mode  = GPIO_Mode_IPU;
    gpioDef.GPIO_Pin   = GPIO_Pin_13;
    gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &gpioDef);
    GPIO_WriteBit(LED_PORT, LED_PIN_R, SET);
    GPIO_WriteBit(LED_PORT, LED_PIN_G, SET);
    GPIO_WriteBit(LED_PORT, LED_PIN_B, RESET);
}

void LED_R(void)
{
    GPIO_WriteBit(LED_PORT, LED_PIN_R, RESET);
    GPIO_WriteBit(LED_PORT, LED_PIN_G, SET);
    GPIO_WriteBit(LED_PORT, LED_PIN_B, SET);
}

void LED_G(void)
{
    GPIO_WriteBit(LED_PORT, LED_PIN_R, SET);
    GPIO_WriteBit(LED_PORT, LED_PIN_G, RESET);
    GPIO_WriteBit(LED_PORT, LED_PIN_B, SET);
}

void LED_B(void)
{
    GPIO_WriteBit(LED_PORT, LED_PIN_R, SET);
    GPIO_WriteBit(LED_PORT, LED_PIN_G, SET);
    GPIO_WriteBit(LED_PORT, LED_PIN_B, RESET);
}

// 按钮输入
void btn_Input(void)
{
    uint8_t PA0  = 0;
    uint8_t PC13 = 0;
    PA0          = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    PC13         = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
    if (PA0 == 1) {
        GPIO_WriteBit(LED_PORT, LED_PIN_R, RESET);
        GPIO_WriteBit(LED_PORT, LED_PIN_G, SET);
        GPIO_WriteBit(LED_PORT, LED_PIN_B, SET);
    }
    if (PC13 == 1) {
        GPIO_WriteBit(LED_PORT, LED_PIN_R, SET);
        GPIO_WriteBit(LED_PORT, LED_PIN_G, SET);
        GPIO_WriteBit(LED_PORT, LED_PIN_B, RESET);
    }
    GPIO_WriteBit(LED_PORT, LED_PIN_R, SET);
    GPIO_WriteBit(LED_PORT, LED_PIN_G, RESET);
    GPIO_WriteBit(LED_PORT, LED_PIN_B, SET);
}
