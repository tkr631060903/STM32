#ifndef _EXIT_H
#define _EXIT_H

#include "stm32f10x.h"

#define EXIT_INPUT_PA0                      GPIO_Pin_0
#define EXIT_INPUT_A_PORT                   GPIOA
#define EXIT_INPUT_PC13                     GPIO_Pin_13
#define EXIT_INPUT_C_PORT                   GPIOC
#define EXIT_INPUT_PA0_IRQ                  EXTI0_IRQn
#define EXIT_INPUT_PC13_IRQ                 EXTI15_10_IRQn
#define EXIT_INPUT_PA0_CLOCK                (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)
#define EXIT_INPUT_PC13_CLOCK               (RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO)
#define EXIT_INPUT_PA0_GPIO_PortSourceGPIO  GPIO_PortSourceGPIOA
#define EXIT_INPUT_PC13_GPIO_PortSourceGPIO GPIO_PortSourceGPIOC
#define EXIT_INPUT_PA0_GPIO_PinSource       GPIO_PinSource0
#define EXIT_INPUT_PC13_GPIO_PinSource      GPIO_PinSource13
#define EXIT_INPUT_PA0_EXTI_Line            EXTI_Line0
#define EXIT_INPUT_PC13_EXTI_Line           EXTI_Line13

void EXIT_config(void);

#define ExitPA0_Handler  EXTI0_IRQHandler

#define ExitPC13_Handler EXTI15_10_IRQHandler

#endif // !_EXIT_H
