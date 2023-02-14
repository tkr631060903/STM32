
#include "includes.h"

static u16 gAWX0=0, gAWY0=0, gAWX1=0, gAWY1=0;
static u16 gCursorX=0, gCursorY=0;
static u8 gRamCache[1024];
//static u8 DisplayCache[1024];

//color ��ɫΪ 0x0000����ӦֵΪ1��������ʾ��
void WriteData(u16 val)
{
    u16 cacheIndex = ((gCursorY&0x38)<<4)+(gCursorX&0x7F);
    if(cacheIndex>1023) cacheIndex = 1023;
    if(val) 
        gRamCache[cacheIndex] |= (1<<(gCursorY&0x07));
    else 
        gRamCache[cacheIndex] &= (~(1<<(gCursorY&0x07)));
    gCursorX ++;
    
    if(gCursorX>gAWX1)
    {
        gCursorX = gAWX0;
        gCursorY++;
    }
    if(gCursorY>gAWY1)
    {
        gCursorY = gAWY0;
    }
}



int Init_SSD1306(void)
{
    //const u8 cmdList[]={0xAE,0x00,0x10,0x40,0xB0,0x81,0xFF,0xA1,0xA6,0xA8,0x3F,0xC8,0xD3,0x00,0xD5,0x80,0xD8,0x05,0xD9,0xF1,0xDA,0x12,0xDB,0x30,0x8D,0x14,0xAF};
    const u8 cmdList[]={0xAE,0x00,0x10,0x40,0x81,0xFF,0xA1,0xC8,0xA6,0xA8,0x3F,0xD3,0x00,0xD5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,0x40,0x20,0x02,0x8D,0x14,0xA4,0xA6,0xAF};
    int i =0; 
    int retVal = 1;
    
    
    for(i=0;i<sizeof(cmdList);i++)
    {
        retVal *= I2C1_ByteWrite(0x78,cmdList[i],0x00);
    }
    
    return retVal;
}


void XY_Coordinate(u16 x,u16 y)
{
    if(x>127) x=127;
    if(y>63) y=63;
    gCursorX = x;
    gCursorY = y;
    return;
}

void Active_Window(u16 x0, u16 y0, u16 x1, u16 y1)
{
    if(x0>127) x0=127;
    if(x1>127) x1=127;
    if(y0>63) y0=63;
    if(y1>63) y1=63;
    gAWX0=x0;
    gAWY0=y0;
    gAWX1=x1;
    gAWY1=y1;
    XY_Coordinate(x0,y0);
}


/**************************************************************************************
�������ܣ�����
���룺    u16  x,y    	        �������
u16  color		��ɫ
�����	  ��


**************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
    XY_Coordinate(x,y);
    WriteData(color);
}



/**************************************************************************************
�������ܣ�	��ˮƽֱ��
����	��  WORD  x    		���ĺ�����
WORD  y  	    ����������
WORD  width		ֱ�߳���
WORD  color		��ɫ
���	��	��
**************************************************************************************/
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color)
{
    int i;
    Active_Window(x,y,x+width-1,y);
    for(i=0;i<width;i++)
        WriteData(color);
}

/**************************************************************************************
�������ܣ�  ����ֱֱ��
����	��  u16  x    	    ���ĺ�����
u16  y  	    ����������
u16  height		ֱ�߸߶�
u16  color		��ɫ
���	��	��
**************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
    int i;
    Active_Window(x,y,x,y+height-1);
    for(i=0;i<height;i++)
        WriteData(color);
}

/**************************************************************************************
�������ܣ�������
���룺    ���ε����ϽǺ����½����꣬��Ŀ����ɫ
x1,y1  ���Ͻ�����
x2,y2  ���Ͻ�����
fill	 0  �����ο�
1  �������
�����	  ��
**************************************************************************************/
void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color)
{
    Active_Window(x1,y1,x2,y2);
    if(fill)
    {
        int blockSize = (x2-x1+1)*(y2-y1+1);
        int i;
        for(i=0;i<blockSize;i++)
        {
            WriteData(color);
        }
    }
    else
    {
        LcdPrintHorz(x1,y1,x2-x1+1,color);
        LcdPrintHorz(x1,y2,x2-x1+1,color);
        LcdPrintVert(x1,y1,y2-y1+1,color);
        LcdPrintVert(x2,y1,y2-y1+1,color);
    }
    
}

/**************************************************************************************
�������ܣ��������LCD��Ļ
���룺    Ŀ����ɫ
�����	  ��
**************************************************************************************/
void LcdClear(u16 color)
{
    int i;
    const int blockSize = 128*64;

    Active_Window(0,0,127,63);    
    for(i=0;i<blockSize;i++)
    {
        WriteData(color);
    }
    
}

int LcdDirectClear()
{
    unsigned char m,n;
	for(m=0;m<8;m++)
	{
        if(!I2C1_ByteWrite(0x78,0xb0+m,0x00))
            return 0;
        I2C1_ByteWrite(0x78,0x00,0x00);
        I2C1_ByteWrite(0x78,0x10,0x00);
		for(n=0;n<128;n++)
        {
            I2C1_ByteWrite(0x78,0x00,0x40);
        }
	}
    return 1;
}

int LcdDirectFill()
{
    unsigned char m,n;
	for(m=0;m<8;m++)
	{
        if(!I2C1_ByteWrite(0x78,0xb0+m,0x00))
            return 0;
        I2C1_ByteWrite(0x78,0x00,0x00);
        I2C1_ByteWrite(0x78,0x10,0x00);
		for(n=0;n<128;n++)
        {
            I2C1_ByteWrite(0x78,0xFF,0x40);
        }
	}
    return 1;
}


//���Դ��е�����ˢ�µ�LCD��
int LcdRefresh()
{
    unsigned char m;
	for(m=0;m<8;m++)
	{
        if(!I2C1_ByteWrite(0x78,0xb0+m,0x00))
            return 0;
        I2C1_ByteWrite(0x78,0x00,0x00);
        I2C1_ByteWrite(0x78,0x10,0x00);
        I2C1_BufferWrite(0x78,&gRamCache[(m<<7)],0x40,128);
        //for(n=0;n<128;n++)
        //    I2C1_ByteWrite(0x78,gRamCache[(m<<7)+n],0x40);
	}
    return 1;
}
