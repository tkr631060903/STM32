#ifndef _DEBUG_STM32_
#define _DEBUG_STM32_

#define PORT_COMM    0
#define PORT_SENSOR      1

extern u8 IS_DEBUG_MODE;

void UART_Config(u8 channel, u32 Baud);

void UART1_Init(u32 Baud);
void UART1_SendByte (u8 data);
void UART1_PrintStr(char *buffer);
void UART1_PrintArray(char *buffer, int number);

void UART2_Init(u32 Baud);
void UART2_SendByte (u8 data);
void UART2_PrintStr(char *buffer);
void UART2_PrintArray(char *buffer, int number);

void UART3_Init(u32 Baud);
void UART3_SendByte (u8 data);
void UART3_PrintStr(char *buffer);
void UART3_PrintArray(char *buffer, int number);

void UART4_Init(u32 Baud);
void UART4_SendByte (u8 data);
void UART4_PrintStr(char *buffer);
void UART4_PrintArray(char *buffer, int number);

void UART5_Init(u32 Baud);
void UART5_SendByte (u8 data);
void UART5_PrintStr(char *buffer);
void UART5_PrintArray(char *buffer, int number);

int printf(const char *fmt,...);
int SendData(int port, char* buf, int numToSend);


int debug(const char *fmt,...);
int SetDebugMode(u8 dbMode);

#endif