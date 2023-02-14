

#include "includes.h"



u32 gBodyParameterUpdateTime = 0;
//
//


int EventParameterDataReceived(char* str)
{
    int ret = 0;
    if(str[2]==0 && str[3]==0)
    {
        user.parameterStatus = 'R';
        ret = 1;
    }
    else
    { 
        user.parameterStatus = 'R';
        ret = 2;
    }
    gBodyParameterUpdateTime = GetSystemTime();
    return ret;
}


int CheckBodyParameterReceivedString(char* str)
{

    int tmp = GetUartSwitch();
    
    if(tmp != FUNC_WEIGHT)
    {
        return 0;
    }
    else 
    {
        if(str[4]==0x00)
            return 1;
    }
    return 0;
}

int GetFirstBodyParameterData()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    //char idReqMsg[] = {0x00, 0x00, 0x7f, 0x03, 0x00, 0x10, 0x13 };
    
    SetBusy(1);
    SetUartSwitch(FUNC_WEIGHT);
    //SendData(PORT_SENSOR,idReqMsg,7);
    SetBusy(0);
    int counter = 1000;//在2000ms内完成接收
    gBodyParameterUpdateTime = GetSystemTime() ;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
        if(GetSystemTime() - gBodyParameterUpdateTime >50)
            return 0;
    }
    if(counter)
    {
        if(CheckBodyParameterReceivedString(buf))
        {
            return EventParameterDataReceived(buf);
        }
        else
        {
            errorCounter++;
            //校验错误
        }
        
    }
    else
    {
        return 3;//do nothing
    }
    return 0;
}


