/**
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2022/11/07
 * @brief   ��ADC �ж�
 ******************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������
 * ��̳    :http://www.firebbs.cn
 * �Ա�    :https://fire-stm32.taobao.com
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
 * @brief  ������
 * @param  ��
 * @retval ��
 */
int main(void)
{
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();

  /* ����һ���ַ��� */
  Usart_SendString(DEBUG_USARTx, "����һ�������жϽ��ջ���ʵ��\n");
  printf("��ӭʹ��Ұ��STM32������\n\n\n\n");

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
