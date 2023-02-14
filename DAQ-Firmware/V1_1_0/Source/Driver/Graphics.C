
#include "includes.h"




extern const struct __HZK_ZK sysHzzk[];
extern const unsigned char ASCII[][16];

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
                        }
                    }
                }
            }
            return;
        }
        ip++;
    }while(sysHzzk[ip].code[0]);
}

unsigned long __writeOneASCII(unsigned char *pucMsk,
                              unsigned short x0,
                              unsigned short y0,
                              unsigned short color,
                              u16 bcolor,    
                              u8 mode)//1 不透明 0透明
{
    unsigned long i,j;
    unsigned short x,y;
    unsigned char ucChar;
    
    y = y0;
    for(i=0; i<16; i++) 
    {
        ucChar = *pucMsk++;
        XY_Coordinate(x0, y);
        x = x0;
        for(j=0; j<8; j++) 
        {
            if((ucChar << j) & 0x80) 
            {
                x ++;
                WriteData(color);
            } 
            else 
            {
                x ++;
                if(mode)
                {
                    WriteData(bcolor);
                }
                else
                {
                    XY_Coordinate(x, y);
                }
            }
            
        }
        y++;
    }
    return (8); 
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
            if(mode)//非透明
                ;
            else//透明
                ;
            __writeOneASCII((u8 *)&ASCII[(str[i] - 0x20)][0], px, y, color,bcolor,mode);
            px += 8;
            i++;
        }
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
    Active_Window(x0,y0,x0+width-1,y0+height-1);
    XY_Coordinate(x0,y0);
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
    Active_Window(0,0,319,239);	//设定面板分辨率为800*480 pixel
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
    Active_Window(x0,y0,x0+width-1,y0+height-1);
    XY_Coordinate(x0,y0);
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
    Active_Window(0,0,799,479);	//设定面板分辨率为800*480 pixel
    return ;      
}

#else

void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{   
    if(x0+width>319) width = 319-x0+1;
    if(y0+height>239) height = 239-y0+1;
    Active_Window(x0,y0,x0+width-1,y0+height-1);
    
    s32 imgSize = width*height;
    int i = 0;
    
    do
    {
        sFLASH_Init();//SPI Flash
        sFLASH_StartReadSequence(imageAddr+i*2*65535-5*2);    
        
        sFLASH_Rx_Init();
        SPI1_DMA_Configuration((imgSize>65535)?(65535):(imgSize));
        SPI_I2S_ReceiveData(sFLASH_SPI);SPI_I2S_ReceiveData(sFLASH_SPI);
        StartSPIDMATransfer();
        while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));//DMA1_FLAG_TC2
        sFLASH_CS_HIGH();
        
        DMA_Cmd(DMA1_Channel2, DISABLE);
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE); 
        SPI_Cmd(sFLASH_SPI, DISABLE);
        
        i++;
        imgSize -= 65535;
    }while(imgSize > 0);
    
    SPI_Cmd(sFLASH_SPI, DISABLE);
    Active_Window(0,0,319,239);	
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

