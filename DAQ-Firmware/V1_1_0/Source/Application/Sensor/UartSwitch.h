
#ifndef _UART_SWITCH_H_
#define _UART_SWITCH_H_




int ClearUart1RxBuffer();
int ClearUart2RxBuffer();
int ClearUart3RxBuffer();
int ClearUart4RxBuffer();
int ClearUart5RxBuffer();

int GetUart1RxStatus(char* strbuf);
int GetUart2RxStatus(char* strbuf);
int GetUart3RxStatus(char* strbuf);
int GetUart4RxStatus(char* strbuf);
int GetUart5RxStatus(char* strbuf);

int GetData(int port, char* buf);

void AddCheckStringVar(char* str, int var);
void Uart3HardResetEnable();



#endif
