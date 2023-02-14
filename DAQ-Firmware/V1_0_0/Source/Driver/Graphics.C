

#include "includes.h"


#define LCDBUFSIZE	800*480

extern void DelayMs(u32 nMs);
extern const struct __ASC_ZK sysEnzk[];
extern const struct __HZK_ZK sysHzzk[];
extern u8 TOUCHSTS;
u16 fontbuf[256];

/**************************************************************************************
�������ܣ�����
���룺    u16  x,y    	        �������
	  	  u16  color		��ɫ
�����	  ��


**************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
	XY_Coordinate(x,y);
	WriteCommand(0x02);//MRWC
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
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x,x+width-1,y,y); // ����ˮƽ����ʼ��
	WriteCommand(0x90);//д0x90�Ĵ���
	WriteData(0x80);   //��0x90�Ĵ���д����
	RA8875_WAITSTATUS();
}
/**************************************************************************************
�������ܣ�������������ſ�
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
�������ܣ�  ����ֱֱ��
����	��  u16  x    	    ���ĺ�����
	  		u16  y  	    ����������
	  		u16  height		ֱ�߸߶�
	  		u16  color		��ɫ
���	��	��
**************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
	Text_color(color); //�趨��ɫ
	Geometric_Coordinate(x,x,y,y+height-1); // ������ʼ������
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
�������ܣ���һ��б��
���룺    u16  x1   ���ĺ�����
	  u16  y1  	    ����������
	  u16  x2		�����ĺ�����
	  u16  y2		������������
	  u16  color		��ɫ
�����	  ��
**************************************************************************************/
void LcdPrintBIAS(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x1,x2,y1,y2); // ������ʼ������
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
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
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x1,x2,y1,y2); // �趨��������
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
�������ܣ����һ����������
���룺    ���ε����ϽǺ����½����꣬��Ŀ����ɫ
�����	  ��
**************************************************************************************/
/*void Lcd_cursorFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)//LcdFillRec
{
	RA8875_WAITSTATUS();
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x1,x2,y1,y2-1); // �趨��������
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}*/
/**************************************************************************************
�������ܣ��������LCD��Ļ
���룺    Ŀ����ɫ
�����	  ��
**************************************************************************************/
void LcdClear(u16 color)
{
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(0,799,0,479); // �趨��������
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}

void LcdClearRect(u16 color,u16 x1,u16 y1,u16 x2,u16 y2)
{
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x1,x2,y1,y2); // �趨��������
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
�������ܣ���LCD�ϴ�ӡһ������
���룺    code       �ַ�����
          x,y	     Ŀ����ʾλ������
	  	  color	     ������ɫ
	  	  bcolor     ������ɫ
            mode 0 ͸��   1 ��͸��
�����	  ��
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
��������: 	����Ļ��ʾһ���ַ���
��    ��: 	char* str       �ַ���ָ��
                u16 x,y		Ŀ����ʾλ������
		u16 color	������ɫ
		u16 bcolor	������ɫ
		u8  mode    ģʽ��0-͸��1-��͸��
��    ��: 	��
**************************************************************************************/
void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor,u8 mode)
{
    u8 i;
    u16 px;
    
    i = 0;
    px = x;
    while(str[i])
    {
        if(str[i] > 0x7F)//�Ǻ���
        {
            LcdPrintHz((str[i]<<8)|str[i+1],px,y,color,bcolor,mode);
            i += 2;
            px += 16;
        }
        else//�Ǻ���
        {
            if(mode)
                Font_with_BackgroundColor();
            else
                Font_with_BackgroundTransparency();
            //Text_Background_Color(bcolor);
            Text_Foreground_Color(color);
            Font_Coordinate(px,y);//����
            Text_Mode();
            WriteCommand(0x02);
            WriteData(str[i]);
            i++;
            px += 8;
            Graphic_Mode();//�л���ͼ��ģʽ
        }
        RA8875_WAITSTATUS();
    }
}

/**************************************************************************************
��������: ����Ļ����ʽ��ʾһ���ַ���
��    ��: 

��    ��: ��
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
��������: ����Ļ��ʾһ 16bit bmpͼ��
��    ��: u16* image    	ͼ�����������ַ
          u16 x,y			���Ͻ�����
	  	  u16 width,height	ͼ�εĿ��Ⱥ͸߶�
��    ��: ��
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
        if(sys.language == 0)//����
        {
            imageAddr += 6498976;
        }
        else//Ӣ��
        {
            imageAddr = imageAddr;
        }
    }
    else//ϵͳ����
    {
        //do nothing
    }
    //s32 imgSize = width*height;
    //1...1. �趨�������ڷ�Χ (REG[30h] ~REG[37h])���ڴ�д��λ�� (REG[46h] ~REG[49h])
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    //2...2. �趨Serial Flash/ROM ��̬ (REG[05h])
    Serial_ROM_select1();
    Serial_ROM_Address_set_24u8();
    Select_Serial_Waveform_mode0();
    SERIAL_ROM_Read_Cycle_4bus();
    SERIAL_ROM_DMA_mode();
    SERIAL_ROM_Signal_mode();
    //3....3. �趨 �ڴ�ֱ�Ӵ�ȡ������Դ��ʼλ�� (REG[B0h] ~REG[B2h])
    DMA_Start_address_setting(imageAddr+2*800*y0+2*x0);
    //4....4. �趨 �ڴ�ֱ�Ӵ�ȡ������� (REG[B4h] ��REG[B5h])
    //     5. �趨 �ڴ�ֱ�Ӵ�ȡ����߶� (REG[B6h] �� REG[B7h])
    //     6. �趨�ڴ�ֱ�Ӵ�ȡ��ԴͼƬ���� (REG[B8h] �� REG[B9h])
    DMA_block_mode_size_setting(width, height, 800);
    //7....7. �����ڴ�ֱ�Ӵ�ȡΪ�������ģʽ (REG[BFh] bit 1)
    DMA_Block_mode();
    //8...8. �����ڴ�ֱ�Ӵ�ȡ��ʼѶ���Ҽ���ڴ�ֱ�Ӵ�ȡæµѶ�� (REG[BFh] bit 0)
    DMA_Start_enable();
    Chk_DMA_Busy();
    Active_Window(0,799,0,479);	//�趨���ֱ���Ϊ800*480 pixel
    return ;      
}
void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{   
    if(imageAddr<13000000)
    {
        if(sys.language == 0)//����
        {
            imageAddr += 6498976;
        }
        else//Ӣ��
        {
            imageAddr = imageAddr;
        }
    }
    else//ϵͳ����
    {
        //do nothing
    }
    //s32 imgSize = width*height;
    //1...1. �趨�������ڷ�Χ (REG[30h] ~REG[37h])���ڴ�д��λ�� (REG[46h] ~REG[49h])
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    //2...2. �趨Serial Flash/ROM ��̬ (REG[05h])
    Serial_ROM_select1();
    Serial_ROM_Address_set_24u8();
    Select_Serial_Waveform_mode0();
    SERIAL_ROM_Read_Cycle_4bus();
    SERIAL_ROM_DMA_mode();
    SERIAL_ROM_Signal_mode();
    //3....3. �趨 �ڴ�ֱ�Ӵ�ȡ������Դ��ʼλ�� (REG[B0h] ~REG[B2h])
    DMA_Start_address_setting(imageAddr);
    //4....4. �趨 �ڴ�ֱ�Ӵ�ȡ������� (REG[B4h] ��REG[B5h])
    //     5. �趨 �ڴ�ֱ�Ӵ�ȡ����߶� (REG[B6h] �� REG[B7h])
    //     6. �趨�ڴ�ֱ�Ӵ�ȡ��ԴͼƬ���� (REG[B8h] �� REG[B9h])
    DMA_block_mode_size_setting(width, height, width);
    //7....7. �����ڴ�ֱ�Ӵ�ȡΪ�������ģʽ (REG[BFh] bit 1)
    DMA_Block_mode();
    //8...8. �����ڴ�ֱ�Ӵ�ȡ��ʼѶ���Ҽ���ڴ�ֱ�Ӵ�ȡæµѶ�� (REG[BFh] bit 0)
    DMA_Start_enable();
    Chk_DMA_Busy();
    Active_Window(0,799,0,479);	//�趨���ֱ���Ϊ800*480 pixel
    return ;      
}
/*
void DispImage(unsigned short x0, unsigned short y0,unsigned long imageAddr,unsigned short width,unsigned short height)
{   
    s32 imgSize = width*height;
    //int i = 0;
    //1....1. �趨�������ڷ�Χ (REG[30h] ~REG[37h])���ڴ�д��λ�� (REG[46h] ~REG[49h])
    Active_Window(x0,x0+width-1,y0,y0+height-1);
    XY_Coordinate(x0,y0);
    WriteCommand(0x02);
    //2....2. �趨 Serial Flash/ROM ��̬ (REG[05h])
    Serial_ROM_select1();
    Serial_ROM_Address_set_24u8();
    Select_Serial_Waveform_mode0();
    SERIAL_ROM_Read_Cycle_4bus();
    SERIAL_ROM_DMA_mode();
    SERIAL_ROM_Signal_mode();
    //3.....3. �趨�ڴ�ֱ�Ӵ�ȡ������Դ��ʼλ�� (REG[B0h] ~REG[B2h])
    DMA_Start_address_setting(imageAddr);
    //4....4. �趨�ڴ�ֱ�Ӵ�ȡ���ݰ�����Ŀ (REG[B4h], REG[B6h] ��REG[B8h])
    DMA_Continuous_mode_size_setting(imgSize*2);
    //5....5. �����ڴ�ֱ�Ӵ�ȡ��ʼѶ�źͼ���ڴ�ֱ�Ӵ�ȡæµѶ�� (REG[BFh] bit 0)
    DMA_Start_enable();
    //5....check busy
    Chk_DMA_Busy();
    Active_Window(0,799,0,479);	//�趨���ֱ���Ϊ800*480 pixel
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
        if(sys.language == 0)//����
        {
            imageAddr += 6498976;
        }
        else//Ӣ��
        {
            imageAddr = imageAddr;
        }
    }
    else//ϵͳ����
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
        
        DMA_Cmd(DMA1_Channel2, DISABLE);// ��DMAͨ�� /
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);      // Enable SPI1 /    
        SPI_Cmd(sFLASH_SPI, DISABLE);
        
        i++;
        imgSize -= 65535;
    }while(imgSize > 0);
    
    SPI_Cmd(sFLASH_SPI, DISABLE);
    Active_Window(0,799,0,479);	//�趨���ֱ���Ϊ800*480 pixel
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
//DMA��ʾ�̶���ɫ�ģ�֧��16λ 2015-10-29 21:05
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