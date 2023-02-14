

#include "includes.h"



u8 Uart1_Rx_Buffer[UART1_BUFFER_SIZE*2]={0};
int Uart1_Rx_Index = 0;

int ClearUart1RxBuffer()
{
    int i;
    for(i=0;i<UART1_BUFFER_SIZE*2;i++)
    {
        Uart1_Rx_Buffer[i] = 0;
    }
    Uart1_Rx_Index = 0;
    return 1;
}

int GetUart1RxStatus(char* strbuf)
{
    int retVal=Uart1_Rx_Index;
    if(Uart1_Rx_Index)
    {
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
        memcpy(strbuf,Uart1_Rx_Buffer+1,retVal);//收到的第一个字符存在Uart1_Rx_Buffer[1]中而不是Uart1_Rx_Buffer[0]中
        Uart1_Rx_Index = 0;
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
    }
    return retVal;
}
//------------------------------------------------------------------
//函数名：void USART1_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：串口接收中断服务
//------------------------------------------------------------------
void USART1_IRQHandler(void)
{
    char c;    
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)          //判断读寄存器是否非空
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志. 
        c = USART_ReceiveData(USART1);    //接收字节到接收缓冲区
        
        Uart1_Rx_Index++;
        if(Uart1_Rx_Index>=UART1_BUFFER_SIZE)
            Uart1_Rx_Index = 0;
        Uart1_Rx_Buffer[Uart1_Rx_Index] = c;
        Uart1_Rx_Buffer[Uart1_Rx_Index+UART1_BUFFER_SIZE] = c;
        
        SetTFCEvent(EVENT_UART1);
    } 
}


u8 Uart2_Rx_Buffer[UART2_BUFFER_SIZE*2]={0};
int Uart2_Rx_Index = 0;

int ClearUart2RxBuffer()
{
    int i;
    for(i=0;i<UART2_BUFFER_SIZE*2;i++)
    {
        Uart2_Rx_Buffer[i] = 0;
    }
    Uart2_Rx_Index = 0;
    return 1;
}
int GetUart2RxStatus(char* strbuf)
{ 
    int retVal=Uart2_Rx_Index;
    
    if(Uart2_Rx_Index)
    {   
        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
        memcpy(strbuf,Uart2_Rx_Buffer+1,retVal);//收到的第一个字符存在Uart2_Rx_Buffer[1]中而不是Uart2_Rx_Buffer[0]中
        Uart2_Rx_Index = 0;
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    }
    return retVal;
}
//------------------------------------------------------------------
//函数名：void USART1_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：串口接收中断服务
//------------------------------------------------------------------
void USART2_IRQHandler(void)
{
    char c;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)          //判断读寄存器是否非空
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志. 
        c = USART_ReceiveData(USART2);    //接收字节到接收缓冲区
        
        Uart2_Rx_Index++;
        if(Uart2_Rx_Index>=UART2_BUFFER_SIZE)
            Uart2_Rx_Index = 0;
        Uart2_Rx_Buffer[Uart2_Rx_Index] = c;
        Uart2_Rx_Buffer[Uart2_Rx_Index+UART2_BUFFER_SIZE] = c;
        
        SetTFCEvent(EVENT_UART2);
    } 
}


u8 Uart3_Rx_Buffer[UART3_BUFFER_SIZE+UART3_BUFFER_SIZE]={0};
int Uart3_Rx_Index = 0;

int ClearUart3RxBuffer()
{
    int i;
    for(i=0;i<UART3_BUFFER_SIZE*2;i++)
    {
        Uart3_Rx_Buffer[i] = 0;
    }
    Uart3_Rx_Index = 0;
    return 1;
}
int GetUart3RxStatus(char* strbuf)
{ 
    int i=0;
    int charReceived=0;
    int startIndex = 0;
    int retVal = 0;
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
    
    if(Uart3_Rx_Buffer[Uart3_Rx_Index]==0x0a)
    {   
        for(i=Uart3_Rx_Index+UART3_BUFFER_SIZE-1;i>Uart3_Rx_Index;i--)
        {
            if(Uart3_Rx_Buffer[i]==0x0d || Uart3_Rx_Buffer[i]==0x0a || Uart3_Rx_Buffer[i]==0x00)
                break;
        }
        if(i<=Uart3_Rx_Index) 
        {
            strbuf[0] = 0;
            retVal = 0;
        }
        else
        {
            charReceived = Uart3_Rx_Index+UART3_BUFFER_SIZE - i;
            startIndex = i+1;
            //DebugOutputArray((char*)(&Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-12+0+1]),12);
            for(i=0;i<charReceived;i++)
            {
                strbuf[i] = Uart3_Rx_Buffer[startIndex+i];
            }
            retVal = charReceived;
            strbuf[charReceived] = 0;
        }
        
        Uart3_Rx_Buffer[Uart3_Rx_Index]=0x0D;
        Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE]=0x0D;
    }
    Uart3_Rx_Index = (Uart3_Rx_Index + UART3_BUFFER_SIZE - charReceived)%UART3_BUFFER_SIZE;
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    
    
    return retVal;
}
//------------------------------------------------------------------
//函数名：void USART1_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：串口接收中断服务
//------------------------------------------------------------------
void USART3_IRQHandler(void)
{
    char c;
    
    
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)          //判断读寄存器是否非空
    {
        USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志. 
        c = USART_ReceiveData(USART3);    //接收字节到接收缓冲区
        if(c == 0x0D)
		    return;
        
        Uart3_Rx_Index++;
        if(Uart3_Rx_Index>=UART3_BUFFER_SIZE)
            Uart3_Rx_Index = 0;
        Uart3_Rx_Buffer[Uart3_Rx_Index] = c;
        Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE] = c;
        
        if(Uart3_Rx_Buffer[Uart3_Rx_Index]==0x0a)
        {
          if(Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-15]== '{' && Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-14]== '"' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-13]== 'c' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-12]== 'm' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-11]== 'd' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-10]== '"' 
             &&Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-9]== ':' && Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-8]== '"' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-7]== 'r' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-6]== 'e' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-5]== 's' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-4]== 'e'
               &&Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-3]== 't' && Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-2]== '"' &&  Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-1]== '}')
            {
                NVIC_SystemReset();
                while(1);
            }
        }
        //UART1_SendByte(c);
        SetTFCEvent(EVENT_UART3);
    }         
}

u8 Uart4_Rx_Buffer[UART4_BUFFER_SIZE+UART4_BUFFER_SIZE]={0};
int Uart4_Rx_Index = 0;

int ClearUart4RxBuffer()
{
    int i;
    for(i=0;i<UART4_BUFFER_SIZE*2;i++)
    {
        Uart4_Rx_Buffer[i] = 0;
    }
    Uart4_Rx_Index = 0;
    return 1;
}

int GetUart4RxStatus(char* strbuf)
{ 
    int i=0;
    int charReceived=0;
    int startIndex = 0;
    int retVal = 0;
    USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
    
    if(Uart4_Rx_Buffer[Uart4_Rx_Index]==0x0a)
    {   
        for(i=Uart4_Rx_Index+UART4_BUFFER_SIZE-1;i>Uart4_Rx_Index;i--)
        {
            if(Uart4_Rx_Buffer[i]==0x0d || Uart4_Rx_Buffer[i]==0x0a || Uart4_Rx_Buffer[i]==0x00)
                break;
        }
        if(i<=Uart4_Rx_Index) 
        {
            strbuf[0] = 0;
            retVal = 0;
        }
        else
        {
            charReceived = Uart4_Rx_Index+UART4_BUFFER_SIZE - i;
            startIndex = i+1;
            //DebugOutputArray((char*)(&Uart3_Rx_Buffer[Uart3_Rx_Index+UART3_BUFFER_SIZE-12+0+1]),12);
            for(i=0;i<charReceived;i++)
            {
                strbuf[i] = Uart4_Rx_Buffer[startIndex+i];
            }
            retVal = charReceived;
            strbuf[charReceived] = 0;
        }
        
        Uart4_Rx_Buffer[Uart4_Rx_Index]=0x0D;
        Uart4_Rx_Buffer[Uart4_Rx_Index+UART4_BUFFER_SIZE]=0x0D;
    }
    Uart4_Rx_Index = (Uart4_Rx_Index + UART4_BUFFER_SIZE - charReceived)%UART4_BUFFER_SIZE;
    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    return retVal;
}
//------------------------------------------------------------------
//函数名：void USART1_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：串口接收中断服务
//------------------------------------------------------------------
void UART4_IRQHandler(void)
{
    char c;
    
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)          //判断读寄存器是否非空
    {
        USART_ClearITPendingBit(UART4,USART_IT_RXNE); //清除中断标志. 
        c = USART_ReceiveData(UART4);    //接收字节到接收缓冲区
        if(c == 0x0D)
		    return;
        
        Uart4_Rx_Index++;
        if(Uart4_Rx_Index>=UART4_BUFFER_SIZE)
            Uart4_Rx_Index = 0;
        Uart4_Rx_Buffer[Uart4_Rx_Index] = c;
        Uart4_Rx_Buffer[Uart4_Rx_Index+UART4_BUFFER_SIZE] = c;
        
        SetTFCEvent(EVENT_UART4);
    }         
}

u8 Uart5_Rx_Buffer[UART5_BUFFER_SIZE+UART5_BUFFER_SIZE]={0};
int Uart5_Rx_Index = 0;

int ClearUart5RxBuffer()
{
    int i;
    for(i=0;i<UART5_BUFFER_SIZE*2;i++)
    {
        Uart5_Rx_Buffer[i] = 0;
    }
    Uart5_Rx_Index = 0;
    return 1;
}
int GetUart5RxStatus(char* strbuf)
{ 
    int retVal=Uart5_Rx_Index;
    
    if(Uart5_Rx_Index)
    {   
        USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
        memcpy(strbuf,Uart5_Rx_Buffer+1,retVal);//收到的第一个字符存在Uart2_Rx_Buffer[1]中而不是Uart2_Rx_Buffer[0]中
        Uart5_Rx_Index = 0;
        USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    }
    return retVal;
}
//------------------------------------------------------------------
//函数名：void USART1_IRQHandler(void)
//输入参数：null
//返回参数：null
//说明：串口接收中断服务
//------------------------------------------------------------------
void UART5_IRQHandler(void)
{
    char c;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)          //判断读寄存器是否非空
    {
        USART_ClearITPendingBit(UART5,USART_IT_RXNE); //清除中断标志. 
        c = USART_ReceiveData(UART5);    //接收字节到接收缓冲区
        
        Uart5_Rx_Index++;
        if(Uart5_Rx_Index>=UART5_BUFFER_SIZE)
            Uart5_Rx_Index = 0;
        Uart5_Rx_Buffer[Uart5_Rx_Index] = c;
        Uart5_Rx_Buffer[Uart5_Rx_Index+UART5_BUFFER_SIZE] = c;
        
        SetTFCEvent(EVENT_UART5);
    } 
}

int GetData(int port, char* buf)
{
    if(port==1)
        return GetUart1RxStatus(buf);
    else if(port==2) 
        return GetUart2RxStatus(buf);
    else if(port==3) 
        return GetUart3RxStatus(buf);
    else if(port==4) 
        return GetUart4RxStatus(buf);
    else if(port==5) 
        return GetUart5RxStatus(buf);
    else return 0;
}


void AddCheckStringVar(char* str, int var)
{
    const char table[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    u8 result = 0;
    int i;
    
    if(var <4)
        return;
    
    result = str[0];
    for(i=1;i<var-3;i++)
    {
        result ^= str[i];    
    }
    str[var-3] = table[(result>>4)];
    str[var-2] = table[(result&0x0F)];
    return;
}

