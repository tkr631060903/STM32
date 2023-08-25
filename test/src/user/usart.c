#include "usart.h"

static void NVIC_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* 配置NVIC为优先级组1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel                   = DEBUG_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
    // 打开GPIO串口时钟
    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
    // 打开串口外设时钟
    DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
    // 将USART TX配置为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    // 将USART RX配置为浮空输入
    GPIO_InitStructure.GPIO_Pin  = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    // 配置串口工作参数
    USART_InitTypeDef USART_InitStructure;
    // 波特率
    USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
    // 数据长度
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    // 停止位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    // 校验位
    USART_InitStructure.USART_Parity = USART_Parity_No;
    // 硬件流控
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    // 工作模式,收发一起
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEBUG_USARTx, &USART_InitStructure);
    // 配置中断优先级
    NVIC_config();
    // 使能串口中断
    USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
    // 使能串口
    USART_Cmd(DEBUG_USARTx, ENABLE);
}

// 发送一个字符
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
    USART_SendData(pUSARTx, ch);

    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

// 发送16位
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
    uint8_t tmepHigh, tempLow;
    // 取出高8位
    tmepHigh = (ch & 0xFF00) >> 8;
    // 取出低8位
    tempLow = (ch & 0x00FF);
    // 分别发送高8位和低8位
    Usart_SendByte(pUSARTx, tmepHigh);
    Usart_SendByte(pUSARTx, tempLow);
    // 判断是否TXE是否发送完成
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

// 发送字符串
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
    uint8_t i = 0;
    while (*(str + i) != '\0') {
        Usart_SendByte(pUSARTx, *(str + i));
        i++;
    }

    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

// 重定向printg到串口
int fputc(int ch, FILE *f)
{
    USART_SendData(DEBUG_USARTx, (uint8_t)ch);
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET)
        ;
    return (ch);
}

// 重定向scanf到串口，可以使用scanf，getchar函数
int fgetc(FILE *f)
{
		// 等待串口输入
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
