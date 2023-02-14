
#ifndef _JHB_H_
#define _JHB_H_


extern u32 gJHBUpdateTime;



typedef struct
{
    u8 command;//����
    u8 dataLength;//���ݳ���
    char data[10][10];//����
}DownstreamData;

typedef struct
{
    u8 command;//����
    u8 dataLength;//���ݳ���
    char data[10][10];//����
    u8 status;//״̬��
    u8 errorCode[10];//�������
    
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