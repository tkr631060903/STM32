#include "includes.h"

u32 ERROR_MASK = 0xFFFFFFFF;

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


void TFCReportError(int errorToReport)
{
    int error = errorToReport;
    if(!error) return;
    
    error &= ERROR_SHORT;
    char ss[200];
    if(error)
    {
        int error_channel = 0;
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
    
    error = errorToReport;
    error &= ERROR_PWR;
    if(error)
    {
        if(error & ERROR_PWR_IN)
        {
            MessageBox("Error","输入异常");
            sprintf(ss,"{\"cmd\": \"report_error\",\"error_code\":2,\"report\":\"ERROR_PWR_IN\",\"status\":1,\"time_stamp\":%d}\r\n",GetSystemTime());
        }
        else if(error & ERROR_PWR_SELF)
        {
            MessageBox("Error","自身故障");
            sprintf(ss,"{\"cmd\": \"report_error\",\"error_code\":3,\"report\":\"ERROR_PWR_SELF\",\"status\":1,\"time_stamp\":%d}\r\n",GetSystemTime());
        }
        else if(error & ERROR_PWR_OC0)
        {
            MessageBox("Error","CH0过流");
            sprintf(ss,"{\"cmd\": \"report_error\",\"error_code\":4,\"report\":\"ERROR_PWR_OC0\",\"status\":1,\"time_stamp\":%d}\r\n",GetSystemTime());
        }
        else if(error & ERROR_PWR_OC1)
        {
            MessageBox("Error","CH1过流");
            sprintf(ss,"{\"cmd\": \"report_error\",\"error_code\":5,\"report\":\"ERROR_PWR_OC1\",\"status\":1,\"time_stamp\":%d}\r\n",GetSystemTime());
        }
        else if(error & ERROR_PWR_SC0)
        {
            MessageBox("Error","CH0短路");
            sprintf(ss,"{\"cmd\": \"report_error\",\"error_code\":6,\"report\":\"ERROR_PWR_SC0\",\"status\":1,\"time_stamp\":%d}\r\n",GetSystemTime());
        }
        else 
        {
            MessageBox("Error","CH1短路");
            sprintf(ss,"{\"cmd\": \"report_error\",\"error_code\":7,\"report\":\"ERROR_PWR_SC1\",\"status\":1,\"time_stamp\":%d}\r\n",GetSystemTime());
        }
        UART3_PrintStr(ss);
    }
}


void EXTI0_IRQHandler(void)//DO 0
{   
    if(EXTI_GetITStatus(EXTI_Line0) != 0)
    {
        SetTFCError(ERROR_DAQ);
        EXTI_ClearITPendingBit(EXTI_Line0); 
    }
}

void EXTI3_IRQHandler(void)//DO 1
{
    if(EXTI_GetITStatus(EXTI_Line3) != 0)
    {
        SetTFCError(ERROR_DAQ);
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