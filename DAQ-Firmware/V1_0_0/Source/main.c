
#include "includes.h"



void FlashWriteTest()
{
    char buf[0x100];
    
    int i;
    /*for(i=0;i<0x100;i++)
    {
        buf[i]=0xff-i;
    }
    sFLASH_EraseSector(0x1000);
    sFLASH_WritePage((uint8_t*)buf,0x1000,0x100);
    
    for(i=0;i<0x100;i++)
    {
        buf[i]=0xff;
    }*/
    debug("0x0000:\r\n");
    sFLASH_ReadBuffer((uint8_t*)buf, 0x0,0x100);
    for(i=0;i<0x100;i++)
    {
        debug("%02x\t",(int)buf[i]);
        if(i%10==9)
            debug("\r\n");
    }
    debug("\r\n");
    debug("\r\n");
    
    debug("0x2000:\r\n");
    sFLASH_ReadBuffer((uint8_t*)buf, 0x2000,0x100);
    for(i=0;i<0x100;i++)
    {
        debug("%02x\t",(int)buf[i]);
        if(i%10==9)
            debug("\r\n");
    }
    debug("\r\n");
    debug("\r\n");

    /*
    char internalTestCode[0x100]="Internal Flash Test Passed!\r\n";
    if(InternalFlashOnePageWrite(internalTestCode,strlen(internalTestCode)+1))
    {
        if(InternalFlashOnePageRead(buf,strlen(internalTestCode)+1))
        {
            debug(buf);
        }
        else
        {
            return;
        }
    }
    else
        return;
    */
}

void MemoryInit()
{
    ClearUart1RxBuffer();
    ClearUart2RxBuffer();
    ClearUart3RxBuffer();
    ClearUart4RxBuffer();
    ClearUart5RxBuffer();
}
const char softResetText[]="{\"cmd\": \"reset\",\"status\":1,\"reset_type\":\"RCC_FLAG_SFTRST Reset\",\"time_stamp\": 1}\r\n";

void ResetTypeDetection()
{
    char* str=(char*)softResetText;
    debug("\r\n\r\n");
    if(RCC_GetFlagStatus(RCC_FLAG_SFTRST))
    {
        sys.resetStatus = RCC_FLAG_SFTRST;
        debug("====RCC_FLAG_SFTRST Reset=====\r\n");
        UART3_PrintStr(" ");
        UART3_PrintStr(str);
    }
    else if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST))
    {
        sys.resetStatus = RCC_FLAG_IWDGRST;
        debug("====RCC_FLAG_IWDGRST Reset=====\r\n");
        
    }
    else if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST))
    {
        sys.resetStatus = RCC_FLAG_WWDGRST;
        debug("====RCC_FLAG_WWDGRST Reset=====\r\n");
        
    }
    else if(RCC_GetFlagStatus(RCC_FLAG_PORRST))
    {
        sys.resetStatus = RCC_FLAG_PORRST;
        debug("====RCC_FLAG_PORRST Reset=====\r\n");
        
    }
    else if(RCC_GetFlagStatus(RCC_FLAG_LPWRRST))
    {
        sys.resetStatus = RCC_FLAG_LPWRRST;
        debug("====RCC_FLAG_LPWRRST Reset=====\r\n");
        
    }
    else if(RCC_GetFlagStatus(RCC_FLAG_PINRST))
    {
        sys.resetStatus = RCC_FLAG_PINRST;
        debug("====RCC_FLAG_PINRST Reset=====\r\n");
        
    }
    else
    {
        sys.resetStatus = 0;
        debug("====Unknown Reset=====\r\n");
    }
    return;
}

void SystemInitConfig()
{
    if(sys.resetStatus == RCC_FLAG_PORRST)
    {
        if(sys.start_up_music)
        {
            PlayMusicMultiThreadInit(sys.start_up_music,200);
            SetTimer(5,20,PlayMusicMultiThread);
        }
    }
    if(sys.auto_reset_interval>=15)
    {
        SetTimer(9,sys.auto_reset_interval*100,SystemSoftReset);
    }
    if(sys.debug_mode)
    {
        SetDebugMode(1);
    }
    else SetDebugMode(0);

    return ;
}

void FatalErrorReport()
{
    KillTimer(3);
    IS_DEBUG_MODE = 1;
    debug("System stop running because of fatal error...\r\n");
    while(1)
    {
        ModelBeep(1000,500);
        DelayMs(50);
    }
}


//u8 testStr[] = { 0x30,0x00 }; // 测试数据
/*******************************************************************************************
函数描述；主函数
*******************************************************************************************/
int main(void)
{
    char buf[18];
    
    SystemInit();//系统时钟等初始化
    MemoryInit();
    GPIOInit();
    BeepInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    if(!LoadDefaultSystemParam()) FatalErrorReport();
    
    Timer5Init();
    SetTimer(3,100,FatalErrorReport);//如果自检卡死则1秒后发出警报
    Timer2Init();
    Timer4Init();
    Timer3Init(20000);    
    
    if(!(sys.start_up_music &&sys.resetStatus == RCC_FLAG_PORRST)) Beep(1000,100);
    UART1_Init(sys.uart_baud_rate_default[0]);
    UART2_Init(sys.uart_baud_rate_default[1]);
    UART3_Init(sys.uart_baud_rate_default[2]);
    UART3_SendByte(' ');
    UART4_Init(sys.uart_baud_rate_default[3]);
    UART5_Init(sys.uart_baud_rate_default[4]);
    CAN1_Init(sys.can_baud_rate_default[0]);
    CAN2_Init(sys.can_baud_rate_default[1]);
    I2C1_Init(sys.i2c_baud_rate_default);
    AIInit();
    DoInit();
    if(!DoSelfTest()) 
    {
        if(!IS_DEBUG_MODE)
        {
            IS_DEBUG_MODE = 1;
            DoSelfTest();
        }
        FatalErrorReport();
    }
    AoInit();
    
    
    ResetTypeDetection();
    GetDeviceHardwareId(buf);
    debug("Hardware version: %s\r\n",buf);
    debug("Software version: %s\r\n",SoftwareVersionInfo);
    
    //sys.chipEncrypted = 0;
    sFLASH_Init();
    u32 id = sFLASH_ReadID();
    char uid[17];
    sFLASH_ReadUniqueID(uid);
    debug("External Flash ID:%08x-%s\r\n",id,uid);
    if(!SPIFlashVarify()) 
    {
        IS_DEBUG_MODE = 1;
        debug("Fatal error: Abuse useage of the license!\r\n");
        FatalErrorReport();
    }
    LoadDefaultUserParam();
    debug("This is the %d times TFC starts to work.\r\n",user.startUpCounter);
    
    KillTimer(3);//解除自检卡死发出警报的定时器
    FSModeDetection();
    
    SetPeriodicTask();
    SystemInitConfig();
    RunUserInterface();
}