#ifndef _FS_MODE_H_
#define _FS_MODE_H_


typedef  struct
{
    u8 sector_id;
    u32 cmd_len;
    u8 is_repeat;
    u32 repeat_interval;
    u8 bEnable;
    u8 bRecording;
    u32 cmdAddr;
    u32 randomCheck;
    u32 updateCheck;
}FsModeConfigDef;

extern u8 gFSModeFlag;
extern  FsModeConfigDef fsModeCfg;

u8 ReadFSModeConfig(u8 sector_id, FsModeConfigDef* fsCfg);
u8 WriteFSModeConfig(u8 sector_id,FsModeConfigDef* fsCfg);
void StartFSMode();
void StopFSMode();
u8 IsFSMode();
u8 FSModeDetection();
u8 GetFSModeStatus(char* buf);





#endif