#include "includes.h"

u32 ERROR_MASK = 0x00FFFFFF;

static u32 gTFCError = 0;

void SetTFCError(u32 error)
{
    gTFCError |= error;
}
void ClearTFCError(u32 error)
{
    gTFCError &= (~error);
}
u32 GetTFCError()
{
    return gTFCError;
}


void TFCReportError(int error)
{
    error &= ERROR_MASK;
    char ss[200];
    int error_channel = 0;
    if(!error) return;
    while(error)
    {
        if(error&0x00000001) 
            break;
        else 
        {
            error>>=1;
        }
        error_channel++;
    }
    sprintf(ss,"{\"cmd\": \"report_error\",\"error_code\":1,\"report\":\"CH%d short circuit ever since!\",\"status\":1,\"time_stamp\":%d}\r\n",error_channel,GetSystemTime());
    UART3_PrintStr(ss);
}


void EXTI0_IRQHandler(void)//DO 0
{   
    if(EXTI_GetITStatus(EXTI_Line0) != 0)
    {
        SetTFCError(ERROR_GENERAL);
        EXTI_ClearITPendingBit(EXTI_Line0); 
    }
}

void EXTI3_IRQHandler(void)//DO 1
{
    if(EXTI_GetITStatus(EXTI_Line3) != 0)
    {
        SetTFCError(ERROR_GENERAL);
        EXTI_ClearITPendingBit(EXTI_Line3); 
    }
}


int GetErrorStatus(char* errorStatus)
{
    int i;
    u32 es = GetTFCError() & ERROR_MASK;
    errorStatus[32]=0;
    for(i=0;i<32;i++)
    {
        errorStatus[i]='0'+ (es&0x01);
        es >>= 1;
    }
    ClearTFCError(ERROR_MASK);
    return 1;
}