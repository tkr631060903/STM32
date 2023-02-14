#include "includes.h"

const  char SoftwareVersionInfo[]= {'D','A','Q','-',BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3, BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1, '-', BUILD_HOUR_CH0,BUILD_HOUR_CH1,BUILD_MIN_CH0,BUILD_MIN_CH1,BUILD_SEC_CH0,BUILD_SEC_CH1,'\0'};
 
//频率：int/Hz，100-10000；
//持续时间：int/ms；>20；
//是否阻塞：int,1/0,持续时间大于200建议非阻塞；
int TFCCallbackBeep(cJSON* root, cJSON* retObj)
{
    if(!IsBeepEnabled())
    {
        cJSON_AddStringToObject(retObj, "report", "Beep is disabled while Ch25 is in PWM mode." );
        return 0;
    }
    static u32 freq=1000,duration=200,block=0;
    if(cJSON_GetObjectItem(root,"freq")!=NULL) freq= cJSON_GetObjectItem(root,"freq")->valueint;
    if(cJSON_GetObjectItem(root,"duration")) duration = cJSON_GetObjectItem(root,"duration")->valueint;
    if(cJSON_GetObjectItem(root,"block")) block = cJSON_GetObjectItem(root,"block")->valueint;
    if(freq<100 || freq>10000)
    {
        cJSON_AddStringToObject(retObj, "report", "Freq should be in the range 100-10000." );
        return 0;
    }
    if(duration<20 || (block&& duration>3000))
    {
        cJSON_AddStringToObject(retObj, "report", "Duration should be >=20.If block mode, duration should <3000" );
        return 0;
    }
    if(block)
    {
        ModelBeep(freq,duration);
    }
    else
    {
        Beep(freq,duration);
    }
    return 1;
}

//播放音乐
int TFCCallbackMusic(cJSON* root, cJSON* retObj)
{
    if(!IsBeepEnabled())
    {
        cJSON_AddStringToObject(retObj, "report", "Beep is disabled while Ch25 is in PWM mode." );
        return 0;
    }
    
    static u32 music_id=2, duration=200;
    if(cJSON_GetObjectItem(root,"music_id")) music_id= cJSON_GetObjectItem(root,"music_id")->valueint;
    if(cJSON_GetObjectItem(root,"duration")) duration = cJSON_GetObjectItem(root,"duration")->valueint;
    if(duration<50 || duration>500)
    {
        cJSON_AddStringToObject(retObj, "report", "Duration should be in the range 50-500" );
        return 0;
    }
    PlayMusicMultiThreadInit(music_id,duration);
    SetTimer(5,duration/10,PlayMusicMultiThread);
    return 1;
}
//获取软件版本号
int TFCCallbackSwVersion(cJSON* root, cJSON* retObj)
{
    cJSON_AddStringToObject(retObj, "sw_version", SoftwareVersionInfo);
    return 1;
}
//写E2PROM
int TFCCallbackEepromWrite(cJSON* root, cJSON* retObj)
{
    char nullStr[2] ="";
    char* str = NULL;
    if(cJSON_GetObjectItem(root,"eeprom")) str = cJSON_GetObjectItem(root,"eeprom")->valuestring;
    else str = nullStr;
    if(str == NULL || strlen(str)>64)
    {
        cJSON_AddStringToObject(retObj, "report", "User defined string length should not be <64." );
        return 0;
    }
    strcpy(sys.userString,str);
    return StoreSystemParam();
}
//读E2PROM
int TFCCallbackEepromRead(cJSON* root, cJSON* retObj)
{
    if(GetSystemParam())//IAP读取成功
    {
        if(sys.updateCheck == 1235396309)//存的信息也不对
        {
            cJSON_AddStringToObject(retObj, "eeprom", sys.userString);
            return 1;
        }
        else//读出来的信息是错的
        {   
        }
    }
    else    //IAP信息读取失败
    {
    }
    cJSON_AddStringToObject(retObj, "eeprom", "" );
    return 0;
}
//自检
int TFCCallbackSelfTest(cJSON* root, cJSON* retObj)
{
    char ss[1500];
    char buf[50];
    int retVal = 1;
    retVal = GetErrorStatus(buf);
    ClearDoErrorFlag();
    sprintf(ss,"DO:%s,",buf);
    
    
    float temp;
    //temperature
    if(GetDoADCVal(0,3,&temp))
    {
        sprintf(buf,"ch0-11 temperature %fC,",temp);
        strcat(ss,buf);
    }
    if(GetDoADCVal(1,3,&temp))
    {
        sprintf(buf,"ch12-23 temperature %fC,",temp);
        strcat(ss,buf);
    }
    
    //REF voltage
    if(GetDoADCVal(0,0,&temp))
    {
        sprintf(buf,"ch0-11 REF voltage %fV[typ 1.235V],",temp);
        strcat(ss,buf);
    }
    if(GetDoADCVal(1,0,&temp))
    {
        sprintf(buf,"ch12-23 REF voltage %fV[typ 1.235V],",temp);
        strcat(ss,buf);
    }
    
    //Supply voltage
    if(GetDoADCVal(0,1,&temp))
    {
        sprintf(buf,"ch0-11 supply voltage %fV[typ 12V],",temp);
        strcat(ss,buf);
    }
    if(GetDoADCVal(1,1,&temp))
    {
        sprintf(buf,"ch12-23 supply voltage %fV[typ 12V],",temp);
        strcat(ss,buf);
    }
    
    //VLDO voltage
    if(GetDoADCVal(0,2,&temp))
    {
        sprintf(buf,"ch0-11 VLDO voltage %fV[typ 5],",temp);
        strcat(ss,buf);
    }
    if(GetDoADCVal(1,2,&temp))
    {
        sprintf(buf,"ch12-23 VLDO voltage %fV[typ 5V],",temp);
        strcat(ss,buf);
    }
    
    int i;
    
    for(i=0;i<24;i++)
    {
        //channel voltage
        if(GetDoADCVal(i/12,i%12+0x10,&temp))
        {
            sprintf(buf,"ch%d voltage %fV,",i,temp);
            strcat(ss,buf);
        }
    }
    
    cJSON_AddStringToObject(retObj, "report", ss);
    return retVal;
}
int TFCCallbackUartWrite(cJSON* root, cJSON* retObj)
{
    static u32 plain_text = 1, uart_id = 4;
    if(cJSON_GetObjectItem(root,"plain_text")) plain_text = cJSON_GetObjectItem(root,"plain_text")->valueint;
    if(plain_text) plain_text = 1;
    if(cJSON_GetObjectItem(root,"uart_id")) uart_id = cJSON_GetObjectItem(root,"uart_id")->valueint;
    if(uart_id<1 || uart_id >5)
    {
        cJSON_AddStringToObject(retObj, "report", "uart_id should in the range 1-5." );
        return 0;
    }
    if(plain_text)//字符串
    {
        char *str=NULL;
        str =  cJSON_GetObjectItem(root,"text_content")->valuestring;
        if(str==NULL || strlen(str)==0)
        {
            cJSON_AddStringToObject(retObj, "report", "Data to send should not be empty." );
            return 0;
        }
        if(strlen(str)>500)
        {
            cJSON_AddStringToObject(retObj, "report", "Data length should <=500" );
            return 0;
        }
        return SendData(uart_id,str, strlen(str));
    }
    else
    {
        char buf[510];
        int i;
        
        cJSON* binary_content = cJSON_GetObjectItem(root,"binary_content");

        int arraySize = cJSON_GetArraySize(binary_content);
        if(arraySize>500)
        {
            cJSON_AddStringToObject(retObj, "report", "Data length should <=500" );
            return 0;
        }
        if(!arraySize)
        {
            cJSON_AddStringToObject(retObj, "report", "binary_content should not be empty." );
            return 0;
        }
        for(i=0;i<arraySize;i++)
        {
            buf[i] = cJSON_GetArrayItem(binary_content, i)->valueint;
        }
        
        return SendData(uart_id,buf, arraySize);
    }
}

int TFCCallbackUartRead(cJSON* root, cJSON* retObj)
{
    char buf[510];
    int rxBufLen = 0;
    static u32 plain_text = 1, uart_id = 4;
    if(cJSON_GetObjectItem(root,"plain_text")) plain_text = cJSON_GetObjectItem(root,"plain_text")->valueint;
    if(plain_text) plain_text = 1;
    if(cJSON_GetObjectItem(root,"uart_id")) uart_id = cJSON_GetObjectItem(root,"uart_id")->valueint;
    if(uart_id<1 || uart_id >5)
    {
        cJSON_AddStringToObject(retObj, "report", "uart_id should in the range 1-5." );
        return 0;
    }
    
    cJSON* binArray = NULL;
    int i;
    
    cJSON_AddNumberToObject(retObj, "plain_text",plain_text );
    
    if(plain_text==0 && (uart_id == 3 || uart_id == 4))
    {
        cJSON_AddItemToObject(retObj, "binary_content", cJSON_CreateIntArray((int*)buf,0));
        return 0;
    }
    
    rxBufLen = GetData(uart_id, buf);
    if(!rxBufLen)//没有读到数据
    {
        cJSON_AddItemToObject(retObj, "binary_content", cJSON_CreateIntArray((int*)buf,0));
        cJSON_AddStringToObject(retObj, "text_content", "");
    }
    else//读到了数据
    {
        if(plain_text)//string模式
        {
            buf[rxBufLen]=0;
            cJSON_AddStringToObject(retObj, "text_content", buf);
        }
        else//二进制数组
        {
            binArray = cJSON_CreateArray();
            for(i=0;i<rxBufLen;i++)
            {
                cJSON_AddItemToArray(binArray,cJSON_CreateNumber((double)buf[i]));
            }
            cJSON_AddItemToObject(retObj, "binary_content", binArray);
        }
    }
    ClearTFCEvent(EVENT_UART);
    return rxBufLen;
}


int TFCCallbackCanWrite(cJSON* root, cJSON* retObj)
{
    char txBuf[8];
    int i;
    
    //获取参数并检验参数是否正常
    static u32 can_channel = 1, is_ext_id=1, can_id = 0, repeat_msg_no=1, repeat_duty=200,clear_rx_buf = 0;
    static s32 repeat_sending = 0;
    
    if(cJSON_GetObjectItem(root,"can_channel")) can_channel = cJSON_GetObjectItem(root,"can_channel")->valueint;
    if(can_channel<1) can_channel = 1;
    else if(can_channel>2) can_channel =2 ;
    
    if(cJSON_GetObjectItem(root,"is_ext_id")) is_ext_id = cJSON_GetObjectItem(root,"is_ext_id")->valueint;
    if(is_ext_id) is_ext_id = 1;
    
    if(cJSON_GetObjectItem(root,"can_id")) can_id = cJSON_GetObjectItem(root,"can_id")->valueint;
    
    if(cJSON_GetObjectItem(root,"repeat_sending")) repeat_sending = cJSON_GetObjectItem(root,"repeat_sending")->valueint;
    if(repeat_sending>0) repeat_sending = 1;
    else if(repeat_sending<0) repeat_sending = -1;
    
    if(cJSON_GetObjectItem(root,"repeat_msg_no")) repeat_msg_no = cJSON_GetObjectItem(root,"repeat_msg_no")->valueint;
    if(repeat_msg_no<1) repeat_msg_no = 1;
    else if(repeat_msg_no>CAN_BUFFER_SIZE-1) repeat_msg_no = CAN_BUFFER_SIZE-1;
    
    if(cJSON_GetObjectItem(root,"repeat_duty")) repeat_duty = cJSON_GetObjectItem(root,"repeat_duty")->valueint;
    if(repeat_duty<10) repeat_duty = 10;
    
    
    
    //若需停止定时器则直接执行停止定时器的命令
    if(repeat_sending<0)
    {
        if(can_channel==1)//ch1 TMR7 , ch2 TMR8
        {
            KillTimer(7);
        }
        else 
        {
            KillTimer(8);
        }
        return 1;
    }
    
    cJSON* binary_content = cJSON_GetObjectItem(root,"binary_content");
    
    int arraySize = cJSON_GetArraySize(binary_content);
    if(arraySize>8)
    {
        cJSON_AddStringToObject(retObj, "report", "Data length should <=8" );
        return 0;
    }
    /*if(!arraySize)
    {
        cJSON_AddStringToObject(retObj, "report", "binary_content should not be empty." );
        return 0;
    }*/
    for(i=0;i<arraySize;i++)
    {
        txBuf[i] = cJSON_GetArrayItem(binary_content, i)->valueint;
    }
    
    //将所有发送的命令存入历史发送缓冲区
    if(can_channel==1)
    {
        CAN1_Tx_Index++;
        if(CAN1_Tx_Index>=CAN_BUFFER_SIZE)
            CAN1_Tx_Index =0;
        if(is_ext_id)
        {
            CAN1_Tx_Buffer[CAN1_Tx_Index].IDE = CAN_ID_EXT;
            CAN1_Tx_Buffer[CAN1_Tx_Index].ExtId = can_id;
        }
        else
        {
            CAN1_Tx_Buffer[CAN1_Tx_Index].IDE = CAN_ID_STD;
            CAN1_Tx_Buffer[CAN1_Tx_Index].StdId = can_id;
        }
        
        CAN1_Tx_Buffer[CAN1_Tx_Index].RTR = CAN_RTR_DATA;
        CAN1_Tx_Buffer[CAN1_Tx_Index].DLC = arraySize;
        memcpy(CAN1_Tx_Buffer[CAN1_Tx_Index].Data, txBuf, arraySize);
    }
    else
    {
        CAN2_Tx_Index ++;
        if(CAN2_Tx_Index>=CAN_BUFFER_SIZE)
            CAN2_Tx_Index = 0;
        if(is_ext_id)
        {
            CAN2_Tx_Buffer[CAN2_Tx_Index].IDE = CAN_ID_EXT;
            CAN2_Tx_Buffer[CAN2_Tx_Index].ExtId = can_id;
        }
        else
        {
            CAN2_Tx_Buffer[CAN2_Tx_Index].IDE = CAN_ID_STD;
            CAN2_Tx_Buffer[CAN2_Tx_Index].StdId = can_id;
        }
        
        CAN2_Tx_Buffer[CAN2_Tx_Index].RTR = CAN_RTR_DATA;
        CAN2_Tx_Buffer[CAN2_Tx_Index].DLC = arraySize;
        memcpy(CAN2_Tx_Buffer[CAN2_Tx_Index].Data, txBuf, arraySize);
    }
    
    if(cJSON_GetObjectItem(root,"clear_rx_buf")) clear_rx_buf = cJSON_GetObjectItem(root,"clear_rx_buf")->valueint;
    if(clear_rx_buf) clear_rx_buf = 1;
    if(clear_rx_buf)
    {
        ClearCanRxBuffer(can_channel);
    }
    
    if(repeat_sending)//ch1 TMR7 , ch2 TMR8
    {
        if(can_channel==2)//CAN2
        {
            CAN2_TX_Repeat_No = repeat_msg_no;
            SetTimer(8,repeat_duty/10,Can2RepeatSendingCallback);
        }
        else//CAN 1
        {
            CAN1_TX_Repeat_No = repeat_msg_no;
            SetTimer(7,repeat_duty/10,Can1RepeatSendingCallback);
        }
        return 1;
    }
    else
    {
        if(can_channel==2)//CAN2
        {
            return CAN_SendData(2,&(CAN2_Tx_Buffer[CAN2_Tx_Index]));
        }
        else //CAN 1
        {
            return CAN_SendData(1,&(CAN1_Tx_Buffer[CAN1_Tx_Index]));
        }
    }
    
    
    
    
}
int TFCCallbackCanRead(cJSON* root, cJSON* retObj)
{
    int i;
    static u32 can_channel = 1;
    
    ClearTFCEvent(EVENT_CAN);
    
    if(cJSON_GetObjectItem(root,"can_channel")) can_channel = cJSON_GetObjectItem(root,"can_channel")->valueint;
    if(can_channel<1) can_channel = 1;
    else if(can_channel>2) can_channel =2 ;
    
    cJSON* packetArray = NULL;
    packetArray = cJSON_CreateArray();
    int msgInFifo =0;
    //int GetCan2RxStatus(CanRxMsg* rxMsg)
    do
    {
        CanRxMsg rxMsg;
        
        if(can_channel==2)
        {
            msgInFifo = GetCan2RxStatus(&rxMsg);
        }
        else//channel 1
        {
            msgInFifo = GetCan1RxStatus(&rxMsg);
        }
        if(!msgInFifo)
        {
            cJSON_AddNumberToObject(retObj, "msg_in_fifo", 0);
            cJSON_AddStringToObject(retObj, "report", "GetCanRxStatus return 0" );
            return 0;
        }
        
        //添加回复元素
        cJSON* packetObject = cJSON_CreateObject();
        
        cJSON* binArray = NULL;
        binArray = cJSON_CreateArray();
        for(i=0;i<rxMsg.DLC;i++)
        {
            cJSON_AddItemToArray(binArray,cJSON_CreateNumber((double)rxMsg.Data[i]));
        }
        cJSON_AddItemToObject(packetObject, "binary_content", binArray);
        
        if(rxMsg.IDE)
        {
            cJSON_AddNumberToObject(packetObject, "is_ext_id", 1);
            cJSON_AddNumberToObject(packetObject, "can_id", rxMsg.ExtId);
        }
        else
        {
            cJSON_AddNumberToObject(packetObject, "is_ext_id", 0);
            cJSON_AddNumberToObject(packetObject, "can_id", rxMsg.StdId);
        }
        cJSON_AddItemToArray(packetArray,packetObject);
    }while(msgInFifo-1>0);
    cJSON_AddNumberToObject(retObj, "msg_in_fifo", msgInFifo-1);//除掉自己
    cJSON_AddItemToObject(retObj, "msg_packets", packetArray);
    return 1;
}
//AI
int TFCCallbackAiRead(cJSON* root, cJSON* retObj)
{    
    int i;
    int ch;
    
    cJSON* channel = cJSON_GetObjectItem(root,"channel");
    cJSON* ai_val = cJSON_CreateArray();
    
    int arraySize = cJSON_GetArraySize(channel);
    if(!arraySize)
    {
        cJSON_AddStringToObject(retObj, "report", "channel array should not be empty." );
        return 0;
    }
    for(i=0;i<arraySize;i++)
    {
        ch = cJSON_GetArrayItem(channel, i)->valueint;
        if(ch<0 || ch>22)
        {
            cJSON_AddStringToObject(retObj, "report", "Ch should range from 0 to 22");
            return 0;
        }
        cJSON_AddItemToArray(ai_val,cJSON_CreateNumber(GetAIVal(ch)));
    }
    cJSON_AddItemToObject(retObj, "ai_val", ai_val);
    return 1;
}

//AO
int TFCCallbackAoWrite(cJSON* root, cJSON* retObj)
{
    
    if(cJSON_GetObjectItem(root,"channel")==NULL)
    {
        cJSON_AddStringToObject(retObj, "report", "channel object is necessary." );
        return 0;
    }
    
    
    cJSON* channel = cJSON_GetObjectItem(root,"channel");
    //获取要设置的通道
    int channel_array_size = cJSON_GetArraySize(channel);
    if(!channel_array_size)
    {
        cJSON_AddStringToObject(retObj, "report", "channel array should not be empty." );
        return 0;
    }
    if(channel_array_size>4)
    {
        cJSON_AddStringToObject(retObj, "report", "Max channel 4 at one time.");
        return 0;
    }
    
    int i;
    int channel_array[4];
    float freq = 1;
    s32 vpp = 0, offset = 0, enable = 0, phase = 0;
    
    for(i=0;i<channel_array_size;i++)
    {
        int j;
        channel_array[i] = cJSON_GetArrayItem(channel, i)->valueint;
        if(channel_array[i]>3) channel_array[i] = 3;
        if(channel_array[i]<0) channel_array[i] = 0;
        
        cJSON* ao_val_array = cJSON_GetObjectItem(root,"ao_val_array");
        if(ao_val_array)
        {
            int ao_val_array_size = cJSON_GetArraySize(ao_val_array);
            if(ao_val_array_size)
            {
                if(ao_val_array_size>=200) ao_val_array_size = 200;
                for(j=0;j<ao_val_array_size;j++)
                {
                    if(cJSON_GetArrayItem(ao_val_array, j)->valueint>4095 || cJSON_GetArrayItem(ao_val_array, j)->valueint<0)
                    {
                        cJSON_AddStringToObject(retObj, "report", "Number in LUT must be in the range 0-4095");
                        return 0;
                    }
                }
                for(j=0;j<ao_val_array_size;j++)
                {
                    AoDdsParam[channel_array[i]].LUT[j]=cJSON_GetArrayItem(ao_val_array, j)->valueint;
                }
            }
            AoDdsParam[channel_array[i]].lenLUT = ao_val_array_size;
        }
        freq = AoDdsParam[channel_array[i]].frequency;
        if(cJSON_GetObjectItem(root,"freq")) freq = cJSON_GetObjectItem(root,"freq")->valuedouble;
        if(freq<0.00001) freq = 0.00001;
        if(freq>500) freq = 500;
        
        vpp = AoDdsParam[channel_array[i]].vpp;
        if(cJSON_GetObjectItem(root,"vpp")) vpp = cJSON_GetObjectItem(root,"vpp")->valueint;
        if(vpp<0) vpp = 0;
        if(vpp>30000) vpp = 30000;
        
        offset = AoDdsParam[channel_array[i]].voffset;
        if(cJSON_GetObjectItem(root,"offset")) offset = cJSON_GetObjectItem(root,"offset")->valueint;
        if(offset<0) offset = 0;
        if(offset>30000) offset = 30000;
        
        enable = AoDdsParam[channel_array[i]].bEnable;
        if(cJSON_GetObjectItem(root,"enable")) enable = cJSON_GetObjectItem(root,"enable")->valueint;
        if(enable) enable = 1;
        
        if(cJSON_GetObjectItem(root,"phase")) phase = cJSON_GetObjectItem(root,"phase")->valueint;
        if(phase<0) phase = 0;
        if(phase>=360) phase =phase%360;
        
        /*if(offset+vpp/2>30000)
            offset = (offset-vpp/2+30000)/2;
        if(offset-vpp/2<0)
            offset = (offset+vpp/2+0)/2;
        */
        if(offset+vpp/2<17000)
            SetAoSupplyVol(channel_array[i], 20000);
        else
            SetAoSupplyVol(channel_array[i], offset+vpp/2+3000);
        
        if(cJSON_GetObjectItem(root,"freq")) AoDdsParam[channel_array[i]].frequency = freq;
        if(cJSON_GetObjectItem(root,"vpp")) AoDdsParam[channel_array[i]].vpp = vpp;
        if(cJSON_GetObjectItem(root,"offset")) AoDdsParam[channel_array[i]].voffset = offset;
        if(cJSON_GetObjectItem(root,"ao_val_array")) 
        {
            AoDdsParam[channel_array[i]].actualIndex = 0;
            AoDdsParam[channel_array[i]].curIndex = 0;
        }
        if(cJSON_GetObjectItem(root,"phase")) 
        {
            if(AoDdsParam[channel_array[i]].lenLUT<1)
            {
                AoDdsParam[channel_array[i]].actualIndex = 0;
                AoDdsParam[channel_array[i]].curIndex = 0;
            }
            else
            {
                AoDdsParam[channel_array[i]].actualIndex = (float)phase/360.0*(AoDdsParam[channel_array[i]].lenLUT-1);
                AoDdsParam[channel_array[i]].curIndex = (u8)AoDdsParam[channel_array[i]].actualIndex;
            }
        }
        if(cJSON_GetObjectItem(root,"freq") ||cJSON_GetObjectItem(root,"ao_val_array")) 
        {
            AoDdsParam[channel_array[i]].step = AoDdsParam[channel_array[i]].lenLUT*AoDdsParam[channel_array[i]].frequency/AO_DAC_SPS;
        }
    }
    for(i=0;i<channel_array_size;i++)
    {
        if(cJSON_GetObjectItem(root,"enable")) AoDdsParam[channel_array[i]].bEnable = enable;
    }
    return 1;
}

//TFCCallbackAoChannelWrite
int TFCCallbackAoChannelWrite(cJSON* root, cJSON* retObj)
{/*
    static u32 channel = 0;
    static u32 vol = 0;
    
    if(cJSON_GetObjectItem(root,"channel")) channel = cJSON_GetObjectItem(root,"channel")->valueint;
    if(channel>3) channel = 3;
    
    if(cJSON_GetObjectItem(root,"vol")) vol = cJSON_GetObjectItem(root,"vol")->valueint;
    if(vol>30000) vol = 30000;
    
    if(vol<17000)
        SetAoSupplyVol(channel, 20000);
    else 
        SetAoSupplyVol(channel, vol+3000);
    //DelayMs(1000);
    SetAoChannelVol(channel, vol);
    
    return 1;
    */
    int i;
    u32 channel_array[4], vol_array[4];
    
    if(cJSON_GetObjectItem(root,"channel")==NULL)
    {
        cJSON_AddStringToObject(retObj, "report", "channel object is necessary." );
        return 0;
    }
    cJSON* channel = cJSON_GetObjectItem(root,"channel");
    
    if(cJSON_GetObjectItem(root,"vol")==NULL)
    {
        cJSON_AddStringToObject(retObj, "report", "vol object is necessary." );
        return 0;
    }
    cJSON* vol = cJSON_GetObjectItem(root,"vol");
    
    //获取要设置的通道
    int channel_array_size = cJSON_GetArraySize(channel);
    if(!channel_array_size)
    {
        cJSON_AddStringToObject(retObj, "report", "channel array should not be empty." );
        return 0;
    }
    if(channel_array_size>4)
    {
        cJSON_AddStringToObject(retObj, "report", "Max channel 4 at one time.");
        return 0;
    }
    
    int vol_array_size = cJSON_GetArraySize(vol);
    if(!vol_array_size)
    {
        cJSON_AddStringToObject(retObj, "report", "vol array should not be empty." );
        return 0;
    }
    if(vol_array_size>4)
    {
        cJSON_AddStringToObject(retObj, "report", "Max vol# 4 at one time.");
        return 0;
    }
    if(channel_array_size != vol_array_size)
    {
        cJSON_AddStringToObject(retObj, "report", "channel and vol size should be the same.");
        return 0;
    }
    
    for(i=0;i<channel_array_size;i++)
    {
        channel_array[i] = cJSON_GetArrayItem(channel, i)->valueint;
        if(channel_array[i]>3 )
        {
            cJSON_AddStringToObject(retObj, "report", "Ch only support 0-3!");
            return 0;
        }
        vol_array[i] = cJSON_GetArrayItem(vol, i)->valueint;
        if(vol_array[i]>30000 )
        {
            cJSON_AddStringToObject(retObj, "report", "Vol only support 0-30000!");
            return 0;
        }
    }
    
    for(i=0;i<channel_array_size;i++)
    {
        if(vol_array[i]<17000)
            SetAoSupplyVol(channel_array[i], 20000);
        else 
            SetAoSupplyVol(channel_array[i], vol_array[i]+3000);
        SetAoChannelVol(channel_array[i], vol_array[i]);
    }
    return 1;
}
/*
typedef struct
{
    u16 LUT[100];--
    u8 lenLUT;--
    float frequency;--
    float step;//重要==
    u8 curIndex;==
    float actualIndex;==
    float vpp;--
    float voffset;--
    u8 bEnable;--
}AoDDSType;
*/
int TFCCallbackDoWrite(cJSON* root, cJSON* retObj)
{
    int retVal =0;
    int channel_array[26];
    int do_val_array[26];
    int do_pwm_duty_array[26];
    static int do_pwm_freq = 1000;
    static int ocp =75 ;
    int i;
    
    if(cJSON_GetObjectItem(root,"pwm_mode")==NULL)
    {
        cJSON_AddStringToObject(retObj, "report", "pwm_mode object is necessary." );
        return 0;
    }
    if(cJSON_GetObjectItem(root,"channel")==NULL)
    {
        cJSON_AddStringToObject(retObj, "report", "channel object is necessary." );
        return 0;
    }
    
    u32 pwm_mode = cJSON_GetObjectItem(root,"pwm_mode")->valueint;
    cJSON* channel = cJSON_GetObjectItem(root,"channel");
    cJSON* do_val = NULL;
    cJSON* do_pwm_duty = NULL;
    
    //获取要设置的通道
    int channel_array_size = cJSON_GetArraySize(channel);
    if(!channel_array_size)
    {
        cJSON_AddStringToObject(retObj, "report", "channel array should not be empty." );
        return 0;
    }
    if(channel_array_size>26)
    {
        cJSON_AddStringToObject(retObj, "report", "Max channel 26 at one time.");
        return 0;
    }
    int do_val_size = 0, do_pwm_duty_size = 0;
    for(i=0;i<channel_array_size;i++)
    {
        channel_array[i] = cJSON_GetArrayItem(channel, i)->valueint;
        /*if(pwm_mode ==1 && channel_array[i]==24)
        {
            cJSON_AddStringToObject(retObj, "report", "Ch24 don't support PWM mode");
            return 0;
        }*/
        if(channel_array[i]>25 || channel_array[i]<0)
        {
            cJSON_AddStringToObject(retObj, "report", "Ch only support 0-25!");
            return 0;
        }
    }
    if(cJSON_GetObjectItem(root,"ocp")) ocp =  cJSON_GetObjectItem(root,"ocp")->valueint;
    if(ocp>75 || ocp<=0) ocp = 75;
    //PWM模式获取占空比，IO模式获取电平高低
    if(pwm_mode)
    {
        if(cJSON_GetObjectItem(root,"do_pwm_freq")) do_pwm_freq = cJSON_GetObjectItem(root,"do_pwm_freq")->valueint;
        if(do_pwm_freq<1 || do_pwm_freq>1000000)
        {
            cJSON_AddStringToObject(retObj, "report", "do_pwm_freq should be in the range 1-1000000" );
            return 0;
        }
        do_pwm_duty = cJSON_GetObjectItem(root,"do_pwm_duty");
        if(do_pwm_duty==NULL)
        {
            cJSON_AddStringToObject(retObj, "report", "do_pwm_duty object while pwm_mode =1 is necessary." );
            return 0;
        }
        do_pwm_duty_size = cJSON_GetArraySize(do_pwm_duty);
        if(do_pwm_duty_size>26)
        {
            cJSON_AddStringToObject(retObj, "report", "Max do_pwm_duty# 26 at one time.");
            return 0;
        }
        if(do_pwm_duty_size<1)
        {
            cJSON_AddStringToObject(retObj, "report", "do_pwm_duty_size should be >0" );
            return 0;
        }
        for(i=0;i<do_pwm_duty_size;i++)
        {
            do_pwm_duty_array[i] = cJSON_GetArrayItem(do_pwm_duty, i)->valueint;
        }
    }
    else
    {
        do_val = cJSON_GetObjectItem(root,"do_val");
        if(do_val==NULL)
        {
            cJSON_AddStringToObject(retObj, "report", "do_val object while pwm_mode =0 is necessary." );
            return 0;
        }
        do_val_size = cJSON_GetArraySize(do_val);
        if(do_val_size>26 || do_val_size<1)
        {
            cJSON_AddStringToObject(retObj, "report", "do_val size should be in the range 1-26.");
            return 0;
        }
        for(i=0;i<do_val_size;i++)
        {
            do_val_array[i]= cJSON_GetArrayItem(do_val, i)->valueint;
        }
    }
    
    retVal =1;
    //进行设置
    for(i=0;i<channel_array_size;i++)
    {
        //大电流通道
        if(channel_array[i]>=24)
        {
            if(pwm_mode)
            {
                if(channel_array[i]>24)//ch24不支持PWM模式
                    retVal*= DoUnprotectedPwmModeWrite(do_pwm_freq,500);//占空比只支持50%
            }
            else//IO mode
            {
                retVal*= DoUnprotectedIoModeWrite(channel_array[i]-24,do_val_array[i]);
            }
        }
        else//普通通道
        {
            if(cJSON_GetObjectItem(root,"ocp")) retVal*= DoOcpWrite(channel_array[i], ocp);
            if(pwm_mode)
            {
                if(cJSON_GetObjectItem(root,"do_pwm_freq")) retVal*= DoFreqWrite(channel_array[i],do_pwm_freq);//占空比只支持50%
                retVal*= DoPwmModeWrite(channel_array[i],do_pwm_duty_array[i]);//占空比只支持50%
            }
            else
            {
                retVal*= DoIoWrite(channel_array[i],do_val_array[i]);
            }
        }
    }
    
    if(retVal) retVal = 1;
    return retVal;
}
int TFCCallbackGetKey(cJSON* root, cJSON* retObj)
{
    char keyStatus[5];
    GetKeyStatus(keyStatus);
    keyStatus[4]=0;
    cJSON_AddStringToObject(retObj, "key_status", keyStatus);
    
    return 1;
}
//int ledDriverRegRead(u8 ch, u8 regAddr, u8 length, u8* readbackData);
//u8 ledDriverRegWrite(u8 ch, u8 regAddr, u8 length, u8* writeData);
int TFCCallbackDoRegWrite(cJSON* root, cJSON* retObj)
{
    u8 readbackData[8];
    int i;
    
    cJSON* regData = cJSON_GetObjectItem(root,"regData");
    int regData_size = cJSON_GetArraySize(regData);
    for(i=0;i<regData_size;i++)
    {
        readbackData[i]=cJSON_GetArrayItem(regData, i)->valueint;
    }
    
    u32 channel = cJSON_GetObjectItem(root,"channel")->valueint;
    u32 regAddr = cJSON_GetObjectItem(root,"regAddr")->valueint;
    u32 length = cJSON_GetObjectItem(root,"length")->valueint;
    return ledDriverRegWrite(channel,regAddr,length,readbackData);
}

int TFCCallbackDoRegRead(cJSON* root, cJSON* retObj)
{
    u8 readbackData[8];
    char ss[50]={0};
    char tmpStr[10];
    int i;
    u32 channel = cJSON_GetObjectItem(root,"channel")->valueint;
    u32 regAddr = cJSON_GetObjectItem(root,"regAddr")->valueint;
    u32 length = cJSON_GetObjectItem(root,"length")->valueint;
    int retVal = ledDriverRegRead(channel,regAddr,length,readbackData);
    if(!retVal) return 0;
    for(i=0;i<length;i++)
    {
        sprintf(tmpStr,"0x%x,",readbackData[i]);
        strcat(ss,tmpStr);
    }
    cJSON_AddStringToObject(retObj, "regData", ss);
    return 1;
}

int TFCCallbackSetEventMask(cJSON* root, cJSON* retObj)
{
    u32 event_mask = cJSON_GetObjectItem(root,"event_mask")->valueint;
    EVENT_MASK = event_mask;
    return 1;
}

int TFCCallbackTest(cJSON* root, cJSON* retObj)
{
    //GetCan1RxStatus(CanRxMsg* rxMsg);
    /*CanRxMsg rxMsg;
    if(GetCan2RxStatus(&rxMsg))
    {
        cJSON_AddNumberToObject(retObj, "DLC", rxMsg.DLC);
        cJSON_AddNumberToObject(retObj, "ExtId", rxMsg.ExtId);
        cJSON_AddNumberToObject(retObj, "StdId", rxMsg.StdId);
        return  1;
    }
    else 
    {
        return 0;
    }*/
//    void CAN2_TxMessage(u8 bExtAddress, u32 can_id, int dataLen, u8 *txBuf);
    u32 duty = cJSON_GetObjectItem(root,"duty")->valueint;
    Timer3Init(20000);
    SetAoVoltageCtrlDuty(duty);
    return 1;
}

int TFCCallbackGetError(cJSON* root, cJSON* retObj)
{
    int retVal;
    char buf[33];
    retVal = GetErrorStatus(buf);
    ClearDoErrorFlag();
    cJSON_AddStringToObject(retObj, "error_status", buf);
    return retVal;
}

//TFCCallbackIOOperation
int TFCCallbackIOOperation(cJSON* root, cJSON* retObj)
{
    /*
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    GPIO_WriteBit(GPIOE, GPIO_Pin_9, (BitAction)1);
    GPIO_WriteBit(GPIOE, GPIO_Pin_11, (BitAction)1);
    
    //PTD8,9,10,11  KEY IN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
    
    int a = GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14);
    
    */
    char* io = NULL;
    
    static u32 port = 2, pin = 13, bWrite = 0, io_val = 1;
    if(cJSON_GetObjectItem(root,"io")) io = cJSON_GetObjectItem(root,"io")->valuestring;
    if(io!=NULL && strlen(io)!=0)
    {
        if(io[0]>='a' && io[0]<='z')
            port = io[0]-'a';
        else if(io[0]>='A' && io[0]<='Z')
            port = io[0]-'A';
        else 
        {
            cJSON_AddStringToObject(retObj, "report", "io should be like the format 'A11'");
            return 0;
        }
        if(strlen(io)>3)
        {
            cJSON_AddStringToObject(retObj, "report", "io should be like the format 'A11'");
            return 0;
        }
        if(strlen(io)==2)
        {
            if(io[1]>='0' && io[1]<='9')
            {
                pin = io[1]-'0';
            }
            else
            {
                cJSON_AddStringToObject(retObj, "report", "io should be like the format 'A7'");
                return 0;
            }
        }
        else if(strlen(io)==3)
        {
            if(io[2]>='0' && io[2]<='9' && io[1]=='1')
            {
                pin = io[2]-'0'+10;
            }
            else
            {
                cJSON_AddStringToObject(retObj, "report", "io should be like the format 'A11' or 'E8'");
                return 0;
            }
        }
        if(pin>15 )
        {
            pin = 15;
            cJSON_AddStringToObject(retObj, "report", "io pin range 0-15'");
            return 0;
        }
        if(port>4 )
        {
            port = 0;
            cJSON_AddStringToObject(retObj, "report", "io port range A-E'");
            return 0;
        }
    }
    
    char* operation = NULL;
    if(cJSON_GetObjectItem(root,"operation")) operation = cJSON_GetObjectItem(root,"operation")->valuestring;
    if(strcmp(operation,"read")==0)
        bWrite = 0;
    else if(strcmp(operation,"write")==0)
        bWrite = 1;
    
    if(cJSON_GetObjectItem(root,"io_val")) io_val = cJSON_GetObjectItem(root,"io_val")->valueint;
    if(io_val) io_val =1;
    
    //执行操作
    const u16 GPIO_PIN_LIST[16]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15};
    GPIO_TypeDef* GPIO_PORT_LIST[5]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE};
    GPIO_InitTypeDef GPIO_InitStructure;
    if(bWrite)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LIST[pin];
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIO_PORT_LIST[port],&GPIO_InitStructure);
        GPIO_WriteBit(GPIO_PORT_LIST[port], GPIO_PIN_LIST[pin], (BitAction)io_val);
    }
    else
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LIST[pin];
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIO_PORT_LIST[port],&GPIO_InitStructure);
        int level = GPIO_ReadInputDataBit(GPIO_PORT_LIST[port],GPIO_PIN_LIST[pin]);
        if(level) level = 1;
        cJSON_AddNumberToObject(retObj, "io_val", level);
    }
    
    return 1;
}

//CAN波特率
int TFCCallbackCanBaud(cJSON* root, cJSON* retObj)
{
    static u32 can_channel = 1, baud_rate = 500;
    
    if(cJSON_GetObjectItem(root,"can_channel")) can_channel = cJSON_GetObjectItem(root,"can_channel")->valueint;
    if(can_channel!=2)  can_channel = 1;
    
    if(cJSON_GetObjectItem(root,"baud_rate")) baud_rate = cJSON_GetObjectItem(root,"baud_rate")->valueint;
    if(baud_rate<15) baud_rate = 10;
    else if(baud_rate<27) baud_rate =20;
    else if(baud_rate<42) baud_rate = 33;
    else if(baud_rate<56) baud_rate = 50;
    else if(baud_rate<73) baud_rate = 62;
    else if(baud_rate<90) baud_rate = 83;
    else if(baud_rate<113) baud_rate = 100;
    else if(baud_rate<170) baud_rate = 125;
    else if(baud_rate<375) baud_rate = 250;
    else if(baud_rate<625) baud_rate = 500;
    else if(baud_rate<775) baud_rate = 750;
    else if(baud_rate<900) baud_rate = 800;
    else  baud_rate = 1000;
    
    if(can_channel==1)
    {
        if(baud_rate!=sys.can_baud_rate_default[0])
        {
            //CAN1_Config(baud_rate);
            CAN_Baud_Config(1, baud_rate);
            sys.can_baud_rate_default[0]= baud_rate;
            StoreSystemParam();
        }
    }
    else
    {
        if(baud_rate!=sys.can_baud_rate_default[1])
        {
            //CAN2_Config(baud_rate);
            CAN_Baud_Config(2, baud_rate);
            sys.can_baud_rate_default[1]= baud_rate;
            StoreSystemParam();
        }
    }
    
    return 1;
}


//TFCCallbackUartBaud
int TFCCallbackUartBaud(cJSON* root, cJSON* retObj)
{
    static u32 uart_id = 1, baud_rate = 115200;
    
    if(cJSON_GetObjectItem(root,"uart_id")) uart_id = cJSON_GetObjectItem(root,"uart_id")->valueint;
    if(uart_id<1)  uart_id = 1;
    if(uart_id>5)  uart_id = 5;
    
    if(cJSON_GetObjectItem(root,"baud_rate")) baud_rate = cJSON_GetObjectItem(root,"baud_rate")->valueint;
    if(baud_rate<110) baud_rate = 110;
    if(baud_rate>2000000) baud_rate = 2000000;
    
    UART_Config(uart_id,baud_rate);
    return 1;
}




//TFCCallbackSetDebugMode
int TFCCallbackTFCConfig(cJSON* root, cJSON* retObj)
{
    int i;
    
    
    
    cJSON* uart_baud_rate_default = cJSON_GetObjectItem(root,"uart_baud_rate_default");
    int uart_baud_rate_default_array_size = cJSON_GetArraySize(uart_baud_rate_default);
    if(uart_baud_rate_default_array_size!=5 && uart_baud_rate_default_array_size!=0)
    {
        cJSON_AddStringToObject(retObj, "report", "There should be 5 number in the array" );
        return 0;
    }
    for(i=0;i<uart_baud_rate_default_array_size;i++)
    {
        sys.uart_baud_rate_default[i] = cJSON_GetArrayItem(uart_baud_rate_default, i)->valueint;
        if(sys.uart_baud_rate_default[i]<110) sys.uart_baud_rate_default[i] = 110;
        if(sys.uart_baud_rate_default[i]>2000000) sys.uart_baud_rate_default[i] = 2000000;
    }
    
    cJSON* can_baud_rate_default = cJSON_GetObjectItem(root,"can_baud_rate_default");
    int can_baud_rate_default_array_size = cJSON_GetArraySize(can_baud_rate_default);
    if(can_baud_rate_default_array_size!=2 && can_baud_rate_default_array_size!=0)
    {
        cJSON_AddStringToObject(retObj, "report", "There should be 2 number in the array" );
        return 0;
    }
    for(i=0;i<can_baud_rate_default_array_size;i++)
    {
        sys.can_baud_rate_default[i] = cJSON_GetArrayItem(can_baud_rate_default, i)->valueint;
        if(sys.can_baud_rate_default[i]<15) sys.can_baud_rate_default[i] = 10;
        else if(sys.can_baud_rate_default[i]<27) sys.can_baud_rate_default[i] =20;
        else if(sys.can_baud_rate_default[i]<42) sys.can_baud_rate_default[i] = 33;
        else if(sys.can_baud_rate_default[i]<56) sys.can_baud_rate_default[i] = 50;
        else if(sys.can_baud_rate_default[i]<73) sys.can_baud_rate_default[i] = 62;
        else if(sys.can_baud_rate_default[i]<90) sys.can_baud_rate_default[i] = 83;
        else if(sys.can_baud_rate_default[i]<113) sys.can_baud_rate_default[i] = 100;
        else if(sys.can_baud_rate_default[i]<170) sys.can_baud_rate_default[i] = 125;
        else if(sys.can_baud_rate_default[i]<375) sys.can_baud_rate_default[i] = 250;
        else if(sys.can_baud_rate_default[i]<625) sys.can_baud_rate_default[i] = 500;
        else if(sys.can_baud_rate_default[i]<775) sys.can_baud_rate_default[i] = 750;
        else if(sys.can_baud_rate_default[i]<900) sys.can_baud_rate_default[i] = 800;
        else  sys.can_baud_rate_default[i] = 1000;
    }
    
    if(cJSON_GetObjectItem(root,"debug_mode")) sys.debug_mode = cJSON_GetObjectItem(root,"debug_mode")->valueint;
    if(sys.debug_mode)  sys.debug_mode = 1;
    
    if(cJSON_GetObjectItem(root,"beep_after_ins")) sys.beep_after_ins = cJSON_GetObjectItem(root,"beep_after_ins")->valueint;
    if(sys.beep_after_ins)  sys.beep_after_ins = 1;
    
    if(cJSON_GetObjectItem(root,"auto_reset_interval")) sys.auto_reset_interval = cJSON_GetObjectItem(root,"auto_reset_interval")->valueint;
    if(sys.auto_reset_interval<15 && sys.auto_reset_interval>0)  sys.auto_reset_interval = 15;
    if(sys.auto_reset_interval<=0) sys.auto_reset_interval =0;
    
    if(cJSON_GetObjectItem(root,"start_up_music")) sys.start_up_music = cJSON_GetObjectItem(root,"start_up_music")->valueint;
    if(sys.start_up_music>3)  sys.start_up_music = 3;
    
    if(cJSON_GetObjectItem(root,"i2c_baud_rate_default")) sys.i2c_baud_rate_default = cJSON_GetObjectItem(root,"i2c_baud_rate_default")->valueint;
    if(sys.i2c_baud_rate_default>400000)  sys.i2c_baud_rate_default = 400000;
    if(sys.i2c_baud_rate_default<1000) sys.i2c_baud_rate_default = 1000;
    
    if(cJSON_GetObjectItem(root,"ins_exec_interval")) sys.ins_exec_interval = cJSON_GetObjectItem(root,"ins_exec_interval")->valueint;
    if(sys.ins_exec_interval>1000)  sys.ins_exec_interval = 1000;
    if(sys.ins_exec_interval<10) sys.ins_exec_interval =10;
    
    cJSON_AddStringToObject(retObj, "report", "It will take effect after reboot." );

    char str[333];
    PrintSystemParam(str);
    cJSON_AddStringToObject(retObj, "current_config", str );
    
    return StoreSystemParam();
    
     
}


//TFCCallbackCpuId
int TFCCallbackCpuId(cJSON* root, cJSON* retObj)
{
    char cpu_id[33];
    Get_Device_Guid(cpu_id);
    cJSON_AddStringToObject(retObj, "cpu_id", cpu_id);
    return 1;
}


//TFCCallbackCanFilter
int TFCCallbackCanFilter(cJSON* root, cJSON* retObj)
{
    u8 can_channel=1, filter_enable=1, fixed_id = 1, is_ext_id1=1, is_remote_frame1=1, is_ext_id2=1, is_remote_frame2=1;
    u32 can_id1 = 123, can_id2 = 123;
    if(cJSON_GetObjectItem(root,"can_channel")) 
    {
        can_channel = cJSON_GetObjectItem(root,"can_channel")->valueint;
        if(can_channel!=1)  can_channel = 2;
    }
    else
    {
        cJSON_AddStringToObject(retObj, "report", "can_channel object is necessary." );
        return 0;
    }
    
    if(cJSON_GetObjectItem(root,"filter_enable")) 
    {
        filter_enable = cJSON_GetObjectItem(root,"filter_enable")->valueint;
        if(filter_enable)  filter_enable = 1;
    }
    else
    {
        cJSON_AddStringToObject(retObj, "report", "filter_enable object is necessary." );
        return 0;
    }
    
    if(filter_enable==0)
    {
        CANFilterConfig(can_channel,0,0,0,0,0,0,0,0);
        return 1;
    }
    
    if(cJSON_GetObjectItem(root,"fixed_id")) 
    {
        fixed_id = cJSON_GetObjectItem(root,"fixed_id")->valueint;
        if(fixed_id)  fixed_id = 1;
    }
    else
    {
        cJSON_AddStringToObject(retObj, "report", "fixed_id object is necessary when enabled" );
        return 0;
    }
    
    if(cJSON_GetObjectItem(root,"can_id1")) 
    {
        can_id1 = cJSON_GetObjectItem(root,"can_id1")->valueint;
    }
    else
    {
        cJSON_AddStringToObject(retObj, "report", "can_id1 object is necessary when enabled" );
        return 0;
    }
    
    if(cJSON_GetObjectItem(root,"is_ext_id1")) 
    {
        is_ext_id1 = cJSON_GetObjectItem(root,"is_ext_id1")->valueint;
        if(is_ext_id1)  is_ext_id1 = 1;
    }
    else
    {
        cJSON_AddStringToObject(retObj, "report", "is_ext_id1 object is necessary when enabled" );
        return 0;
    }
    
    if(cJSON_GetObjectItem(root,"is_remote_frame1")) 
    {
        is_remote_frame1 = cJSON_GetObjectItem(root,"is_remote_frame1")->valueint;
        if(is_remote_frame1)  is_remote_frame1 = 1;
    }
    else
    {
        cJSON_AddStringToObject(retObj, "report", "is_remote_frame1 object is necessary when enabled" );
        return 0;
    }
    
    if(cJSON_GetObjectItem(root,"can_id2")) 
    {
        can_id2 = cJSON_GetObjectItem(root,"can_id2")->valueint;
    }
    else if(fixed_id==0)
    {
        cJSON_AddStringToObject(retObj, "report", "can_id2 object is necessary when it's mask" );
        return 0;
    }
    else
    {
        can_id2 = can_id1;
    }
    
    if(cJSON_GetObjectItem(root,"is_ext_id2")) 
    {
        is_ext_id2 = cJSON_GetObjectItem(root,"is_ext_id2")->valueint;
        if(is_ext_id2) is_ext_id2 = 1;
    }
    else if(fixed_id==0)
    {
        cJSON_AddStringToObject(retObj, "report", "is_ext_id2 object is necessary when it's mask" );
        return 0;
    }
    else
    {
        is_ext_id2 = is_ext_id1;
    }
    
    if(cJSON_GetObjectItem(root,"is_remote_frame2")) 
    {
        is_remote_frame2 = cJSON_GetObjectItem(root,"is_remote_frame2")->valueint;
        if(is_remote_frame2) is_remote_frame2 = 1;
    }
    else if(fixed_id==0)
    {
        cJSON_AddStringToObject(retObj, "report", "is_remote_frame2 object is necessary when it's mask" );
        return 0;
    }
    else
    {
        is_remote_frame2 = is_remote_frame1;
    }
    
    CANFilterConfig(can_channel, filter_enable, fixed_id, can_id1, is_ext_id1, is_remote_frame1, can_id2, is_ext_id2, is_remote_frame2);
    return 1;
}


//TFCCallbackCpuUsage
int TFCCallbackCpuUsage(cJSON* root, cJSON* retObj)
{
    cJSON_AddNumberToObject(retObj, "cpu_usage", GetCPULoadResult());
    return 1;
}

//TFCCallbackTfcInfo
int TFCCallbackTfcInfo(cJSON* root, cJSON* retObj)
{
    char buf[33];
    cJSON_AddStringToObject(retObj, "tfc_module", "daq");
    Get_Device_Guid(buf);
    cJSON_AddStringToObject(retObj, "cpu_id", buf);
    GetDeviceHardwareId(buf);
    cJSON_AddStringToObject(retObj, "hw_version", buf);
    cJSON_AddStringToObject(retObj, "sw_version", SoftwareVersionInfo);
    u32 id = sFLASH_ReadID();
    sprintf(buf,"%08x",id);
    cJSON_AddStringToObject(retObj, "flash_vender", buf);
    sFLASH_ReadUniqueID(buf);
    cJSON_AddStringToObject(retObj, "flash_guid", buf);
    
    cJSON_AddNumberToObject(retObj, "start_up_counter", user.startUpCounter);
    if(sys.chipEncrypted==0x24678)
    {
        cJSON_AddStringToObject(retObj, "security_level", "high");
    }
    else
    {
        cJSON_AddStringToObject(retObj, "security_level", "low");
    }
    switch(sys.resetStatus)
    {
    case RCC_FLAG_SFTRST:
        cJSON_AddStringToObject(retObj, "reset_status", "RCC_FLAG_SFTRST");
        break;
    case RCC_FLAG_IWDGRST:
        cJSON_AddStringToObject(retObj, "reset_status", "RCC_FLAG_IWDGRST");
        break;
    case RCC_FLAG_WWDGRST:
        cJSON_AddStringToObject(retObj, "reset_status", "RCC_FLAG_WWDGRST");
        break;
    case RCC_FLAG_PORRST:
        cJSON_AddStringToObject(retObj, "reset_status", "RCC_FLAG_PORRST");
        break;
    case RCC_FLAG_LPWRRST:
        cJSON_AddStringToObject(retObj, "reset_status", "RCC_FLAG_LPWRRST");
        break;
    case RCC_FLAG_PINRST:
        cJSON_AddStringToObject(retObj, "reset_status", "RCC_FLAG_PINRST");
        break;
    default://Unknown Reset
        cJSON_AddStringToObject(retObj, "reset_status", "Unknown Reset");
        break;
    }
    
    cJSON_AddNumberToObject(retObj, "UART1_BUFFER_SIZE", UART1_BUFFER_SIZE);
    cJSON_AddNumberToObject(retObj, "UART2_BUFFER_SIZE", UART2_BUFFER_SIZE);
    cJSON_AddNumberToObject(retObj, "UART3_BUFFER_SIZE", UART3_BUFFER_SIZE);
    cJSON_AddNumberToObject(retObj, "UART4_BUFFER_SIZE", UART4_BUFFER_SIZE);
    cJSON_AddNumberToObject(retObj, "UART5_BUFFER_SIZE", UART5_BUFFER_SIZE);
    
    cJSON_AddNumberToObject(retObj, "CAN_BUFFER_SIZE", CAN_BUFFER_SIZE);
    cJSON_AddNumberToObject(retObj, "SYSTEM_CLOCK", SYSTEM_CLOCK);
    cJSON_AddNumberToObject(retObj, "ERROR_MASK", ERROR_MASK);
    cJSON_AddNumberToObject(retObj, "EVENT_MASK", EVENT_MASK);
    cJSON_AddNumberToObject(retObj, "IS_DEBUG_MODE", IS_DEBUG_MODE);
    
    
    cJSON_AddNumberToObject(retObj, "AO_DAC_SPS", AO_DAC_SPS);
    cJSON_AddNumberToObject(retObj, "AO1_working_status", AoDdsParam[0].bEnable);
    cJSON_AddNumberToObject(retObj, "AO2_working_status", AoDdsParam[1].bEnable);
    cJSON_AddNumberToObject(retObj, "AO3_working_status", AoDdsParam[2].bEnable);
    cJSON_AddNumberToObject(retObj, "AO4_working_status", AoDdsParam[3].bEnable);
    
    cJSON_AddNumberToObject(retObj, "WINTIMER_MAX_EVENT_NUMBER", MAX_EVENT_NUMBER);
    cJSON_AddNumberToObject(retObj, "TFCCallbackLookupTableLen", TFCCallbackLookupTableLen);
    
    
    cJSON_AddNumberToObject(retObj, "beep_after_ins", sys.beep_after_ins);
    cJSON_AddNumberToObject(retObj, "auto_reset_interval", sys.auto_reset_interval);
    cJSON_AddNumberToObject(retObj, "start_up_music", sys.start_up_music);
    cJSON_AddNumberToObject(retObj, "uart1_baud_rate_default", sys.uart_baud_rate_default[0]);
    cJSON_AddNumberToObject(retObj, "uart2_baud_rate_default", sys.uart_baud_rate_default[1]);
    cJSON_AddNumberToObject(retObj, "uart3_baud_rate_default", sys.uart_baud_rate_default[2]);
    cJSON_AddNumberToObject(retObj, "uart4_baud_rate_default", sys.uart_baud_rate_default[3]);
    cJSON_AddNumberToObject(retObj, "uart5_baud_rate_default", sys.uart_baud_rate_default[4]);
    cJSON_AddNumberToObject(retObj, "can1_baud_rate_default", sys.can_baud_rate_default[0]);
    cJSON_AddNumberToObject(retObj, "can2_baud_rate_default", sys.can_baud_rate_default[1]);
    cJSON_AddNumberToObject(retObj, "i2c_baud_rate_default", sys.i2c_baud_rate_default);
    cJSON_AddNumberToObject(retObj, "ins_exec_interval", sys.ins_exec_interval);
    
    float temp;
    if(GetDoADCVal(0,3,&temp)) cJSON_AddNumberToObject(retObj, "DO0-11_temperature", temp);
    if(GetDoADCVal(1,3,&temp)) cJSON_AddNumberToObject(retObj, "DO12-23_temperature", temp);
    if(GetDoADCVal(0,0,&temp)) cJSON_AddNumberToObject(retObj, "DO0-11_ref_voltage", temp);
    if(GetDoADCVal(1,0,&temp)) cJSON_AddNumberToObject(retObj, "DO12-23_ref_voltage", temp);
    if(GetDoADCVal(0,1,&temp)) cJSON_AddNumberToObject(retObj, "12V_supply_voltage", temp);
    if(GetDoADCVal(0,2,&temp)) cJSON_AddNumberToObject(retObj, "DO0-11_working_voltage", temp);
    if(GetDoADCVal(1,2,&temp)) cJSON_AddNumberToObject(retObj, "DO12-23_working_voltage", temp);
    
    int i;
    for(i=0;i<24;i++)
    {
        if(GetDoADCVal(i/12,i%12+0x10,&temp))
        {
            sprintf(buf,"DO%d_actual_voltage",i);
            cJSON_AddNumberToObject(retObj, buf, temp);
        }
    }
    
    cJSON_AddStringToObject(retObj, "tfc_pm", "Liu Wenhao");
    cJSON_AddStringToObject(retObj, "tfc_hardware", "Liu Wenhao");
    cJSON_AddStringToObject(retObj, "tfc_software", "Liu Wenhao");
    cJSON_AddStringToObject(retObj, "tfc_copyright", "Minieye 2020");
    cJSON_AddStringToObject(retObj, "tfc_designer_email", "vio@minieye.cc");
    cJSON_AddStringToObject(retObj, "tfc_designer_contact", "18627031415");
    
    return 1;
}


//TFCCallbackMacroRecord
int TFCCallbackMacroRecord(cJSON* root, cJSON* retObj)
{
    char* operation;
    static u32 sectorId = 1, is_repeat=1, repeat_interval=1000;
    //符合性检查
    if(cJSON_GetObjectItem(root,"operation")) 
    {
        operation = cJSON_GetObjectItem(root,"operation")->valuestring;
    }
    else
    {
        cJSON_AddStringToObject(retObj, "report", "operation object is either 'start' or 'stop'." );
        return 0;
    }
    if(strcmp(operation,"start") && strcmp(operation,"stop"))
    {
        cJSON_AddStringToObject(retObj, "report", "operation object is either 'start' or 'stop'." );
        return 0;
    }
    
    if(cJSON_GetObjectItem(root,"sector_id")) sectorId = cJSON_GetObjectItem(root,"sector_id")->valueint;
    if(sectorId>15)  sectorId = 15;
    if(sectorId<1)  sectorId = 1;
    
    if(cJSON_GetObjectItem(root,"is_repeat")) is_repeat = cJSON_GetObjectItem(root,"is_repeat")->valueint;
    if(is_repeat)  is_repeat = 1;
    
    if(cJSON_GetObjectItem(root,"repeat_interval")) repeat_interval = cJSON_GetObjectItem(root,"repeat_interval")->valueint;
    if(repeat_interval>86400000)  repeat_interval = 86400000;
    if(repeat_interval<10) repeat_interval = 10;
    
    //处理start
    if(strcmp(operation,"start")==0)
    {
        fsModeCfg.sector_id = sectorId;
        fsModeCfg.cmd_len = 0;
        fsModeCfg.is_repeat = is_repeat;
        fsModeCfg.repeat_interval = repeat_interval;
        fsModeCfg.bEnable = 0;
        fsModeCfg.bRecording = 1;
        fsModeCfg.cmdAddr = 0;
        fsModeCfg.randomCheck = 0;
        WriteFSModeConfig(sectorId, &fsModeCfg);
        sFLASH_EraseSector((u32)sectorId*0x1000+0x20000);
        gFSModeFlag = 1;
    }
    else//stop
    {
        if(!fsModeCfg.bRecording)
        {
            cJSON_AddStringToObject(retObj, "report", "Must start recording, then stop it." );
            return 0;
        }
        gFSModeFlag = 0;
        //fsModeCfg.sector_id = sector_id;
        fsModeCfg.cmd_len = fsModeCfg.cmdAddr;
        fsModeCfg.bEnable = 1;
        fsModeCfg.bRecording = 0;
        fsModeCfg.randomCheck = 0;
        WriteFSModeConfig(fsModeCfg.sector_id , &fsModeCfg);
        cJSON_AddNumberToObject(retObj, "cmd_len", fsModeCfg.cmd_len);
    }
    return 1;
}

