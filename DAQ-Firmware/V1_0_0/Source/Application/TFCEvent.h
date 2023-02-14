#ifndef _TFC_EVENT_H_
#define _TFC_EVENT_H_

#define EVENT_UART1 0x0001
#define EVENT_UART2 0x0002
#define EVENT_UART3 0x0004
#define EVENT_UART4 0x0008
#define EVENT_UART5 0x0010
#define EVENT_UART  (EVENT_UART1|EVENT_UART2|EVENT_UART3|EVENT_UART4|EVENT_UART5)

#define EVENT_CAN1  0x0020
#define EVENT_CAN2  0x0040
#define EVENT_CAN   (EVENT_CAN1|EVENT_CAN2)

#define EVENT_KEY0  0x0080
#define EVENT_KEY1  0x0100
#define EVENT_KEY2  0x0200
#define EVENT_KEY3  0x0400
#define EVENT_KEY   (EVENT_KEY0|EVENT_KEY1|EVENT_KEY2|EVENT_KEY3)

extern u32 EVENT_MASK;

void SetTFCEvent(u32 event);
void ClearTFCEvent(u32 event);
u32 GetTFCEvent();
void TFCReportEvent(int event);

int GetKeyStatus(char* keyStatus);

#endif