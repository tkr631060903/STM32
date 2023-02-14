

#include "includes.h"


#define LCDBUFSIZE	800*480

extern void DelayMs(u32 nMs);
extern const struct __ASC_ZK sysEnzk[];
extern const struct __HZK_ZK sysHzzk[];
extern u8 TOUCHSTS;
u16 fontbuf[256];

/**************************************************************************************
函数功能：画点
输入：    u16  x,y    	        点的坐标
	  	  u16  color		颜色
输出：	  无


**************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
	XY_Coordinate(x,y);
	WriteCommand(0x02);//MRWC
	WriteData(color);
}

/**************************************************************************************
函数功能：	画水平直线
输入	：  WORD  x    		起点的横坐标
	  		WORD  y  	    起点的纵坐标
	  		WORD  width		直线长度
	  		WORD  color		颜色
输出	：	无
**************************************************************************************/
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x,x+width-1,y,y); // 设置水平线起始点
	WriteCommand(0x90);//写0x90寄存器
	WriteData(0x80);   //向0x90寄存器写数据
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：画喷油器零件号框
**************************************************************************************/
void LcdPrintHorzZuHe(u16 x, u16 y, u16 width, u16 color)
{
	int i;
	for(i =y;i<=475;i+=42)
	{
		LcdPrintHorz(x, i, width, color);
		LcdPrintHorz(x, i+1, width, color);
	}
}
/**************************************************************************************
函数功能：  画垂直直线
输入	：  u16  x    	    起点的横坐标
	  		u16  y  	    起点的纵坐标
	  		u16  height		直线高度
	  		u16  color		颜色
输出	：	无
**************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
	Text_color(color); //设定颜色
	Geometric_Coordinate(x,x,y,y+height-1); // 设置起始点坐标
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：画一条斜线
输入：    u16  x1   起点的横坐标
	  u16  y1  	    起点的纵坐标
	  u16  x2		结束的横坐标
	  u16  y2		结束的纵坐标
	  u16  color		颜色
输出：	  无
**************************************************************************************/
void LcdPrintBIAS(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x1,x2,y1,y2); // 设置起始点坐标
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：画矩形
输入：    矩形的左上角和右下角坐标，和目标颜色
		  x1,y1  左上角坐标
		  x2,y2  右上角坐标
		  fill	 0  画矩形框
		  		 1  矩形填充
输出：	  无
**************************************************************************************/
void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x1,x2,y1,y2); // 设定矩形坐标
	WriteCommand(0x90);
	if (fill)
	{
		WriteData(0xB0);
	}
	else
	{
		WriteData(0x90);
	}
	RA8875_WAITSTATUS();
}

/**************************************************************************************
函数功能：填充一个矩形区域
输入：    矩形的左上角和右下角坐标，和目标颜色
输出：	  无
**************************************************************************************/
/*void Lcd_cursorFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)//LcdFillRec
{
	RA8875_WAITSTATUS();
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x1,x2,y1,y2-1); // 设定矩形坐标
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}*/
/**************************************************************************************
函数功能：清除整个LCD屏幕
输入：    目标颜色
输出：	  无
**************************************************************************************/
void LcdClear(u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(0,799,0,479); // 设定矩形坐标
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}

void LcdClearRect(u16 color,u16 x1,u16 y1,u16 x2,u16 y2)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x1,x2,y1,y2); // 设定矩形坐标
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：在LCD上打印一个汉字
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  	  color	     字体颜色
	  	  bcolor     背景颜色
            mode 0 透明   1 不透明
输出：	  无
**************************************************************************************/
void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor,u8 mode)
{
    u16 i,j,ip,fcode;
    
    ip = 0;
    do
    {
        if(   (sysHzzk[ip].code[0] == (code>>8) )   &&   (sysHzzk[ip].code[1] == (code&0xFF))  )
        {
            for(i=0;i<32;i=i+2)
            {
                fcode = sysHzzk[ip].HZCODE[i]<<8|sysHzzk[ip].HZCODE[i+1];
                XY_Coordinate(x,y+i/2);
                WriteCommand(0x02);
                for(j=0;j<16;j++)
                {
                    if(fcode&(0x8000>>j))
                    {
                        WriteData(color);
                    }
                    else
                    {
                        if(mode)
                        {
                            WriteData(bcolor);
                        }
                        else
                        {
                            XY_Coordinate(x+j+1,y+i/2);
                            WriteCommand(0x02);
                        }
                    }
                }
            }
            return;
        }
        ip++;
    }while(sysHzzk[ip].code[0]);
}

/**************************************************************************************
功能描述: 	在屏幕显示一个字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
		u8  mode    模式，0-透明1-非透明
输    出: 	无
**************************************************************************************/
void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor,u8 mode)
{
    u8 i;
    u16 px;
    
    i = 0;
    px = x;
    while(str[i])
    {
        if(str[i] > 0x7F)//是汉字
        {
            LcdPrintHz((str[i]<<8)|str[i+1],px,y,color,bcolor,mode);
            i += 2;
            px += 16;
        }
        else//非汉字
        {
            if(mode)
                Font_with_BackgroundColor();
            else
                Font_with_BackgroundTransparency();
            //Text_Background_Color(bcolor);
            Text_Foreground_Color(color);
            Font_Coordinate(px,y);//坐标
            Text_Mode();
            WriteCommand(0x02);
            WriteData(str[i]);
            i++;
            px += 8;
            Graphic_Mode();//切换回图形模式
        }
        RA8875_WAITSTATUS();
    }
}

/**************************************************************************************
功能描述: 在屏幕按格式显示一个字符串
输    入: 

输    出: 无
**************************************************************************************/
void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,u8 mode,char *fmt,...)
{
    va_list ap;
    char str[64];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    LcdPrintStr(str,x,y,color,bcolor,0);
    va_end(ap);
}

/**************************************************************************************
功能描述: 在屏幕显示一 16bit bmp图形
输    入: u16* image    	图形数据数组地址
          u16 x,y			左上角坐标
	  	  u16 width,height	图形的宽度和高度
输    出: 无
**************************************************************************************/
void LcdPrint16bitBmp(u16* image,u16 x,u16 y,u16 widht,u16 height)
{
	u16 w,h;
	
	for(h=0;h<height;h++)
	{
		XY_Coordinate(x,y+h);
		WriteCommand(0x02);		//MRWC  REG[02h] -- Memory Read/Write Command
		for(w = 0; w< widht; w++)
		{
			WriteData(*image++);
		}
	}
}

void LcdPrint8bitBmp(const u8* image,u16 x,u16 y,u16 widht,u16 height)
{
    //u8  color8;
    u16 w,h;
    //u16 r,g,b;
    
    for(h=0;h<height;h++)
    {
        XY_Coordinate(x,y+h);
        WriteCommand(0x02);		//MRWC  REG[02h] -- Memory Read/Write Command
        for(w = 0; w< widht; w++)
        {
            WriteData(*image++);
            
            //color8 = *image++;
            /*
            0123456700000000 
            67000 345000 01200
            0011 1000 1110 0011
            
            */
            //r = (color8 & 0xE0)<<13; 
            //g = (color8 & 0x1C)<<6;
            //b = (color8 & 0x03)<<3;
            
            //r = ((color8 & 0xE0)<<13)|0x1800; 
            //g = ((color8 & 0x1C)<<6) |0xE0;
            //b = ((color8 & 0x03)<<3) |0x07;
            
            //r = (color8 & 0x03)<<14;
            //g = (color8 & 0x1C)<<6;
            //b = (color8 & 0xE0)>>11; 
            
            //WriteData(r|g|b|0x38E3);
        }
    }
}


#if DUAL_FLASH_MODE
void DispBgImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{   
    if(imageAddr<13000000)
    {
        if(sys.language == 0)//中文
        {
            imageAddr += 6498976;
        }
        else//英文
        {
            imageAddr = imageAddr;
        }
    }
    else//系统符号
    {
        //do nothing
    }
    //s32 imgSize = width*height;
    //1...1. 设定工作窗口范围 (REG[30h] ~REG[37h])和内存写入位置 (REG[46h] ~REG[49h])
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    //2...2. 设定Serial Flash/ROM 组态 (REG[05h])
    Serial_ROM_select1();
    Serial_ROM_Address_set_24u8();
    Select_Serial_Waveform_mode0();
    SERIAL_ROM_Read_Cycle_4bus();
    SERIAL_ROM_DMA_mode();
    SERIAL_ROM_Signal_mode();
    //3....3. 设定 内存直接存取数据来源起始位置 (REG[B0h] ~REG[B2h])
    DMA_Start_address_setting(imageAddr+2*800*y0+2*x0);
    //4....4. 设定 内存直接存取区块宽度 (REG[B4h] 和REG[B5h])
    //     5. 设定 内存直接存取区块高度 (REG[B6h] 和 REG[B7h])
    //     6. 设定内存直接存取来源图片宽度 (REG[B8h] 和 REG[B9h])
    DMA_block_mode_size_setting(width, height, 800);
    //7....7. 开启内存直接存取为区块搬移模式 (REG[BFh] bit 1)
    DMA_Block_mode();
    //8...8. 开启内存直接存取起始讯号且检查内存直接存取忙碌讯号 (REG[BFh] bit 0)
    DMA_Start_enable();
    Chk_DMA_Busy();
    Active_Window(0,799,0,479);	//设定面板分辨率为800*480 pixel
    return ;      
}
void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{   
    if(imageAddr<13000000)
    {
        if(sys.language == 0)//中文
        {
            imageAddr += 6498976;
        }
        else//英文
        {
            imageAddr = imageAddr;
        }
    }
    else//系统符号
    {
        //do nothing
    }
    //s32 imgSize = width*height;
    //1...1. 设定工作窗口范围 (REG[30h] ~REG[37h])和内存写入位置 (REG[46h] ~REG[49h])
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    //2...2. 设定Serial Flash/ROM 组态 (REG[05h])
    Serial_ROM_select1();
    Serial_ROM_Address_set_24u8();
    Select_Serial_Waveform_mode0();
    SERIAL_ROM_Read_Cycle_4bus();
    SERIAL_ROM_DMA_mode();
    SERIAL_ROM_Signal_mode();
    //3....3. 设定 内存直接存取数据来源起始位置 (REG[B0h] ~REG[B2h])
    DMA_Start_address_setting(imageAddr);
    //4....4. 设定 内存直接存取区块宽度 (REG[B4h] 和REG[B5h])
    //     5. 设定 内存直接存取区块高度 (REG[B6h] 和 REG[B7h])
    //     6. 设定内存直接存取来源图片宽度 (REG[B8h] 和 REG[B9h])
    DMA_block_mode_size_setting(width, height, width);
    //7....7. 开启内存直接存取为区块搬移模式 (REG[BFh] bit 1)
    DMA_Block_mode();
    //8...8. 开启内存直接存取起始讯号且检查内存直接存取忙碌讯号 (REG[BFh] bit 0)
    DMA_Start_enable();
    Chk_DMA_Busy();
    Active_Window(0,799,0,479);	//设定面板分辨率为800*480 pixel
    return ;      
}
/*
void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{   
    s32 imgSize = width*height;
    //int i = 0;
    //1....1. 设定工作窗口范围 (REG[30h] ~REG[37h])和内存写入位置 (REG[46h] ~REG[49h])
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    //2....2. 设定 Serial Flash/ROM 组态 (REG[05h])
    Serial_ROM_select1();
    Serial_ROM_Address_set_24u8();
    Select_Serial_Waveform_mode0();
    SERIAL_ROM_Read_Cycle_4bus();
    SERIAL_ROM_DMA_mode();
    SERIAL_ROM_Signal_mode();
    //3.....3. 设定内存直接存取数据来源起始位置 (REG[B0h] ~REG[B2h])
    DMA_Start_address_setting(imageAddr);
    //4....4. 设定内存直接存取数据搬移数目 (REG[B4h], REG[B6h] 和REG[B8h])
    DMA_Continuous_mode_size_setting(imgSize*2);
    //5....5. 开启内存直接存取起始讯号和检查内存直接存取忙碌讯号 (REG[BFh] bit 0)
    DMA_Start_enable();
    //5....check busy
    Chk_DMA_Busy();
    Active_Window(0,799,0,479);	//设定面板分辨率为800*480 pixel
    return ;
}
*/
#else

void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{   
    //GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)(1));
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    
    s32 imgSize = width*height;
    int i = 0;
    if(imageAddr<13000000)
    {
        if(sys.language == 0)//中文
        {
            imageAddr += 6498976;
        }
        else//英文
        {
            imageAddr = imageAddr;
        }
    }
    else//系统符号
    {
        //do nothing
    }
    do
    {
        sFLASH_Init();//SPI Flash
        sFLASH_StartReadSequence(imageAddr+i*2*65535-5*2);    
        
        //sFLASH_RecvByte();sFLASH_RecvByte();///////////
        sFLASH_Rx_Init();
        SPI1_DMA_Configuration((imgSize>65535)?(65535):(imgSize));
        SPI_I2S_ReceiveData(sFLASH_SPI);SPI_I2S_ReceiveData(sFLASH_SPI);
        StartSPIDMATransfer();
        while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));//DMA1_FLAG_TC2
        sFLASH_CS_HIGH();
        
        DMA_Cmd(DMA1_Channel2, DISABLE);// 打开DMA通道 /
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);      // Enable SPI1 /    
        SPI_Cmd(sFLASH_SPI, DISABLE);
        
        i++;
        imgSize -= 65535;
    }while(imgSize > 0);
    
    SPI_Cmd(sFLASH_SPI, DISABLE);
    Active_Window(0,799,0,479);	//设定面板分辨率为800*480 pixel
    //GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)(0));
    return ;      
}
#endif
void DispAlphaImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned long pointNum)
{
    int i;
    unsigned short x,y,color;
    
    sFLASH_Init();//SPI Flash
    sFLASH_StartReadSequence(imageAddr);    
    
    for(i=0;i<pointNum;i++)
    {
        x=(unsigned short)sFLASH_ReadByte();
        y=(unsigned short)sFLASH_ReadByte();
        color = ((unsigned short)sFLASH_ReadByte()<<8) |  sFLASH_ReadByte();
        LcdPrintDot(x0+x,y0+y,color);
    }
    sFLASH_CS_HIGH();
}


/*
void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{    
    unsigned short img; 
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    
    sFLASH_Init();//SPI Flash
    sFLASH_StartReadSequence(imageAddr);    
    sFLASH_Rx_Init();
    

    SPI1_DMA_Configuration(65535);
    StartSPIDMATransfer();
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2))//DMA1_FLAG_TC2
    {
        u16 cnt = DMA_GetCurrDataCounter(DMA1_Channel2);
        debug("%d\r\n",cnt);
    }
    
    
    SPI1_DMA_Configuration(65535);
    StartSPIDMATransfer();
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2))//DMA1_FLAG_TC2
    {
        u16 cnt = DMA_GetCurrDataCounter(DMA1_Channel2);
        debug("%d\r\n",cnt);
    }
    SPI1_DMA_Configuration(65535);
    StartSPIDMATransfer();
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2))//DMA1_FLAG_TC2
    {
        u16 cnt = DMA_GetCurrDataCounter(DMA1_Channel2);
        debug("%d\r\n",cnt);
    }
    SPI1_DMA_Configuration(65535);
    StartSPIDMATransfer();
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2))//DMA1_FLAG_TC2
    {
        u16 cnt = DMA_GetCurrDataCounter(DMA1_Channel2);
        debug("%d\r\n",cnt);
    }
    
    sFLASH_CS_HIGH();
    //SPI_Cmd(sFLASH_SPI, DISABLE);
    
   
    return ;      
}
*/


/*
//DMA显示固定颜色的，支持16位 2015-10-29 21:05
void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{
    unsigned short i=0,j=0;
    
    unsigned short img;    

    sFLASH_Init();//SPI Flash
    u8* pt =(u8*)(&img);
    sFLASH_StartReadSequence(imageAddr);
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    
    sFLASH_Rx_Init();
    
    SPI1_DMA_Configuration(width*height*2);
    StartSPIDMATransfer();

    
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));//DMA1_FLAG_TC2
    sFLASH_CS_HIGH();
    
   
    return ;      
}*/