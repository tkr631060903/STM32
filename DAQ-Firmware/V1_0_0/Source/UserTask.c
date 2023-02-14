
#include "includes.h"
#include "math.h"

/*
新建TFC命令相应函数：
1. 新建一个int pTFCCallback(cJSON* root, cJSON* retObj) 格式的响应函数
2. 在TFCCallbackLookupTable中新添一行，thumbText取json字符串中cmd的值，tfcCallback写响应函数的函数名
3. pTFCCallback中从root获取发来的参数名，往retObj中添加对象来增加相应JSON字符串的相应值，pTFCCallback的返回值即status值
*/

const TFCCallbackLookupTable_Type TFCCallbackLookupTable[]={
  {0,TFCCallbackBeep,"beep"},
  {1,TFCCallbackMusic,"music"},
  {2,TFCCallbackSwVersion,"sw_version"},
  {3,TFCCallbackEepromWrite,"eeprom_write"},
  {4,TFCCallbackEepromRead,"eeprom_read"},
  {5,TFCCallbackSelfTest,"self_test"},
  {6,TFCCallbackUartWrite,"uart_write"},
  {7,TFCCallbackUartRead,"uart_read"},
  {8,TFCCallbackCanWrite,"can_write"},
  {9,TFCCallbackCanRead,"can_read"},
  {10,TFCCallbackAiRead,"ai_read"},
  {11,TFCCallbackAoWrite,"ao_write"},
  {12,TFCCallbackDoWrite,"do_write"},
  {13,TFCCallbackGetKey,"get_key"},
  {14,TFCCallbackDoRegWrite,"do_reg_write"},
  {15,TFCCallbackDoRegRead,"do_reg_read"},
  {16,TFCCallbackSetEventMask,"event_mask_write"},
  {17,TFCCallbackTest,"test"},
  {18,TFCCallbackGetError,"get_error"},
  {19,TFCCallbackIOOperation,"io_operation"},
  {20,TFCCallbackCanBaud,"can_baud"},
  {21,TFCCallbackUartBaud,"uart_baud"},
  {22,TFCCallbackAoChannelWrite,"ao_channel_write"},
  {23,TFCCallbackTFCConfig,"tfc_config"},
  {24,TFCCallbackCpuId,"cpu_id"},
  {25,TFCCallbackCanFilter,"can_filter"},
  {26,TFCCallbackCpuUsage,"cpu_usage"},
  {27,TFCCallbackTfcInfo,"tfc_info"},
  {28,TFCCallbackMacroRecord,"macro_record"},
};
const int TFCCallbackLookupTableLen = sizeof(TFCCallbackLookupTable)/(sizeof(TFCCallbackLookupTable[0]));

void ToggleLedFunction()
{
    u8 a, b;
    GetLedStatus(&a, &b);
    SetLedStatus(!a, !b);
}

void CheckHostCommStatus()
{
    int i;
    char buf[UART3_BUFFER_SIZE];
    char* retStr;
    cJSON *root = NULL, *cmdName=NULL, *retObj=NULL;
    char* msg_id;
    if(GetUart3RxStatus(buf) || GetFSModeStatus(buf))
    {
        debug(" ->");
        debug(buf);
        root = cJSON_Parse(buf);
        if(root)
        {
            if(sys.beep_after_ins)
                Beep(1000,50);
            cmdName = cJSON_GetObjectItem(root, "cmd");
            if(cmdName && cmdName->valuestring && strlen(cmdName->valuestring)!=0)
            {
                for(i=0;i<TFCCallbackLookupTableLen;i++)
                {
                    if(strcmp(cmdName->valuestring,TFCCallbackLookupTable[i].thumbText)==0)
                    {
                        break;
                    }
                }
                if(i<TFCCallbackLookupTableLen)
                {
                    retObj = cJSON_CreateObject();
                    cJSON_AddStringToObject(retObj, "cmd", TFCCallbackLookupTable[i].thumbText);
                    cJSON_AddNumberToObject(retObj, "status", (*(TFCCallbackLookupTable[i].tfcCallback))(root,retObj));
                    if(cJSON_GetObjectItem(root, "msg_id")) 
                    {
                        msg_id = cJSON_GetObjectItem(root, "msg_id")->valuestring;
                        if(msg_id!=NULL && strlen(msg_id)>0 && strlen(msg_id)<=10)
                        {
                            cJSON_AddStringToObject(retObj, "msg_id", cJSON_GetObjectItem(root, "msg_id")->valuestring);
                        }
                    }
                    cJSON_AddNumberToObject(retObj, "time_stamp",GetSystemTime());
                    retStr=cJSON_PrintUnformatted(retObj);
                    UART3_PrintStr(retStr);
                    UART3_PrintStr("\r\n");
                    cJSON_free(retStr);
                    if(retObj) cJSON_Delete(retObj);
                    
                    if(strcmp(cmdName->valuestring,"macro_record"))
                    {
                        if(gFSModeFlag && fsModeCfg.bRecording)//录制过程中时
                        {
                            if(fsModeCfg.cmdAddr+strlen(buf)>=4095) 
                            {
                                NVIC_SystemReset();
                                while(1);
                            }
                            sFLASH_WriteBuffer((uint8_t*)buf,fsModeCfg.cmdAddr+(u32)(fsModeCfg.sector_id)*0x1000+0x20000,strlen(buf));
                            fsModeCfg.cmdAddr+=strlen(buf);
                            if(!sys.beep_after_ins)
                            {
                                Beep(2000,50);
                            }
                        }
                    }
                }
            }
        }
        if(root) cJSON_Delete(root);
    }
    else
    {
        //do nothing
    }
    return;
}
void TFCEventCheck()
{
    if(GetTFCEvent()&EVENT_MASK)
    {
        TFCReportEvent(GetTFCEvent());
    }
    if(GetTFCError()&ERROR_GENERAL)
    {
        u32 sf;
        if(ShortCircuitDetect(&sf))
        {
            if(sf& ERROR_SHORT)
            {
                SetTFCError (sf&ERROR_SHORT);
                TFCReportError(sf);
            }
        }
        ClearTFCError(ERROR_GENERAL);
    }
    else
    {
        /*u8 outBuf[2];
        outBuf[0]=0x03;
        ledDriverRegWrite(0,0x60,1,outBuf);
        ledDriverRegWrite(1,0x60,1,outBuf);*/
    }
}

void SetPeriodicTask()
{
    //SetTimer(1,100,ToggleLedFunction);
    if(sys.ins_exec_interval>=10 && sys.ins_exec_interval<=1000)
        SetTimer(1,sys.ins_exec_interval/10,CheckHostCommStatus);
    else
        SetTimer(1,5,CheckHostCommStatus);
    SetTimer(4,10,ToggleLedFunction);
    SetTimer(6,100,TFCEventCheck);
}


void OnTimer(unsigned short nIDEvent)
{
//  TODO: add extra Timer process here. 
//  e.g.  if(nIDEvent==1) 
//            Delay(23);
//        else if(nIDEvent==2)        
//            KillTimer(2);
//  End of TODO: add extra Timer process here. 
    return;
}