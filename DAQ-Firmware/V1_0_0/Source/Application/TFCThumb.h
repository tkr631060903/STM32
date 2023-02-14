#ifndef __TFC_THUMB_H_
#define __TFC_THUMB_H_

extern const  char SoftwareVersionInfo[];

int TFCCallbackBeep(cJSON* root, cJSON* retObj);
int TFCCallbackMusic(cJSON* root, cJSON* retObj);
int TFCCallbackSwVersion(cJSON* root, cJSON* retObj);
int TFCCallbackEepromWrite(cJSON* root, cJSON* retObj);
int TFCCallbackEepromRead(cJSON* root, cJSON* retObj);
int TFCCallbackSelfTest(cJSON* root, cJSON* retObj);
int TFCCallbackUartWrite(cJSON* root, cJSON* retObj);
int TFCCallbackUartRead(cJSON* root, cJSON* retObj);
int TFCCallbackCanWrite(cJSON* root, cJSON* retObj);
int TFCCallbackCanRead(cJSON* root, cJSON* retObj);
int TFCCallbackAiRead(cJSON* root, cJSON* retObj);
int TFCCallbackAoWrite(cJSON* root, cJSON* retObj);
int TFCCallbackDoWrite(cJSON* root, cJSON* retObj);
int TFCCallbackGetKey(cJSON* root, cJSON* retObj);

//for debug only
int TFCCallbackDoRegWrite(cJSON* root, cJSON* retObj);
int TFCCallbackDoRegRead(cJSON* root, cJSON* retObj);
int TFCCallbackSetEventMask(cJSON* root, cJSON* retObj);

int TFCCallbackTest(cJSON* root, cJSON* retObj);
int TFCCallbackGetError(cJSON* root, cJSON* retObj);
int TFCCallbackIOOperation(cJSON* root, cJSON* retObj);
int TFCCallbackCanBaud(cJSON* root, cJSON* retObj);
int TFCCallbackUartBaud(cJSON* root, cJSON* retObj);
int TFCCallbackAoChannelWrite(cJSON* root, cJSON* retObj);
int TFCCallbackTFCConfig(cJSON* root, cJSON* retObj);
int TFCCallbackCpuId(cJSON* root, cJSON* retObj);
int TFCCallbackCanFilter(cJSON* root, cJSON* retObj);
int TFCCallbackCpuUsage(cJSON* root, cJSON* retObj);
int TFCCallbackTfcInfo(cJSON* root, cJSON* retObj);
int TFCCallbackMacroRecord(cJSON* root, cJSON* retObj);

#endif