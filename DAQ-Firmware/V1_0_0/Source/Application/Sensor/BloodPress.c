

#include "includes.h"



u32 gBloodPressUpdateTime = 0;
//
//


int EventBloodPressDataReceived(char* str)
{
    int ret = 0;
    
    user.highBloodPress =str[0];
    user.lowBloodPress =str[1];
    user.heartRate =str[2];
    
    user.bloodPressStatus = 'R';
    
    gBloodPressUpdateTime = GetSystemTime();
    
    ret = 1;
#if SEND_RESULT_TWICE    
    char buf[12];
    strcpy(buf,"RBA+000.0HL");
    buf[4] = '0'+user.highBloodPress/100%10;
    buf[5] = '0'+user.highBloodPress/10%10;
    buf[6] = '0'+user.highBloodPress%10;
    buf[11]= 0x0a;
    AddCheckString(buf);
    UART2_PrintArray(buf,12);
    DelayMs(50);
    
    strcpy(buf,"RBB+000.0HL");
    buf[4] = '0'+user.lowBloodPress/100%10;
    buf[5] = '0'+user.lowBloodPress/10%10;
    buf[6] = '0'+user.lowBloodPress%10;
    buf[11]= 0x0a;
    AddCheckString(buf);
    UART2_PrintArray(buf,12);
    DelayMs(50);
    
    strcpy(buf,"RBR+000.0HL");
    buf[4] = '0'+user.heartRate/100%10;
    buf[5] = '0'+user.heartRate/10%10;
    buf[6] = '0'+user.heartRate%10;
    buf[11]= 0x0a;
    AddCheckString(buf);
    UART2_PrintArray(buf,12);
    DelayMs(50);
#endif    
    return ret;
}


int CheckBloodPressReceivedString(char* str)
{

    int tmp = GetUartSwitch();
    
    if(tmp != FUNC_PRESSURE)
    {
        return 0;
    }
    else 
    {
        if(str[3]==0x00)
            return 1;
    }
    return 0;
}

int GetFirstBloodPressData()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    char bloodPressReqMsg[] = {0xFF,0xFF,0xFD,0xFD,0xFA,0x05,0X0D, 0x0A};
    
    
    SetBusy(1);
    SetUartSwitch(FUNC_PRESSURE);
//    SendData(PORT_SENSOR,bloodPressReqMsg,8);
    SendData(PORT_SENSOR,bloodPressReqMsg,8);
    SetBusy(0);
    int counter = 80000;//在2000ms内完成接收
    gBloodPressUpdateTime = GetSystemTime();
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
        if(GetSystemTime() - gBloodPressUpdateTime >500)
            return 0;
    }
    if(counter)
    {
        if(CheckBloodPressReceivedString(buf))
        {
            return EventBloodPressDataReceived(buf);
        }
        else
        {
            errorCounter++;
            //校验错误
        }
        
    }
    else
    {
        //do nothing
    }
    return 0;
}


