#ifndef _TFC_ERROR_H_
#define _TFC_ERROR_H_

#define ERROR_SHORT   0x00FFFFFF
#define ERROR_OT      0x01000000
#define ERROR_GENERAL 0x80000000

extern u32 ERROR_MASK;

void SetTFCError(u32 error);
void ClearTFCError(u32 error);
u32 GetTFCError();
void TFCReportError(int error);

int GetErrorStatus(char* errorStatus);

#endif