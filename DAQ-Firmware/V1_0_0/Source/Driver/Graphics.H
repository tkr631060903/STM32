
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//-------------------------------------------------------------------------------------
#include "stm32f10x.h"

extern void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2);
extern void LcdPrintDot(u16 x, u16 y, u16 color);
extern void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color);
extern void LcdPrintVert(u16 x, u16 y, u16 height, u16 color);
extern void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
extern void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color);
extern void LcdClear(u16 color);
extern void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor,u8 mode);
extern void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor,u8 mode);
extern void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,u8 mode, char *fmt,...);
extern void LcdPrint16bitBmp(u16* image,u16 x,u16 y,u16 widht,u16 height);
extern void LcdPrint8bitBmp(const u8* image,u16 x,u16 y,u16 widht,u16 height);
//extern void LcdPrintVertZuHe(u16 x, u16 y, u16 height, u16 y1);
extern void LcdPrintHorzZuHe(u16 x, u16 y, u16 width, u16 color);
void LcdPrintBIAS(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void Lcd_cursorFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LcdClearRect(u16 color,u16 x1,u16 y1,u16 x2,u16 y2);

void DispImage(unsigned short x0, //屏幕坐标
    unsigned short y0,            //屏幕坐标
    unsigned long imageAddr,      //EEROM地址
    unsigned short width,         //图像宽度
    unsigned short height);       //图像高度
#if DUAL_FLASH_MODE
void DispBgImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height);
#endif
void DispAlphaImage(unsigned short x0,  //屏幕坐标
    unsigned short y0,                  //屏幕坐标
    unsigned long imageAddr,            //EEROM地址
    unsigned long pointNum);            //有效点数


#endif
