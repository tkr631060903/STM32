

#include "includes.h"

//Sector    0x00000-0x01000:        Encrypt       1s
//Sector    0x01000-0x02000:        RSVD          1s
//Sector    0x02000-0x0E000:        User info     12s
//Sector    0x10000-0x20000:        fsConfig[16]  16s
//Sector    0x20000-0x30000:        fsContent[16] 16s


/*
typedef struct
{
    u8 sector_id;
    u16 cmd_len;
    u8 is_repeat;
    u32 repeat_interval;
    u8 bEnable;
    u8 bRecording;
    u32 cmdAddr;
    u32 randomCheck;
    u32 updateCheck;
}FsModeConfigDef;
*/



u8 gFSModeFlag = 0;
 FsModeConfigDef  fsModeCfg;
char sectorBuffer[0x1000];

u8 ReadFSModeConfig(u8 sectorId, FsModeConfigDef* fsCfg)
{
    if(sectorId>15) sectorId = 15;
    sFLASH_ReadBuffer((uint8_t*)fsCfg,(u32)sectorId*0x1000+0x10000,sizeof(FsModeConfigDef));
    if(fsCfg->updateCheck==1235396309)
        return 1;
    else return 0;
}

u8 WriteFSModeConfig(u8 sectorId,FsModeConfigDef* fsCfg)
{
    if(sectorId>15) sectorId = 15;
    sFLASH_EraseSector((u32)sectorId*0x1000+0x10000);
    fsCfg->updateCheck=1235396309;
    sFLASH_WriteBuffer((uint8_t*)fsCfg,(u32)sectorId*0x1000+0x10000,sizeof(FsModeConfigDef));
    return 1;
}

void StartFSMode()
{
    //debug("StartFSMode();\r\n");
    KillTimer(10);
    gFSModeFlag = 1;
    sFLASH_ReadBuffer((uint8_t*)sectorBuffer,(u32)(fsModeCfg.sector_id)*0x1000+0x20000,0x1000);
    fsModeCfg.cmdAddr = fsModeCfg.cmd_len-1;
    return;
}

void StopFSMode()
{
    gFSModeFlag= 0;
}

u8 IsFSMode()
{
    return gFSModeFlag;
}

u8 FSModeDetection()
{
    int i;
    u8 keyStatus = ((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8))<<0)+((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9))<<1)+((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10))<<2)+((!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11))<<3);
    if(keyStatus==0) return 0;
    if(ReadFSModeConfig(keyStatus,&fsModeCfg))
    {
        if(fsModeCfg.bEnable && fsModeCfg.cmd_len && fsModeCfg.sector_id<=15 && fsModeCfg.cmd_len<4096 && fsModeCfg.bRecording==0)
        {
            for(i=0;i<keyStatus;i++)
            {
                ModelBeep(2000,70);
                DelayMs(70);
            }
            //PlayMusicMultiThreadInit(4,200);
            //SetTimer(5,200/10,PlayMusicMultiThread);
            StartFSMode();
            return 1;
        }
    }
    return 0;
}
u8 GetFSModeStatus(char* buf)
{
    if(gFSModeFlag==0) return 0;
    if(!(fsModeCfg.bEnable && fsModeCfg.cmd_len && fsModeCfg.sector_id<=15 &&fsModeCfg.sector_id>=1 && fsModeCfg.cmd_len<4096 && fsModeCfg.bRecording ==0)) return 0;
    int i = 0;
    int charReceived = 1;
    int startIndex = 0;
    int retVal = 0;
    if(sectorBuffer[fsModeCfg.cmdAddr]==0x0a)
    {
        for(i=fsModeCfg.cmdAddr-1;i>0;i--)
        {
            if(sectorBuffer[i]==0x0d || sectorBuffer[i]==0x0a || sectorBuffer[i]==0x00)
                break;
        }
        if(i)
        {
            charReceived = fsModeCfg.cmdAddr - i;
            startIndex = i+1;
        }
        else
        {
            charReceived = fsModeCfg.cmdAddr - i+1;
            startIndex = i;
        }
        for(i=0;i<charReceived;i++)
        {
            buf[i] = sectorBuffer[startIndex+i];
        }
        retVal = charReceived;
        buf[charReceived] = 0;
        
        sectorBuffer[fsModeCfg.cmdAddr]=0x0d;
    }
    if(fsModeCfg.cmdAddr<charReceived)
        fsModeCfg.cmdAddr = 0;
    else
        fsModeCfg.cmdAddr = fsModeCfg.cmdAddr - charReceived;
    if(fsModeCfg.cmdAddr==0)
    {
        //debug("StopFSMode();\r\n");
        StopFSMode();
        if(fsModeCfg.is_repeat)
            SetTimer(10,fsModeCfg.repeat_interval/10,StartFSMode);
    }
    return retVal;
}


   