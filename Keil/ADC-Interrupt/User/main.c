/**
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2022/11/07
 * @brief   单ADC 中断
 ******************************************************************************
 * @attention
 *
 * 实验平台:野火 F103-指南者 STM32 开发板
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :https://fire-stm32.taobao.com
 *
 ******************************************************************************
 */

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "includes.h"

extern __IO uint16_t ADC_ConvertedValue;
float ADC_ConvertedValueLocal;

void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--)
    ;
}

/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */
int main(void)
{
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();

  /* 发送一个字符串 */
  Usart_SendString(DEBUG_USARTx, "这是一个串口中断接收回显实验\n");
  printf("欢迎使用野火STM32开发板\n\n\n\n");

  ADCx_Init();

  while (1)
  {
    ADC_ConvertedValueLocal = (float)ADC_ConvertedValue / 4096 * 3.3;

    printf("\r\n The Value = 0x%04X \r\n", ADC_ConvertedValue);
    printf("\r\n The Value = %f V \r\n", ADC_ConvertedValueLocal);
    printf("\r\n\r\n");

    Delay(5000000);
  }
}
/*********************************************END OF FILE**********************/
