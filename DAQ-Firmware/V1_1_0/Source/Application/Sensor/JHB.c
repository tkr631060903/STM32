

#include "includes.h"



u32 gJHBUpdateTime = 0;
//
//
u8 gJHBWorkingState = 0;

//CE外面需要加上拉电阻
void JHBInit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //PB8   R23 OPSO
    //PB9   R24 OPCC
    //PA11  R-- CCMC
    
    //new//
    //PD0 CTRL
    //PD1 CE
    //PD2 Status
    
    //OPSO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//ABC
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
    
    //OPCC
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//ABC
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

void SetOPCC(u8 state)
{
    GPIO_WriteBit(GPIOD, GPIO_Pin_0, (BitAction)(state));
}
u8 GetOPCC()
{
    return GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_0);
}
void SetCCMC(u8 state)
{
    SetBusy(~state);
}
u8 GetCCMC()
{
    return GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_1);
}
u8 GetOPSO()
{
    return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
}

void SetOPCCLowPulse()
{
    u8 temp = GetOPCC();
    if(temp)
    {
        SetOPCC(0);
        delay_us(100);
        SetOPCC(1);
    }
    else
    {
        SetOPCC(1);
        delay_us(600);
        SetOPCC(0);
        delay_us(100);
        SetOPCC(1);
        delay_us(600);
    }
}
void JHBSleep()
{
    SetBusy(0); 
}

void JHBReset()
{
    int startTime = GetSystemTime();
    SetOPCCLowPulse();//
    DelayMs(1);
    SetOPCCLowPulse();//
    SetOPCCLowPulse();//
    DelayMs(1);
    JHBSleep();
    DelayMs(1);
    SetBusy(1);
    while(1)
    {
        if(GetSystemTime()-startTime>10)
            break;
        if(GetOPSO()==0)
        {
            delay_us(100);
            startTime = GetSystemTime();
            while(GetOPSO()==0)
            {
                if(GetSystemTime()-startTime>10)
                    break;
            }
            break;
        }
    }
}

/*
UpstreamData dsd;
char str[] = {0xBC,0x83,0x30,0xA5,0x30,0xA5,   0x30,0x31,0x32,0xA5,   0x35,0x36,0x37,0xA5,  0x60,0x61,0xA5,   0xD4,0xc6,0xc8,0xd4,0x00};
ParseUpstreamData(&dsd, str,strlen(str));
SendDownstreamData((DownstreamData*)(&dsd));
*/
int FreeDownstreamData(DownstreamData* dsd)
{
    /*
    int i;
    for(i=0;i<dsd->dataLength;i++)
    {
        if(dsd->data[i]==NULL)
        {
        }
        else
        {
            free(dsd->data[i]);
            dsd->data[i] = NULL;
        }
    }*/
    return 1;
}
int SendDownstreamData(DownstreamData* dsd)
{
    char idReqMsg[2*BUFFER_SIZE]={0xBC,0x00,0xA5};
    int dataIndex = 0;
    int i;
    
    if(dsd->command == 0xAA)
    {
        return 0;
    }
    idReqMsg[1] = dsd->command;
    
    //SetBusy(1);
    SetUartSwitch(FUNC_JHB);
    
    dataIndex = 3;
    if(dsd->dataLength)
    {
        for(i=0;i<dsd->dataLength;i++)
        {
            if(strlen(dsd->data[i]))
            {
                strcpy(idReqMsg+dataIndex,dsd->data[i]);
                dataIndex += strlen(dsd->data[i]);
                idReqMsg[dataIndex++] = 0xA5;
            }
            else
            {
                idReqMsg[dataIndex++] = 0x00;
                idReqMsg[dataIndex++] = 0xA5;
            }
        }
    }
    else
    {
        idReqMsg[dataIndex++] = 0xFF;
        idReqMsg[dataIndex++] = 0xA5;
    }
    idReqMsg[dataIndex++] = 0xD4;
    idReqMsg[dataIndex++] = 0xC6;
    idReqMsg[dataIndex++] = 0xC8;
    idReqMsg[dataIndex++] = 0xD4;
    
    SendData(PORT_SENSOR,idReqMsg,dataIndex);
    //SetBusy(0);
    
    return 1;
}

int FreeUpstreamData(UpstreamData* dsd)
{
    /*int i;
    for(i=0;i<dsd->dataLength;i++)
    {
        if(dsd->data[i]==NULL)
        {
        }
        else
        {
            free(dsd->data[i]);
            dsd->data[i] = NULL;
        }
    }*/
    return 1;
}
int ParseUpstreamData(UpstreamData* dsd, char* str, int len)
{
    char buffer[BUFFER_SIZE];
    if(len< 12 || str[0]!= 0xBC || str[1]&0x80 != 0x80 || str[3]!=0xA5 ||  str[len-4]!=0xD4 || str[len-3]!=0xC6 || str[len-2]!=0xC8 || str[len-1]!=0xD4 )
    {
        dsd->command = 0xAA;//自定义状态
        dsd->dataLength = 0;
        return 0;
    }
    dsd->command = str[1]&0x7F;
    dsd->status = str[2];
    int counter = 4;
    while(str[counter]!=0xA5 && counter < len-4)
    {
        dsd->errorCode[counter-4] = str[counter];
        counter ++;
    }
    if(counter>=len-4)
    {
        dsd->command = 0xAA;//自定义状态
        dsd->dataLength = 0;
        return 0;
    }
    counter ++;
    if(str[counter]==0xFF && str[counter+1]==0xA5)
    {
        dsd->dataLength = 0;
    }
    else
    {
        int dataIndex = counter;
        int i = 0, j= 0;
        dsd->dataLength = 0;
        while(dataIndex<len-4)
        {
            if(str[dataIndex]==0xA5)
            {
                buffer[i]=0;
                //dsd->data[dsd->dataLength] = (char*)malloc(i+1);
                //if(dsd->data[dsd->dataLength]==NULL)
                //{
                //    i=i;
                //    buffer[0]=1;
                //    break;
                //}
                for(j=0;j<i+1;j++)
                {
                    dsd->data[dsd->dataLength][j] = buffer[j];
                }
                i = 0;
                dsd->dataLength ++;
                dataIndex++;
            }
            else
            {
                buffer[i++] = str[dataIndex++];
            }
        }
    }
    return 1;
}

int EventJHBDataReceived(UpstreamData* usd, char* str, int len)
{
    int ret = 0;
    
    ret = ParseUpstreamData(usd, str, len);
    
    gJHBUpdateTime = GetSystemTime();
    
    return ret;
}


int CheckJHBReceivedString(char* str, int len)
{
    
    int tmp = GetUartSwitch();
    
    if(tmp != FUNC_JHB)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}
//发送dsd，并将接收到的usd存储。
int GetFirstJHBData(DownstreamData* dsd, UpstreamData* usd)
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    int strLen;
    int ret =0;
    //char idReqMsg[] = {0x00, 0x00, 0x7f, 0x03, 0x00, 0x10, 0x13 };
    
    //SetBusy(1);
    SetUartSwitch(FUNC_JHB);
    
    if(!SendDownstreamData(dsd))
    {
        //dsd->command = 0xAA
        if(dsd->command!=0xAA)
        {
            ret = 0;
            return ret;
        }
    }
    
    int counter = 2000;//在1000ms内完成接收
    gJHBUpdateTime = GetSystemTime() ;
    while((strLen = GetUart1RxStatus(buf))==0 && counter )
    {
        counter--;
        DelayMs(1);
    }
    
    //SetBusy(0);
    if(counter && strLen)
    {
        if(CheckJHBReceivedString(buf, strLen))
        {
            ret = EventJHBDataReceived(usd, buf, strLen);
        }
        else
        {
            errorCounter++;
            //校验错误
        }
        
    }
    else
    {
        ret = 0;//do nothing
    }
    
    return ret;
}

void StartJHBWeightMeasure()
{
    JHBReset();
    DelayMs(30);
    SetOPCC(0);
}

void EndJHBWeightMeasure()
{
    SetOPCC(1);
    JHBReset();
    JHBSleep();
}


//需要配合两个函数StartJHBWeightMeasure() EndJHBWeightMeasure()一起使用
int GetJHBWeightData()
{
    char sendStr[10];// =(char*)malloc(10);
    int ret = 0;
    
    gJHBUpdateTime = GetSystemTime() ;
    
    DownstreamData dsd;
    UpstreamData usd;
    
    dsd.command = 0xAA;//为AA时，不发送downstream
    dsd.dataLength = 0;
    strcpy(dsd.data[0] , sendStr);
    if(GetFirstJHBData(&dsd,&usd))
    {
        if(strlen(usd.data[0]))
        {
            user.weight = (float)atof(usd.data[0]);
            ret = 1;
        }
        else 
        {
        }
        
    }
    else
    {
        //do nothing
    }
    FreeUpstreamData(&usd);
    
    
    return ret;
    
}


int StartJHBParameterMeasure()
{
    int ret = 0;
    JHBReset();
    delay_us(300);
    
    
    DownstreamData dsd;
    UpstreamData usd;
    char str[4][10];
    
    dsd.command = 0x13;//为AA时，不发送downstream
    dsd.dataLength = 4;
    if(user.sex ==0)//men
    {
        str[0][0] = 0x31;
        str[0][1] = 0;
    }
    else 
    {
        str[0][0] = 0x30;
        str[0][1] = 0;
    }
    if( user.age<=99)
    {
        str[1][0]=user.age/10+'0';
        str[1][1]=user.age%10+'0';
        str[1][2]=0;
    }
    else
    {
        str[1][0]='0';
        str[1][1]=0;
    }
    if(user.height>=HEIGHT_MIN && user.height<HEIGHT_MAX)
    {
    }
    else
    {
        user.height = HEIGHT_MIN;
    }
    sprintf(str[2],"%d",(int)user.height);
    if(user.weight >=WEIGHT_MIN && user.weight < WEIGHT_MAX)
    {
    }
    else if(user.weight > WEIGHT_MAX)
    {
        user.weight = WEIGHT_MAX;
    }
    else if(user.weight < 0)
    {
        user.weight = 0;
    }
    else
    {
        //do nothing.
    }
    sprintf(str[3],"%d",(int)user.weight);
    strcpy(dsd.data[0],str[0]);
    strcpy(dsd.data[1],str[1]);
    strcpy(dsd.data[2],str[2]);
    strcpy(dsd.data[3],str[3]);
    
    ClearUart1RxBuffer();
    if(GetFirstJHBData(&dsd,&usd))
    {
        if(usd.status == 0x30)
            ret = 1;
        else 
            ret = 0;
    }
    
    FreeUpstreamData(&usd);
    
    SetOPCCLowPulse();
    
    return ret ;
}

void EndJHBParameterMeasure()
{
    SetOPCC(1);   
}

int GetJHBParameterDataDetail()
{
    char parameterId[20]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x22};
    char paramHasLim[20]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00};
    DownstreamData dsd;
    UpstreamData usd;
    static int iCounter =0;
    int errorCount = 0;
    int mark = 0;
    
    dsd.command = 0x03;
    dsd.dataLength = 1;
    
    
    for(iCounter=0;iCounter<21;iCounter++)
    {
        dsd.data[0][1]=0;
        dsd.data[0][0]=iCounter;
        
        if(iCounter == 20)
        {
            iCounter = 19;
            dsd.data[0][0]=0x1D;
            mark = 1;
        }
        else
        {
            user.parameter[iCounter] = 0;
        }
        ClearUart1RxBuffer();
        if(GetFirstJHBData(&dsd,&usd))
        {
            if(usd.command != 0xAA && usd.data[0][0]==parameterId[iCounter] &&  usd.dataLength>1)
            {
                user.parameter[iCounter] = atof(usd.data[1]);
                if(paramHasLim[iCounter])
                {
                    user.paramHighLimit[iCounter] = atof(usd.data[3]);
                    user.paramLowLimit[iCounter] = atof(usd.data[2]);
                }
                
                if(user.parameter[19])
                {
                    //已经有数据了，不再去写
                }
                else
                {
                    user.parameter[19] = atof(usd.data[1]);
                }
                
            }
            else
            {
                if(!mark)
                {   
                    user.parameter[iCounter] = 0;
                    errorCount ++;
                }
            }
            
        }
        else
        {
        }
        FreeUpstreamData(&usd);
                         
        if(mark)
            iCounter = 20;
        
    }
    user.parameterStatus = 'R';
    JHBSleep();
    if(errorCount >=20)
        return 0;
    else
        return 1;
}


int GetJHBParameterDataLoop()
{

    user.parameterStatus = 'T';
    if(!StartJHBParameterMeasure())
    {
        //error report
        return 0;
    }
    gJHBUpdateTime = GetSystemTime() ;
    while(1)
    {
        if(GetSystemTime()-gJHBUpdateTime>4000)
        {
            EndJHBParameterMeasure();
            //error report
            return 0;
        }
        if(GetOPSO())
        {
            delay_us(800);
            if(GetOPSO())
            { 
                //user.parameterStatus ='R';
                break;
            }
        }
    }
    EndJHBParameterMeasure();
    ClearUart1RxBuffer();
    DelayMs(1);
    GetJHBParameterDataDetail();//读取全部信息
#if SEND_RESULT_TWICE    
    char buf[12];
    strcpy(buf,"RU2-F-050HL");
    buf[11]= 0x0a;
    AddCheckString(buf);
    UART2_PrintArray(buf,12);
#endif
    return 1;
}





int StartJHBCalibMeasure()
{
    int ret = 0;
    JHBReset();
    delay_us(300);
    SetOPCC(1);
    delay_us(300);
    
    DownstreamData dsd;
    UpstreamData usd;
    
    dsd.command = 0x14;//为AA时，不发送downstream
    dsd.dataLength = 3;
    strcpy(dsd.data[0],"2");
    strcpy(dsd.data[1],"50");
    strcpy(dsd.data[2],"100");
    
    
    
    if(GetFirstJHBData(&dsd,&usd))
    {//usd.status == 0x40 &&
        if( usd.errorCode[0] ==0x00)
            ret = 1;
        else 
            ret = 0;
    }
    
    FreeUpstreamData(&usd);
    
    return ret ;
}

void EndJHBCalibMeasure()
{
    JHBReset();
    SetOPCC(1);
    JHBSleep();
}


int EnterJHBCalibFunction()
{

    int index =0;
    index = index;
    char statusReport[25];
    
    ///////////////////////////////////////////////////// 
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+0;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);          //0 finish
    
    if(!StartJHBCalibMeasure())
    {
        //error report
        EndJHBCalibMeasure();
        return 0;
    }

    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+1;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);          //1 finish
    
    gJHBUpdateTime = GetSystemTime() ;
    while(1)
    {
        if(GetSystemTime()-gJHBUpdateTime>2000)
        {
            EndJHBCalibMeasure();
            //error report
            return 0;
        }
        if(!GetOPSO())
        {
            break;
        }
    }
    gJHBUpdateTime = GetSystemTime() ;
    while(1)
    {
        if(GetSystemTime()-gJHBUpdateTime>1000)
        {
            EndJHBCalibMeasure();
            //error report
            return 0;
        }
        if(GetOPSO())
        {
            break;
        }
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+2;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);          //2 finish  50kg完成，需要进行100kg
    
    
    gJHBUpdateTime = GetSystemTime() ;
    while(1)
    {
        if(GetSystemTime()-gJHBUpdateTime>2000)
        {
            EndJHBCalibMeasure();
            //error report
            return 0;
        }
        if(!GetOPSO())
        {
            break;
        }
    }
    gJHBUpdateTime = GetSystemTime() ;
    while(1)
    {
        if(GetSystemTime()-gJHBUpdateTime>1000)
        {
            EndJHBCalibMeasure();
            //error report
            return 0;
        }
        if(GetOPSO())
        {
            break;
        }
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+3;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);          //3 finish  100kg完成，需要进行150kg
    
    DelayMs(1000);
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+4;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);          //4 finish  150kg完成，需要进行200kg
    
    DelayMs(500);
    
    
    EndJHBCalibMeasure();
    
    /////////////////////////////////////////////////////
    
    
    return 1;
}