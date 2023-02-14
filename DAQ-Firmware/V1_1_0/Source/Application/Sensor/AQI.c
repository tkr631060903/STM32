//空气质量  温度 湿度 PM2.5
#include "includes.h"




int EventAQIDataReceived(char* str)
{
    char sNum[5];
    int ret = 0;
    int num = 0;
    
    //debug(str);
    sNum[0]=str[3];
    sNum[1]=str[4];
    sNum[2]=str[5];
    sNum[3]=0;
    num = atoi(sNum);
    switch(str[2])
    {
    case 'T'://温度
        
        sys.temperature = num;
        ret = 1;
        break;
    case 'H'://
        
        
        sys.humidity = num;
        //do nothing 
        break;
    case 'P'://
        
        sys.pm25 = num;
        break;
    default:
        break;
    }
    return ret;
}




int SendAQIRequest(u8 type)
{
    
    SetBusy(1);
    //SetUartSwitch(FUNC_TEMP);
    if(type==0)
        SendData(PORT_SENSOR,"?E1------4B\n",12);
    else if(type ==1)
        SendData(PORT_SENSOR,"?E2------48\n",12);
    else if(type ==2)
        SendData(PORT_SENSOR,"?E3------49\n",12);
    else
        SendData(PORT_SENSOR,"?E4------4E\n",12);
    SetBusy(0);
    
    return 1;
}

//Type =3时为测试
//其他值为正常值
int CheckAQIData()
{
    char buf[2*BUFFER_SIZE];
    
    //SetUartSwitch(FUNC_TEMP);
    
    if(GetUart1RxStatus(buf))
    {
        if(CheckReceivedString(buf))
        {
            //if(type ==3)
            if(0)
            {
                buf[8]=0;
                if(strcmp(buf,"7ET025.0")==0)
                {
                    debug("Temp Check OK.\r\n");
                    return 1;
                }
                else
                {
                    debug("Temp Check Error.\r\n");
                    return 0;
                }
            }
            if(buf[0]=='7' && buf[1]=='E')
                return EventAQIDataReceived(buf);
            else 
            {
                debug("None sensor msg received.\r\n");
                return 0;
            }
        }
        else
        {
            //errorCounter++;
            //校验错误
        }
    }
    return 0;
}

//type:
//0 温度
//1 湿度
//2 PM2.5
//3 Test
int GetAQIData(u8 type)
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    int tmp = GetUartSwitch();
    int ret = 0;
    
    SendAQIRequest(type);
    
    int counter = 400;//在400ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            if(type ==3)
            {
                buf[8]=0;
                if(strcmp(buf,"7ET025.0")==0)
                {
                    debug("Temp Check OK.\r\n");
                    SetUartSwitch(tmp);
                    return 1;
                }
                else
                {
                    debug("Temp Check Error.\r\n");
                    SetUartSwitch(tmp);
                    return 0;
                }
            }
            if(buf[0]=='7' && buf[1]=='E')
            {
                ret = EventAQIDataReceived(buf);
                SetUartSwitch(tmp);
                return ret;
            }
            else 
            {
                debug("None sensor msg received.\r\n");
                SetUartSwitch(tmp);
                return 0;
            }
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
    SetUartSwitch(tmp);
    return 0;
}



