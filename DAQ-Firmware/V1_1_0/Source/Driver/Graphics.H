
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//-------------------------------------------------------------------------------------


void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor,u8 mode);
void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor,u8 mode);
void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,u8 mode, char *fmt,...);

void DispImage(unsigned short x0, //??Ļ????
    unsigned short y0,            //??Ļ????
    unsigned long imageAddr,      //EEROM??ַ
    unsigned short width,         //ͼ??????
    unsigned short height);       //ͼ???߶?
#if DUAL_FLASH_MODE
void DispBgImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height);
#endif
void DispAlphaImage(unsigned short x0,  //??Ļ????
    unsigned short y0,                  //??Ļ????
    unsigned long imageAddr,            //EEROM??ַ
    unsigned long pointNum);            //??Ч????


#endif
