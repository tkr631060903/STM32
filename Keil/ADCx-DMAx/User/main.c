/**
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2022/11/07
 * @brief   ��ADC��ͨ��
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

extern __IO uint32_t ADC_ConvertedValue[NOFCHANNEL];
float ADC_ConvertedValueLocal[NOFCHANNEL * 2];

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
  uint16_t temp0 = 0, temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0;
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();

  /* ����һ���ַ��� */
  Usart_SendString(DEBUG_USARTx, "����һ�������жϽ��ջ���ʵ��\n");
  printf("��ӭʹ��Ұ��STM32������\n\n\n\n");

  ADCx_Init();

  while (1)
  {
    temp0 = (ADC_ConvertedValue[0] & 0XFFFF0000) >> 16; //ȡ��ADC1���ݼĴ�����16λ�������ADC2��ת������
    temp1 = (ADC_ConvertedValue[0] & 0XFFFF);           //ȡ��ADC1���ݼĴ�����16λ�������ADC1��ת������
    temp2 = (ADC_ConvertedValue[1] & 0XFFFF0000) >> 16; //ȡ��ADC1���ݼĴ�����16λ�������ADC2��ת������
    temp3 = (ADC_ConvertedValue[1] & 0XFFFF);           //ȡ��ADC1���ݼĴ�����16λ�������ADC1��ת������
    temp4 = (ADC_ConvertedValue[2] & 0XFFFF0000) >> 16; //ȡ��ADC1���ݼĴ�����16λ�������ADC2��ת������
    temp5 = (ADC_ConvertedValue[2] & 0XFFFF);           //ȡ��ADC1���ݼĴ�����16λ�������ADC1��ת������

    ADC_ConvertedValueLocal[0] = (float)temp0 / 4096 * 3.3;
    ADC_ConvertedValueLocal[1] = (float)temp1 / 4096 * 3.3;
    ADC_ConvertedValueLocal[2] = (float)temp2 / 4096 * 3.3;
    ADC_ConvertedValueLocal[3] = (float)temp3 / 4096 * 3.3;
    ADC_ConvertedValueLocal[4] = (float)temp4 / 4096 * 3.3;
    ADC_ConvertedValueLocal[5] = (float)temp5 / 4096 * 3.3;

    // printf("\r\n The Value = 0x%04X \r\n", ADC_ConvertedValue);
    printf("\r\n ADC2 PC0 Value = %f V \r\n", ADC_ConvertedValueLocal[4]);
    printf("\r\n ADC1 PC1 Value = %f V \r\n", ADC_ConvertedValueLocal[1]);
    printf("\r\n ADC1 PC2 Value = %f V \r\n", ADC_ConvertedValueLocal[3]);
    printf("\r\n ADC1 PC3 Value = %f V \r\n", ADC_ConvertedValueLocal[5]);
    printf("\r\n ADC2 PC4 Value = %f V \r\n", ADC_ConvertedValueLocal[0]);
    printf("\r\n ADC2 PC5 Value = %f V \r\n", ADC_ConvertedValueLocal[2]);

    printf("\r\n\r\n");

    Delay(5000000);
  }
}
/*********************************************END OF FILE**********************/
