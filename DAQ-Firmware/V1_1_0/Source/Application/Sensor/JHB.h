
#ifndef _JHB_H_
#define _JHB_H_


extern u32 gJHBUpdateTime;



typedef struct
{
    u8 command;//命令
    u8 dataLength;//数据长度
    char data[10][10];//数据
}DownstreamData;

typedef struct
{
    u8 command;//命令
    u8 dataLength;//数据长度
    char data[10][10];//数据
    u8 status;//状态域
    u8 errorCode[10];//错误代码
    
}UpstreamData;

void JHBInit();

int SendDownstreamData(DownstreamData* dsd);
int FreeDownstreamData(DownstreamData* dsd);
int ParseUpstreamData(UpstreamData* dsd, char* str, int len);
int FreeUpstreamData(UpstreamData* dsd);

int GetFirstJHBData(DownstreamData* dsd, UpstreamData* usd);
void StartJHBWeightMeasure();
void EndJHBWeightMeasure();
int GetJHBWeightData();
int GetJHBParameterDataLoop();
int EnterJHBCalibFunction();

#endif