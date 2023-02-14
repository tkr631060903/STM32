#ifndef _PRINTER_H_
#define _PRINTER_H_

#define BOLD        0x0001
#define UNDERLINE   0x0004
#define INVERSE     0x0008
#define ROTATION    0x0010
#define BOTTOMUP    0x0020

int InitPrinter();
int PrintLine(const char *fmt,...);
int SetPrintStyle(u16 style);

int PrintQRLine(const char *fmt,...);

#endif