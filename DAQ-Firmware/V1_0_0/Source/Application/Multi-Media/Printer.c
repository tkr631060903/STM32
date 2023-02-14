#include "includes.h"

int InitPrinter()
{
    const char lopStr[]={0x1D, 0x72, 0x01};//lack of paper
    const char initStr[]={0x1B, 0x40, 0x1B, 0x21, 0x00};
    char recvStr[5];
    u32 curTime =0;
    
    
    
    SetUartSwitch(FUNC_PRINTER);
    SendData(PORT_COMM, (char*)lopStr, sizeof(lopStr));
    curTime = GetSystemTime();
    recvStr[1] =0;
    while(GetSystemTime()-curTime<20)
    {
        if(GetUart3RxStatus(recvStr))
            break;
    }
    if(recvStr[1]==0xAB)
    {
        if(recvStr[0]==0)
        {
            //²»È±Ö½£¬¼ÌÐø
        }
        else if(recvStr[0]==0x0c)
        {
            PEN_STATUS =0;
            DispErrorMsg(0);
            return 0;
        }
    }
    
    SendData(PORT_COMM, (char*)initStr, sizeof(initStr));
    return 1;
}

int PrintLine(const char *fmt,...)
{
    int ret = 0;
    va_list ap;
    char string[256];
    va_start(ap,fmt);
    ret = vsprintf(string,fmt,ap);
    
    const char start[]={0x1b,0x32,0x1c,0x26,0x1C, 0x21, 0x00};
    //const char start[]={0x1C, 0x2E, 0x00};
    const char end[]={0x0d, 0x0a};
    SendData(PORT_COMM, (char*)start, sizeof(start));
    SendData(PORT_COMM, string, strlen(string));
    SendData(PORT_COMM, (char*)end,sizeof(end));
    va_end(ap);
    return ret;
}

int PrintQRLine(const char *fmt,...)
{
    int ret = 0;
    va_list ap;
    char string[256];
    va_start(ap,fmt);
    ret = vsprintf(string,fmt,ap);
    
    //const char start[]={0x1C, 0x21, 0x00};
    const char start[]={0x1b,0x33,0x00,0x1C, 0x2E, 0x00};
    const char end[]={0x0d, 0x0a};
    SendData(PORT_COMM, (char*)start, sizeof(start));
    SendData(PORT_COMM, string, strlen(string));
    SendData(PORT_COMM, (char*)end,sizeof(end));
    va_end(ap);
    return ret;
}



int SetPrintStyle(u16 style)
{
    const char setBold[]={0x1B, 0x45, 0x01};
    const char setnBold[]={0x1B, 0x45, 0x00};
    const char setUnderline[]={0x1B, 0x2D, 0x01};
    const char setnUnderline[]={0x1B, 0x2D, 0x00};
    const char setInverse[]={0x1D, 0x42, 0x01};
    const char setnInverse[]={0x1D, 0x42, 0x00};
    const char setRotation[]={0x1B, 0x56, 0x01};
    const char setnRotation[]={0x1B, 0x56, 0x00};
    const char setBottomup[]={0x1B, 0x7B, 0x01};
    const char setnBottomup[]={0x1B, 0x7B, 0x00};
    if(style & BOLD)
        SendData(PORT_COMM, (char*)setBold,sizeof(setBold));
    else
        SendData(PORT_COMM, (char*)setnBold,sizeof(setnBold));
    if(style & UNDERLINE)
        SendData(PORT_COMM, (char*)setUnderline,sizeof(setUnderline));
    else
        SendData(PORT_COMM, (char*)setnUnderline,sizeof(setnUnderline));
    if(style & INVERSE)
        SendData(PORT_COMM, (char*)setInverse,sizeof(setInverse));
    else
        SendData(PORT_COMM, (char*)setnInverse,sizeof(setnInverse));
    if(style & ROTATION)
        SendData(PORT_COMM, (char*)setRotation,sizeof(setRotation));
    else
        SendData(PORT_COMM, (char*)setnRotation,sizeof(setnRotation));
    if(style & BOTTOMUP)
        SendData(PORT_COMM, (char*)setBottomup,sizeof(setBottomup));
    else
        SendData(PORT_COMM, (char*)setnBottomup,sizeof(setnBottomup));
    return 1;    
}