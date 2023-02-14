
#include "includes.h"




/**********************************************************************************************************
Brief ：UART  初始化
Param : Baud  波特率 
***********************************************************************************************************/
void UART2_Init(u32 Baud)//TX PD5, RX PD6
{
    USART_InitTypeDef    USART_InitStructure;
    GPIO_InitTypeDef     GPIO_InitStructure;
    NVIC_InitTypeDef     NVIC_InitStructure;
    
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    //Enable  clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    //Remap
    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
    
    //Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    //Configure USART Rx as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    USART_Init(USART2, &USART_InitStructure);
    
    //Enable the USART2 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0; //抢占式中断优先级设置为2  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
    //USART_ClearFlag(USART1,USART_FLAG_RXNE);
    USART_ClearFlag(USART2, USART_FLAG_TC);
    //USART_ClearFlag(USART1, USART_FLAG_TXE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);       //使能接收中断
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);       //使能接收中断
    USART_GetFlagStatus(USART2, USART_FLAG_TC);//解决第一个字节发不出去
}

void UART2_SendByte (u8 data)
{
    USART_GetFlagStatus(USART2, USART_FLAG_TC);//解决第一个字节发不出去
    USART_SendData(USART2, data);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void UART_Config(u8 channel, u32 Baud)
{
    USART_InitTypeDef    USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    if(channel==1)
    {
        if(Baud!=sys.uart_baud_rate_default[0])
        {
            USART_Init(USART1, &USART_InitStructure);
            sys.uart_baud_rate_default[0]= Baud;
            StoreSystemParam();
        }
    }
    else if(channel==2)
    {
        if(Baud!=sys.uart_baud_rate_default[1])
        {
            USART_Init(USART2, &USART_InitStructure);
            sys.uart_baud_rate_default[1]= Baud;
            StoreSystemParam();
        }
    }
    else if(channel==3)
    {
        if(Baud!=sys.uart_baud_rate_default[2])
        {
            USART_Init(USART3, &USART_InitStructure);
            sys.uart_baud_rate_default[2]= Baud;
            StoreSystemParam();
        }
    }
    else if(channel==4)
    {
        if(Baud!=sys.uart_baud_rate_default[3])
        {
            USART_Init(UART4, &USART_InitStructure);
            sys.uart_baud_rate_default[3]= Baud;
            StoreSystemParam();
        }
    }
    else if(channel==5)
    {
        if(Baud!=sys.uart_baud_rate_default[4])
        {
            USART_Init(UART5, &USART_InitStructure);
            sys.uart_baud_rate_default[4]= Baud;
            StoreSystemParam();
        }
    }
    
    return ;
}

/**********************************************************************************************************
Brief ：UART  初始化
Param : Baud  波特率 
***********************************************************************************************************/
void UART1_Init(u32 Baud)//TX PA9, RX PA10
{
    USART_InitTypeDef    USART_InitStructure;
    GPIO_InitTypeDef     GPIO_InitStructure;
    NVIC_InitTypeDef     NVIC_InitStructure;
    
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    //Enable  clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
    //Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //Configure USART Rx as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_Init(USART1, &USART_InitStructure);
    
    //Enable the USART2 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0; //抢占式中断优先级设置为2  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
    //USART_ClearFlag(USART1,USART_FLAG_RXNE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
    //USART_ClearFlag(USART1, USART_FLAG_TXE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);       //使能接收中断
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);       //使能接收中断
    USART_GetFlagStatus(USART1, USART_FLAG_TC);//解决第一个字节发不出去
    
}



void UART1_SendByte (u8 data)
{
    USART_GetFlagStatus(USART1, USART_FLAG_TC);//解决第一个字节发不出去
    USART_SendData(USART1, data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}


/**********************************************************************************************************
Brief ：UART  初始化
Param : Baud  波特率 
***********************************************************************************************************/
void UART3_Init(u32 Baud)//TX PB10, RX PB11
{
    USART_InitTypeDef    USART_InitStructure;
    GPIO_InitTypeDef     GPIO_InitStructure;
    NVIC_InitTypeDef     NVIC_InitStructure;
    
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    //Enable  clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    //Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //Configure USART Rx as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    USART_Init(USART3, &USART_InitStructure);
    
    //Enable the USART2 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0; //抢占式中断优先级设置为2  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART3, ENABLE);
    
    USART_ClearFlag(USART3, USART_FLAG_TC);
    //USART_ClearFlag(USART3, USART_FLAG_TXE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);       //使能接收中断
    USART_GetFlagStatus(USART3, USART_FLAG_TC);//解决第一个字节发不出去
}

void UART3_SendByte (u8 data)
{
    USART_SendData(USART3, data);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}



/**********************************************************************************************************
Brief ：UART  初始化
Param : Baud  波特率 
***********************************************************************************************************/
void UART4_Init(u32 Baud)//TX PC10, RX PC11
{
    USART_InitTypeDef    USART_InitStructure;
    GPIO_InitTypeDef     GPIO_InitStructure;
    NVIC_InitTypeDef     NVIC_InitStructure;
    
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    //Enable  clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    //Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //Configure USART Rx as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    USART_Init(UART4, &USART_InitStructure);
    
    //Enable the USART2 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0; //抢占式中断优先级设置为2  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(UART4, ENABLE);
    
    USART_ClearFlag(UART4, USART_FLAG_TC);
    //USART_ClearFlag(USART3, USART_FLAG_TXE);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);       //使能接收中断
    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);       //使能接收中断
    USART_GetFlagStatus(UART4, USART_FLAG_TC);//解决第一个字节发不出去
}

void UART4_SendByte (u8 data)
{
    USART_SendData(UART4, data);
    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
}

/**********************************************************************************************************
Brief ：UART  初始化
Param : Baud  波特率 
***********************************************************************************************************/
void UART5_Init(u32 Baud)//TX PC12, RX PD2
{
    USART_InitTypeDef    USART_InitStructure;
    GPIO_InitTypeDef     GPIO_InitStructure;
    NVIC_InitTypeDef     NVIC_InitStructure;
    
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    //Enable  clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    //Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //Configure USART Rx as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    USART_Init(UART5, &USART_InitStructure);
    
    //Enable the USART2 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0; //抢占式中断优先级设置为2  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(UART5, ENABLE);
    
    USART_ClearFlag(UART5, USART_FLAG_TC);
    //USART_ClearFlag(USART3, USART_FLAG_TXE);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);       //使能接收中断
    USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);       //使能接收中断
    USART_GetFlagStatus(UART5, USART_FLAG_TC);//解决第一个字节发不出去
}

void UART5_SendByte (u8 data)
{
    USART_GetFlagStatus(UART5, USART_FLAG_TC);//解决第一个字节发不出去
    USART_SendData(UART5, data);
    while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
}




void UART1_PrintStr(char *buffer)
{
    while(*buffer != '\0')
    {
        UART1_SendByte(*buffer++);
    }
}
void UART2_PrintStr(char *buffer)
{
    while(*buffer != '\0')
    {
        UART2_SendByte(*buffer++);
    }
}

void UART3_PrintStr(char *buffer)
{
    while(*buffer != '\0')
    {
        UART3_SendByte(*buffer++);
    }
}
void UART4_PrintStr(char *buffer)
{
    while(*buffer != '\0')
    {
        UART4_SendByte(*buffer++);
    }
}
void UART5_PrintStr(char *buffer)
{
    while(*buffer != '\0')
    {
        UART5_SendByte(*buffer++);
    }
}

void UART1_PrintArray(char *buffer, int number)
{
    while(number)
    {
        UART1_SendByte(*buffer++);
        number --;
    }
}
void UART2_PrintArray(char *buffer, int number)
{
    while(number)
    {
        UART2_SendByte(*buffer++);
        number --;
    }
}
void UART3_PrintArray(char *buffer, int number)
{
    while(number)
    {
        UART3_SendByte(*buffer++);
        number --;
    }
}
void UART4_PrintArray(char *buffer, int number)
{
    while(number)
    {
        UART4_SendByte(*buffer++);
        number --;
    }
}
void UART5_PrintArray(char *buffer, int number)
{
    while(number)
    {
        UART5_SendByte(*buffer++);
        number --;
    }
}

//port可选参数：
int SendData(int port, char* buf, int numToSend)
{   
    if(port==1)
        UART1_PrintArray(buf, numToSend);
    else if(port==2) 
        UART2_PrintArray(buf, numToSend);
    else if(port==3) 
        UART3_PrintArray(buf, numToSend);
    else if(port==4) 
        UART4_PrintArray(buf, numToSend);
    else if(port==5) 
        UART5_PrintArray(buf, numToSend);
    else return 0;
    return numToSend;
}


//不建议使用
//强制输出Debug信息
int printf(const char *fmt,...)
{
    int ret = 0;
    va_list ap;
    char string[256];
    va_start(ap,fmt);
    ret = vsprintf(string,fmt,ap);
    UART4_PrintStr(string);
    va_end(ap);
    return ret;
}

u8 IS_DEBUG_MODE = 1;


int debug(const char *fmt,...)
{
    int ret = 0;
    if(!IS_DEBUG_MODE)
        return 0;

    va_list ap;
    char string[UART3_BUFFER_SIZE];
    va_start(ap,fmt);
    ret = vsprintf(string,fmt,ap);
    UART4_PrintStr(string);
    va_end(ap);

    return ret;
}


int SetDebugMode(u8 dbMode)
{
    if(dbMode) IS_DEBUG_MODE =1;
    else IS_DEBUG_MODE = 0;
    return 1;
}