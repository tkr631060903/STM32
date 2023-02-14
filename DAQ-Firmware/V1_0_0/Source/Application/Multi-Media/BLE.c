#include "includes.h"


int SendBLEMsg(char* buf)
{
    int i;
    u8 tmp;
    char str[63]={0xff,0xff,0xff,0xff,0xff,0x02,0x20,0xdd};
    int len;
    int strlength = 8;
    
    str[strlength] = (char)strlen(buf);
    for(i=0;i<str[strlength];i++)
    {
        str[i+strlength+1]=buf[i];
    }
    len = i+strlength+1;
    
    tmp = str[strlength-2];
    for(i=strlength-1;i<len;i++)
    {
        tmp ^= str[i];
    }
    str[len]=tmp;
    
    SetBusy(1);
    SetUartSwitch(FUNC_BLE);
    SendData(PORT_COMM,str,len+1);
    SetBusy(0);
    
    return 1;
}

int SendTestResultToBLE(u8 part)
{
    char str[120];
    const char sex[]={'m','w'};
    if(part ==0)
        sprintf((char*)str,"1a:%d;h:%d;w:%d",(int)user.age,(int)(10*user.height),(int)(10*user.weight));
    else 
        sprintf((char*)str,"2l:%d;s:%c;p:%d",(int)(1000*user.LBM),sex[user.sex],(int)user.athlete);
    SendBLEMsg(str);
    return 1;
}

int CheckReceivedBLEMsg(char* buf)
{
    u8 tmp;
    tmp = buf[0];
    tmp ^= buf[1];
    tmp ^= buf[2];
    if(tmp == buf[3])
        return 1;
    else return 0;
}


int CheckBLEMsg()
{
    char buf[25];
    if(GetUart3RxStatus(buf))
    {
        if(CheckReceivedBLEMsg(buf))
        {
            if(buf[0]==0x81 )
            {
                return 0;
            }
            else if(buf[0]==0x82)
            {
                return 0-buf[1];
            }
            else if(buf[0]==(0xdd^0x20))
            {
                return buf[2];
            }
            else return -999;
        }
        else
        {
            //errorCounter++;
            return -777;//Ð£Ñé´íÎó
        }
    }
    return -888;
}