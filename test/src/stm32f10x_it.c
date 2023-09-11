/**
 ******************************************************************************
 * @file    USART/Printf/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and peripherals
 *          interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "led.h"
#include "exit.h"
#include "systick.h"
#include "usart.h"
#include "includes.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
 * @{
 */

/** @addtogroup USART_Printf
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    DEBUG_INFO("HardFault_Handler");
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
}

// PA0中断输入触发函数
void ExitPA0_Handler(void)
{
    if (EXTI_GetITStatus(EXIT_INPUT_PA0_EXTI_Line) != RESET) {
        GPIO_WriteBit(LED_PORT, LED_PIN_R, RESET);
        GPIO_WriteBit(LED_PORT, LED_PIN_G, SET);
        GPIO_WriteBit(LED_PORT, LED_PIN_B, SET);
    }
}

// PC13中断输入触发函数
void ExitPC13_Handler(void)
{
    if (EXTI_GetITStatus(EXIT_INPUT_PC13_EXTI_Line) != RESET) {
        GPIO_WriteBit(LED_PORT, LED_PIN_R, SET);
        GPIO_WriteBit(LED_PORT, LED_PIN_G, RESET);
        GPIO_WriteBit(LED_PORT, LED_PIN_B, SET);
    }
}

// char data[50];
char data[1];
void DEBUG_USART_IRQHandler(void)
{
    char temp = USART_ReceiveData(DEBUG_USARTx);
    if (temp == '\r') {
        Usart_SendString(DEBUG_USARTx, "\n\r");
        // strcat(data, "\n\r");
        Usart_SendString(DEBUG_USARTx, data);
        Usart_SendString(DEBUG_USARTx, "\n\r");
        if (strcmp(data, "red") == 0) {
            LED_R();
            Usart_SendString(DEBUG_USARTx, "lightRed");
            Usart_SendString(DEBUG_USARTx, "\n\r");
        }else if (strcmp(data, "green") == 0) {
            LED_G();
            Usart_SendString(DEBUG_USARTx, "lightGreen");
            Usart_SendString(DEBUG_USARTx, "\n\r");
        }else if (strcmp(data, "blue") == 0) {
            LED_B();
            Usart_SendString(DEBUG_USARTx, "lightBule");
            Usart_SendString(DEBUG_USARTx, "\n\r");
        }
        data[0] = '\0';
    } else {
        strcat(data, &temp);
        Usart_SendHalfWord(DEBUG_USARTx, temp);
    }
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
