

#include "includes.h"



u32 gRFIDUpdateTime = 0;
//
//


int EventRFIDDataReceived(char* str)
{
    int ret = 0;
    
    gRFIDUpdateTime = GetSystemTime();
    
    if(str[20]==1)
    {
        return 0;//无卡
    }
    else if(str[20]==28)
    {
        if(str[16]==0x00)
            ret = 1;
    }
    else//12
    {
        if(str[4]==0x00)
        {
            user.id =(str[0]<<24)+(str[1]<<16)+(str[2]<<8)+(str[3]<<0);
            ret = 1;
        }
    }
    
    
    return ret;
}


int CheckRFIDReceivedString(char* str)
{

    int tmp = GetUartSwitch();
    
    if(tmp != FUNC_ID)
    {
        return 0;
    }
    else 
    {
        if(str[4]==0x00 || str[16]==0x00 || str[1]==0x00)
            return 1;
    }
    return 0;
}

int GetFirstRFIDData()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    char idReqMsg[] = {0x00, 0x00, 0x7f, 0x03, 0x00, 0x10, 0x13 };
    
    SetBusy(1);
    SetUartSwitch(FUNC_ID);
    SendData(PORT_SENSOR,idReqMsg,7);
    SetBusy(0);
    int counter = 2000;//在2000ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckRFIDReceivedString(buf))
        {
            return EventRFIDDataReceived(buf);
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

char checkSum(char* data, int offset, int length) 
{ 
    char temp = 0; 
    for (int i = offset; i < length + offset; i++) 
    {
        temp ^= data[i]; 
    }
    return temp; 
}


int StoreRFIDData16(char* str, u8 blockNum)
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    char idReqMsg[23] = {0x00, 0x7f, 0x14, 0x00, 0x12, 0x01 };
    int i;
    
    if(strlen(str)!= 16)
        return 0;
    idReqMsg[5] = blockNum;
    for(i=0;i<strlen(str);i++)
    {
        idReqMsg[6+i] = str[i];
    }
    idReqMsg[22] = checkSum(idReqMsg,2,20);
    
    ClearUart1RxBuffer();
    SetBusy(1);
    SetUartSwitch(FUNC_ID);
    SendData(PORT_SENSOR,idReqMsg,23);
    SetBusy(0);
    int counter = 2000;//在2000ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckRFIDReceivedString(buf))
        {
            return EventRFIDDataReceived(buf);
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
const u8 blockLUT[][2]=
    {
        0,1,
        2,4,
        5,6,
        8,9,
        10,12,
        13,14,//5
        16,17,
        18,20,
        21,22,
        24,25,
        26,28,//10
        29,30,
        32,33,
        34,36,
        37,38,
        40,41,//15
        42,44,
        45,46,
        48,49,
        50,52,
        53,54,//20
        56,57,
        58,60,
        61,62//23
    };
int StoreRFIDData32(char* str, u8 blockNum)
{
    char strdup[33];
    
    blockNum %= 24;
    
    strcpy(strdup, str);
    strdup[16]=0;
    if(StoreRFIDData16(strdup,blockLUT[blockNum][0])==0)
        return 0 ;
    
    if(StoreRFIDData16(str+16,blockLUT[blockNum][1])==0)
        return 0 ;
    else return 1;
}

int LoadRFIDData16(char* str, u8 blockNum)
{
    
    static int errorCounter  = 0;
    char idReqMsg[7] = {0x00, 0x7f, 0x04, 0x00, 0x11, 0x01,0x14 };
    
    idReqMsg[5] = blockNum;
    idReqMsg[6] = checkSum(idReqMsg,2,4);
    
    ClearUart1RxBuffer();
    SetBusy(1);
    SetUartSwitch(FUNC_ID);
    SendData(PORT_SENSOR,idReqMsg,7);
    SetBusy(0);
    int counter = 2000;//在2000ms内完成接收
    while(GetUart1RxStatus(str)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckRFIDReceivedString(str))
        {
            return EventRFIDDataReceived(str);
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

int LoadRFIDData32(char* str, u8 blockNum)
{
    blockNum %= 24;
    if(LoadRFIDData16(str,blockLUT[blockNum][0])==0)
        return 0 ;
    
    if(LoadRFIDData16(str+16,blockLUT[blockNum][0])==0)
        return 0 ;
    else return 1;
}