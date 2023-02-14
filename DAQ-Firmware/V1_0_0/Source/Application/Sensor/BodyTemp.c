

#include "includes.h"


u32 gBodyTempUpdateTime = 0;

//
//体温计模块


int EventBodyTempDataReceived(char* str)
{
    int ret = 0;
    
    user.bodyTemp = (float)(256*str[0]+str[1])/100;
    /*if(user.bodyTemp>42)
        user.bodyTemp = 42;
    if(user.bodyTemp < 35)
        user.bodyTemp = 35;
    */
    gBodyTempUpdateTime = GetSystemTime();
    
    ret = 1;
    
    return ret;
}


int CheckBodyTempReceivedString(char* str)
{

    int tmp = GetUartSwitch();
    
    if(tmp != FUNC_BTEMP)
    {
        return 0;
    }
    else 
    {
        if(str[2]==0x00)
            return 1;
    }
    return 0;
}

int GetFirstBodyTempData()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    char bodyTempReqMsg[] = {0xFF ,0xAA ,0x03 ,0xFA ,0x00 ,0xF9 ,0xFF ,0xAA ,0x03 ,0xFA ,0x00 ,0xF9 ,0xFF ,0xAA ,0x03 ,0xFA ,0x00 ,0xF9};
    
    
    SetBusy(1);
    SetUartSwitch(FUNC_BTEMP);
    SendData(PORT_SENSOR,bodyTempReqMsg,18);
    SetBusy(0);
    int counter = 2000;//在2000ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckBodyTempReceivedString(buf))
        {
            return EventBodyTempDataReceived(buf);
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



/*
int SetHeightTest()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    
    SetBusy(1);
    DelayMs(30);
    SetUartSwitch(FUNC_HEIGHT);
    
    SendData(PORT_SENSOR,"?H3\n",4);
    SetBusy(0);
    
    int counter = 40;//在20ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            if(buf[0]=='5' && buf[1]=='R' && buf[2]=='1' && buf[3]=='0' && buf[4]=='0' && buf[5]=='.' )
                return 1;
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



int SetHeightCalib()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    
    SetBusy(1);
    SetUartSwitch(FUNC_HEIGHT);
    
    SendData(PORT_SENSOR,"?H2\n",4);
    SetBusy(0);
    
    int counter = 20;//在20ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            if(buf[0]=='5' && buf[1]=='R' && buf[2]=='0' && buf[3]=='0' && buf[4]=='0' && buf[5]=='.' )
            return 1;
        }
        else
        {
            errorCounter++;
            //归零错误
        }
        
    }
    else
    {
        //do nothing
    }
    return 0;
}


int DetectHeightSensor()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    char distanceReqMsg[] = {0x55,0xaa,0x01,0x01,0x01,0x00};
    
    SetBusy(1);
    //DelayMs(30);
    SetUartSwitch(FUNC_HEIGHT);
    SendData(PORT_SENSOR,distanceReqMsg,5);
    SetBusy(0);
    
    int counter = 300;//在80ms内完成接收
    HeightSensorPID = 2;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        HeightSensorPID = 2;
        return 2;
    }
    else
    {
        HeightSensorPID = 1;
    }
    
    errorCounter  = 0;
    SetBusy(1);
    SetUartSwitch(FUNC_HEIGHT);
    SendData(PORT_SENSOR,"?H2\n",4);
    SetBusy(0);
    
    counter = 2000;//在2000ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            if( EventHeightDataReceived(buf))
                return 1;
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
*/