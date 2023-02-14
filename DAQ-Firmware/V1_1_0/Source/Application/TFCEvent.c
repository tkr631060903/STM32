#include "includes.h"

u32 EVENT_MASK = 0xFFFFFFFF & (~EVENT_UART) & (~EVENT_CAN);

static u32 gTFCEvent = 0;

void SetTFCEvent(u32 event)
{
    gTFCEvent |= event;
}
void ClearTFCEvent(u32 event)
{
    gTFCEvent &= (~event);
}
u32 GetTFCEvent()
{
    return gTFCEvent;
}


void TFCReportEvent(int event)
{
    event &= EVENT_MASK;
    const char uartEventReportText[]="{\"cmd\": \"report_event\",\"event_code\":0,\"report\":\"Uart text received!\",\"status\":1,\"time_stamp\":";
    const char can1EventReportText[]="{\"cmd\": \"report_event\",\"event_code\":1,\"report\":\"CAN1 msg received!\",\"status\":1,\"time_stamp\":";
    const char can2EventReportText[]="{\"cmd\": \"report_event\",\"event_code\":1,\"report\":\"CAN2 msg received!\",\"status\":1,\"time_stamp\":";
    const char keyEventReportText[]="{\"cmd\": \"report_event\",\"event_code\":2,\"report\":\"Key pressed!\",\"status\":1,\"time_stamp\":";
    
    if(event&EVENT_UART)
        UART3_PrintStr((char*)uartEventReportText);
    else if(event&EVENT_CAN1)
        UART3_PrintStr((char*)can1EventReportText);
    else if(event&EVENT_CAN2)
        UART3_PrintStr((char*)can2EventReportText);
    else
        UART3_PrintStr((char*)keyEventReportText);
    
    char outBuf[22];
    sprintf(outBuf,"%d}\r\n",GetSystemTime());
    UART3_PrintStr(outBuf);
}



void EXTI9_5_IRQHandler(void)
{
    
    if(EXTI_GetITStatus(EXTI_Line8) != 0)
    {
        DelayMs(11);
        if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8))
        {
            SetTFCEvent(EVENT_KEY0);
            TFCReportEvent(EVENT_KEY0);
        }
        EXTI_ClearITPendingBit(EXTI_Line8); 
    }
    else if(EXTI_GetITStatus(EXTI_Line9) != 0)
    {
        DelayMs(11);
        if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9))
        {
            SetTFCEvent(EVENT_KEY1);
            TFCReportEvent(EVENT_KEY1);
        }
        EXTI_ClearITPendingBit(EXTI_Line9); 
    }
    
}

void EXTI15_10_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line10) != 0)
    {
        DelayMs(11);
        if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10))
        {
            SetTFCEvent(EVENT_KEY2);
            TFCReportEvent(EVENT_KEY2);
        }
        EXTI_ClearITPendingBit(EXTI_Line10); 
    }
    else if(EXTI_GetITStatus(EXTI_Line11) != 0)
    {
        DelayMs(11);
        if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11))
        {
            SetTFCEvent(EVENT_KEY3);
            TFCReportEvent(EVENT_KEY3);
        }
        EXTI_ClearITPendingBit(EXTI_Line11); 
    }
}


int GetKeyStatus(char* keyStatus)
{
    int i;
    for(i=0;i<4;i++) 
    {
        keyStatus[i] = '0';
    }
    
    if(GetTFCEvent()&EVENT_KEY0) keyStatus[0] = '1';
    if(GetTFCEvent()&EVENT_KEY1) keyStatus[1] = '1';
    if(GetTFCEvent()&EVENT_KEY2) keyStatus[2] = '1';
    if(GetTFCEvent()&EVENT_KEY3) keyStatus[3] = '1';
    
    ClearTFCEvent(EVENT_KEY);
    return 1;
}