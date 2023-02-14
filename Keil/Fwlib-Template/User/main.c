#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_exti.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "stm32f10x_it.h"
#include "bsp_dma_mtom.h"
#include "stm32f10x_dma.h"
#include "bsp_dma_mtop.h"
#include "stm32f10x_usart.h"

int main(void)
{
  // uint8_t a[10]={1,2,3,4,5,6,7,8,9,10};

  // uint8_t ch;
  extern char ucTemp;
  uint8_t status = 0;

  extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
  extern uint32_t aDST_Buffer[BUFFER_SIZE];
  extern uint8_t SendBuff[SENDBUFF_SIZE];

  for (uint16_t i = 0; i < SENDBUFF_SIZE; i++)
  {
    SendBuff[i] = i;
  }

  LED_GPIO_Config();
  KEY_GPIO_Config();
  EXTI0_Key_Config();
  EXTI1_Key_Config();
  USART_Config();
  // DMA_MTOM_Config();
  USARTx_MTOM_Config();
  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);

  // while (DMA_GetFlagStatus(MTOM_DMA_FLAG_TC) == RESET)
  // {
  // }

  // status = Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);
  // if (status == 0)
  // {
  //   GPIO_ResetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
  //   GPIO_SetBits(LED_GPIO_Port, LED_B_GPIO_PIN);
  //   GPIO_SetBits(LED_GPIO_Port, LED_G_GPIO_PIN);
  // }
  // else
  // {
  //   GPIO_ResetBits(LED_GPIO_Port, LED_G_GPIO_PIN);
  //   GPIO_SetBits(LED_GPIO_Port, LED_R_GPIO_PIN);
  //   GPIO_SetBits(LED_GPIO_Port, LED_B_GPIO_PIN);
  // }

  // Usart_Send_TypeByte(DEBUG_USARTx, 0x64);
  // Usart_SendHalfWord(DEBUG_USARTx, 0xff56);
  // Usart_Send_Array(DEBUG_USARTx, a, 10);
  // Usart_Send_Str(DEBUG_USARTx, "uint8_t *Str \n");
  // printf("UTF-8zhongwen \n");
  // putchar('p');
  // getchar('g');

  while (1)
  {

    // Light();
    // SysTick_Delay_Ms(500);
    // Usart_Send_Array(DEBUG_USARTx, a, 10);
    // Usart_Send_Str(DEBUG_USARTx, ucTemp);
    // printf("UTF-8zhongwen \n");

    // ch = getchar();
    // printf("ch = %s\n", ch);
  }
}
