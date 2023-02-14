


#include "includes.h"



u32 gOxygenUpdateTime = 0;
//
//


int EventOxygenDataReceived(char* str)
{
    int ret = 0;
    static u8 counter = 0;
    static u8 ct = 0;
    
    if((str[2]&0x42)==0 && str[3]==0)
    {
        user.oxygenHeartRate  = str[0];
        user.oxygen = str[1]; 
        if(user.oxygen == 0x7F)
        {
            ct++;
            if(ct<3)
            {
                user.oxygen = 0;
                user.oxygenStatus = 'T';
                ret = 0;
            }
            else
            {
                ct = 0;
                user.oxygen = 90;
                user.oxygenStatus = 'R';
                ret = 1;
            }
            
        }
        else
        {
            user.oxygenStatus = 'R';
            ret = 1;
            ct = 0;
        }
        
    }
    else
    {
        user.oxygen  = 0;
        user.oxygenHeartRate = 0;
        user.oxygenStatus = 'T';
        ret = 2;
        counter ++;
        if(counter>=5)
        {
            user.oxygenStatus = 'R';
            counter = 0;   
        }
    }
    
    
    
    gOxygenUpdateTime = GetSystemTime();
    
    
    
    return ret;
}


int CheckOxygenReceivedString(char* str)
{

    int tmp = GetUartSwitch();
    
    if(tmp != FUNC_OXYGEN)
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

int GetFirstOxygenData()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    //char idReqMsg[] = {0x00, 0x00, 0x7f, 0x03, 0x00, 0x10, 0x13 };
    
    SetBusy(1);
    SetUartSwitch(FUNC_OXYGEN);
    //SendData(PORT_SENSOR,idReqMsg,7);
    SetBusy(0);
    int counter = 1000;//在2000ms内完成接收
    gBloodPressUpdateTime = GetSystemTime() ;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
        if(GetSystemTime() - gBloodPressUpdateTime >50)
            return 0;
    }
    if(counter)
    {
        if(CheckOxygenReceivedString(buf))
        {
            return EventOxygenDataReceived(buf);
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


