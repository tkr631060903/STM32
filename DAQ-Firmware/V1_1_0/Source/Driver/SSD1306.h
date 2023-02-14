
#ifndef __SSD1306__H__
#define __SSD1306__H__






void WriteData(u16 val);


inline u8 ReadData(void);
int Init_SSD1306(void);
void XY_Coordinate(u16 x,u16 y);
void Active_Window(u16 x0, u16 x1,u16 y0, u16 y1);
int LcdRefresh();



void LcdPrintDot(u16 x, u16 y, u16 color);
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color);
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color);
void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color);
void LcdClear(u16 color);
int LcdDirectFill();
int LcdDirectClear();
	
#endif