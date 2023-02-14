

#include "includes.h"

u8 heightMeasureStart = 0;
u32 gHeightUpdateTime = 0;

//���ģ����������
//1. �������ģ��  ���ù涨��Э��
//2. DYP-ME007Y50H V2
//���ģ����ڿ������Զ�ʶ�𳧼ң�����ʶ��汾��
u8 HeightSensorPID = 2;

int EventHeightDataReceived(char* str)
{
    int ret = 0;
    switch(str[0])
    {
    case '5'://�������
        if(str[1]=='R')//��R�� ���i��ֵ
        {
            user.height = 100*((str[2]-'0')%10)+10*((str[3]-'0')%10)+1*((str[4]-'0')%10)+0.1*((str[6]-'0')%10);
            if(user.height>HEIGHT_MAX)
                user.height = HEIGHT_MAX;
            if(user.height < HEIGHT_MIN)
                user.height = HEIGHT_MIN;
            
            
            user.status |= 0x04;
            gHeightUpdateTime = GetSystemTime();
        }
        ret = 1;
        break;
    case '7'://
        //do nothing 
        break;
    case '8'://
        break;
    case '9'://
        //do nothing at the moment
        break;
    default:
        break;
    }
    return ret;
}

int GetFirstHeightData()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    char distanceReqMsg[] = {0x55,0xaa,0x01,0x01,0x01,0x00};
    
    if(HeightSensorPID ==1)
    {
        SetBusy(1);
        SetUartSwitch(FUNC_HEIGHT);
        SendData(PORT_SENSOR,"?H2\n",4);
        SetBusy(0);
        int counter = 2000;//��2000ms����ɽ���
        while(GetUart1RxStatus(buf)==0 && counter)
        {
            counter--;
            DelayMs(1);
        }
        if(counter)
        {
            if(CheckReceivedString(buf))
            {
                return EventHeightDataReceived(buf);
            }
            else
            {
                errorCounter++;
                //У�����
            }
            
        }
        else
        {
            //do nothing
        }
        return 0;
    }
    else// if HeightSensorPID ==2
    {
        SetBusy(1);
        SetUartSwitch(FUNC_HEIGHT);
        SendData(PORT_SENSOR,distanceReqMsg,5);
        SetBusy(0);
        int counter = 200;//��2000ms����ɽ���
        while(GetUart1RxStatus(buf)==0 && counter)
        {
            counter--;
            DelayMs(1);
        }
        if(counter)
        {
            if(CheckReceivedString(buf))
            {
                return EventHeightDataReceived(buf);
            }
            else
            {
                errorCounter++;
                //У�����
            }
            
        }
        else
        {
            //do nothing
        }
        return 0;
    }
    
    
}




int SetHeightTest()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    
    SetBusy(1);
    DelayMs(30);
    SetUartSwitch(FUNC_HEIGHT);
    
    SendData(PORT_SENSOR,"?H3\n",4);
    SetBusy(0);
    
    int counter = 40;//��20ms����ɽ���
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
            //У�����
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
    
    int counter = 20;//��20ms����ɽ���
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
            //�������
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
    
    int counter = 300;//��80ms����ɽ���
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
    
    counter = 2000;//��2000ms����ɽ���
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
            //У�����
        }
    }
    else
    {
        //do nothing
    }
    
    return 0;
}