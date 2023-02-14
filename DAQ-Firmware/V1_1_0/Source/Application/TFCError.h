#ifndef _TFC_ERROR_H_
#define _TFC_ERROR_H_

#define ERROR_SHORT     0x00FFFFFF
#define ERROR_DAQ       0x01000000
#define ERROR_PWR       0xFC000000
#define ERROR_PWR_IN    0x04000000
#define ERROR_PWR_SELF  0x08000000
#define ERROR_PWR_OC0   0x10000000
#define ERROR_PWR_OC1   0x20000000
#define ERROR_PWR_SC0   0x40000000
#define ERROR_PWR_SC1   0x80000000

extern u32 ERROR_MASK;

void SetTFCError(u32 error);
void ClearTFCError(u32 error);
u32 GetTFCError();
void TFCReportError(int error);

int GetErrorStatus(char* errorStatus);

#endif