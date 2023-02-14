#include "inifile.h"
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include "PictureTool.h"

#define ROM_SIZE 1048576*16		//16M flash

static int panelHandle;

int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  /* out of memory */
    if ((panelHandle = LoadPanel (0, "PictureTool.uir", PANEL)) < 0)
        return -1;

	IniText g_myInifile = 0;
	if (g_myInifile)
	{
		Ini_Dispose (g_myInifile);
		g_myInifile = 0;
	}
	if (!(g_myInifile = Ini_New (0)))
	{
		MessagePopup("Inifile","Error allocating memory for Inifile");
		return 0;
	}
	if (Ini_ReadFromFile (g_myInifile, "PictureTool.ini"))
	{
		MessagePopup("Inifile","Error reading Inifile");
		return 0;
	}
	int integerValue;
	if (Ini_GetInt (g_myInifile, "Config",
					"Addr", &integerValue)
			> 0)
	{
		SetCtrlVal(panelHandle,PANEL_NUMERIC,integerValue);
	}
	if (Ini_GetInt (g_myInifile, "Config",
					"MaxAddr", &integerValue)
			> 0)
	{
		SetCtrlVal(panelHandle,PANEL_NUMERIC_2,integerValue);
	}
    DisplayPanel (panelHandle);
    RunUserInterface ();
    DiscardPanel (panelHandle);
    return 0;
}

int CVICALLBACK SaveCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char curFileName[512];
    unsigned long addr;
    int bitmapId;
    int bytesPerRow,pixelDepth,width,height;
    unsigned char   *bits, *mask;
    int             colorTableIndex, color, index, *colorTable;
    int i,j;
    FILE* fp;
    unsigned char* romFile;
    unsigned short tmpPixel16;
    //unsigned long tmpPixel32;
    unsigned long tmpAddr;
    char ss[1111];
    unsigned int bPicMode;
	unsigned int bLanguage;
    unsigned char tmpCode[4];
    unsigned int lineCount;
    switch (event)
    {
        case EVENT_COMMIT:
            GetCtrlVal(panelHandle,PANEL_STRING,curFileName);
            if(strlen(curFileName)==0)
            {
                MessagePopup("错误","怎么着也要先填点东西再来点我吧！"); 
                return 0;
            }
            GetCtrlVal(panelHandle,PANEL_NUMERIC,&addr);
			GetCtrlVal(panelHandle,PANEL_RADIOBUTTON_3,&bLanguage);//1.中文
			if(bLanguage && addr <13000000)
			{
				addr += 6498976;
			}
            if(addr<0x100 || addr>=ROM_SIZE)
            {
                MessagePopup("错误","此区域留作备用或无法使用，存入失败 ！\n请使用不小于256的地址。");
                return 0;
            }
			
			
			
            GetBitmapFromFile(curFileName,&bitmapId); 
            
            AllocBitmapData (bitmapId, &colorTable, &bits, &mask);
            GetBitmapData (bitmapId, &bytesPerRow, &pixelDepth, &width, &height, colorTable, bits, mask);
            if(pixelDepth<=8 || (pixelDepth!=24 && pixelDepth!=32))
            {
                MessagePopup("错误","暂无处理此类图的功能，存入失败");
                return 0;   
            }
            
            romFile=(unsigned char*)malloc(ROM_SIZE*1.1);
            
            fp=fopen("rom.bin","rb");
            if(fp==NULL)
            {
                MessagePopup("错误","rom文件无法打开，存入失败");
                return 0;   
            }
            fread(romFile,1,ROM_SIZE,fp);
            fclose(fp);
			
			/*
			const char initPassword[24]={0x88,0x26,0x28,0x30,0xab,0x0b,0x0d,0x2f,0xca,0x1e,0x9d,0x01,0x5d,0x35,0x6d,0xac,0xca,0xbb,0x42,0x9c,0x99,0x42,0xab,0x88};
			for(i=0;i<24;i++)
	        {
	            romFile[0x10080+i]=initPassword[i];
	        }
            */
			
            GetCtrlVal(panelHandle,PANEL_RADIOBUTTON_2,&bPicMode);
            
            if(bPicMode)   //白色略过
            {
                tmpAddr=addr;
                for(i=0;i<height;i++)
                {
                    for(j=0;j<width;j++)
                    {
                        if(pixelDepth==24)
                        {
                            if(bits[(i*width+j)*pixelDepth/8]==0xFF &&bits[(i*width+j)*pixelDepth/8+1]==0xFF && bits[(i*width+j)*pixelDepth/8+2]==0xFF )
                                continue;
                            tmpCode[0]=(j&0xFF);
                            tmpCode[1]=(i&0xFF);
                            tmpPixel16=((bits[(i*width+j)*pixelDepth/8]&0xF8)<<8) | ((bits[(i*width+j)*pixelDepth/8+1]&0xFC)<<3) | ((bits[(i*width+j)*pixelDepth/8+2]&0xF8)>>3);
                            tmpCode[2]=(tmpPixel16>>8);
                            tmpCode[3]=(tmpPixel16&0xFF);
                        }
                        else if(pixelDepth==32)
                        {
                            if(bits[(i*width+j)*pixelDepth/8]==0xFF &&bits[(i*width+j)*pixelDepth/8+1]==0xFF && bits[(i*width+j)*pixelDepth/8+2]==0xFF )
                                continue;
                            tmpCode[0]=(j&0xFF);
                            tmpCode[1]=(i&0xFF);
                            tmpPixel16=((bits[(i*width+j)*pixelDepth/8]&0xF8)<<8) | ((bits[(i*width+j)*pixelDepth/8+1]&0xFC)<<3) | ((bits[(i*width+j)*pixelDepth/8+2]&0xF8)>>3);
                            tmpCode[2]=(tmpPixel16>>8);
                            tmpCode[3]=(tmpPixel16&0xFF);
                        }
                        else
                        {
                            MessagePopup("错误","发生了一个无药可救的错误，请退出本程序");
                            QuitUserInterface(0);
                        }
                        romFile[tmpAddr]=tmpCode[0];
                        romFile[tmpAddr+1]=tmpCode[1];
                        romFile[tmpAddr+2]=tmpCode[2];
                        romFile[tmpAddr+3]=tmpCode[3];
                        tmpAddr+=4;
                    }
                }
            }
            else           //逐点放入
            {
                for(i=0;i<height;i++)
                {
                    for(j=0;j<width;j++)
                    {
                        if(pixelDepth==24)
                        {
                            tmpAddr=(i*width+j)*pixelDepth/8;
                            tmpPixel16=((bits[tmpAddr]&0xF8)<<8) | ((bits[tmpAddr+1]&0xFC)<<3) | ((bits[tmpAddr+2]&0xF8)>>3);
                        }
                        else if(pixelDepth==32)
                        {
                            tmpAddr=(i*width+j)*pixelDepth/8;
                            //tmpPixel16=((bits[tmpAddr]&0xF8)>>3) | ((bits[tmpAddr+1]&0xFC)<<3) | ((bits[tmpAddr+2]&0xF8)<<8);
							tmpPixel16=((bits[tmpAddr]&0xF8)<<8) | ((bits[tmpAddr+1]&0xFC)<<3) | ((bits[tmpAddr+2]&0xF8)>>3);  
                        }
                        else
                        {
                            MessagePopup("错误","发生了一个无药可救的错误，请退出本程序");
                            QuitUserInterface(0);
                        }
                        tmpAddr=addr+(i*width+j)*2;
                        //romFile[tmpAddr]=(tmpPixel16>>8);
						//大端小端切换
						romFile[tmpAddr]=(unsigned char)(*((unsigned char*)(&tmpPixel16)+1));
						romFile[tmpAddr+1]=(unsigned char)(*((unsigned char*)(&tmpPixel16)+0));       
                        //romFile[tmpAddr+1]=(tmpPixel16&0x00FF);
                    }
                }
            }
            
            
            fp=fopen("rom.bin","wb");
            if(fp==NULL)
            {
                MessagePopup("错误","rom文件无法打开，存入失败");
                return 0;
            }
            fwrite(romFile,1,ROM_SIZE,fp);
            fclose(fp);
            
            free(romFile);
            DiscardBitmap (bitmapId);
            free (colorTable);
            free (bits);
            free (mask);
            if(bPicMode)
            {
                GetNumTextBoxLines(panelHandle,PANEL_TEXTBOX,&lineCount);
                if(lineCount<=4)
                {
                    sprintf(ss,"有效点：%d\n透明点：%d",(tmpAddr-addr)/4,width*height-(tmpAddr-addr)/4);
                    SetCtrlVal(panelHandle,PANEL_TEXTBOX,ss);   
                }
            }
            //SetCtrlAttribute(panelHandle,PANEL_COMMANDBUTTON,ATTR_DIMMED,1);
			GetCtrlVal(panelHandle,PANEL_RADIOBUTTON_3,&bLanguage);//1.中文
			if(bLanguage && addr <13000000)    
			{
				addr -= 6498976;
			}
            sprintf(ss,"图片已成功存入！下个可用地址为%d",bPicMode?(tmpAddr):(addr+width*height*2))  ;
            SetCtrlVal(panelHandle,PANEL_NUMERIC,bPicMode?(tmpAddr):(addr+width*height*2));
            MessagePopup("成功",ss);
			
			//存到INI文件中保存，下次自动加载。
			IniText g_myInifile = 0; 
			if (g_myInifile)
			{
				Ini_Dispose (g_myInifile);
				g_myInifile = 0;
			}
			if (!(g_myInifile = Ini_New (0)))
			{
				MessagePopup("Inifile","Error allocating memory for Inifile");
				return 0;
			}
			if(Ini_PutInt(g_myInifile, "Config","Addr",(bPicMode?(tmpAddr):(addr+width*height*2))))
			{
				MessagePopup("Inifile","Error adding data to Inifile");
				return 0;
			}
			int recordedMaxVal = 0;
			int nextPicAddr = 0;
			GetCtrlVal(panelHandle,PANEL_NUMERIC_2,&recordedMaxVal);
			nextPicAddr = (bPicMode?(tmpAddr):(addr+width*height*2));
			if(nextPicAddr > recordedMaxVal)
				recordedMaxVal = nextPicAddr;	
			
			SetCtrlVal(panelHandle,PANEL_NUMERIC_2,recordedMaxVal);
			if(Ini_PutInt(g_myInifile, "Config","MaxAddr",recordedMaxVal))
			{
				MessagePopup("Inifile","Error adding data to Inifile");
				return 0;
			}
			if(Ini_WriteToFile(g_myInifile, "PictureTool.ini"))
			{
				MessagePopup("Inifile","Error save file");
				return 0;
			}
			
            
            break;
    }
    return 0;
}

int CVICALLBACK FileSelectCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char curFileName[512];
    int i;
    int res;
    int bitmapId;
    int bytesPerRow,pixelDepth,width,height;
    char ss[1111];
    switch (event)
    {
        case EVENT_LEFT_CLICK_UP:
            res = FileSelectPopup("","*.tif;*.pcx;*.dib;*.rle;*.ico;*.jpg;*.png;*.wmf;*.emf;*.bmp",
                "Images(*.tif;*.pcx;*.dib;*.rle;*.ico;*.jpg;*.png;*.wmf;*.emf;*.bmp)",
                
                "请选择打开的文件...",VAL_LOAD_BUTTON ,0,0,1,1,curFileName);
            if(res <= VAL_NO_FILE_SELECTED) 
            {
                return 0;
            }
            GetBitmapFromFile(curFileName,&bitmapId);
            
            res = GetBitmapData (bitmapId, &bytesPerRow, &pixelDepth, &width, &height, NULL, NULL, NULL);
            if(width>800 || height>480)
            {
                MessagePopup("错误","图像超过规定的大小，请重新选择图像。如果图像未超，请将图片另存为bmp之后再试");
                return 0;
            }
            SetCtrlBitmap(panelHandle,PANEL_PICTURE,0,bitmapId); 
            sprintf(ss,"宽度：%d\n高度：%d\n位深度：%d\n",width,height,pixelDepth);
            ResetTextBox(panelHandle,PANEL_TEXTBOX,"");
            SetCtrlVal(panelHandle,PANEL_TEXTBOX,ss);
            
            DiscardBitmap(bitmapId);
            //SetCtrlAttribute(panelHandle,PANEL_COMMANDBUTTON,ATTR_DIMMED,0);
            SetCtrlVal(panelHandle,PANEL_STRING, curFileName);
            
            break;
    }
    return 0;
}

int CVICALLBACK PanelCallback (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_GOT_FOCUS:

            break;
        case EVENT_LOST_FOCUS:

            break;
        case EVENT_CLOSE:
            QuitUserInterface(0);
            break;
    }
    return 0;
}

int CVICALLBACK PictureMethodSelectCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            SetCtrlVal(panelHandle,PANEL_RADIOBUTTON,0);
            SetCtrlVal(panelHandle,PANEL_RADIOBUTTON_2,0);
            SetCtrlVal(panelHandle,control,1);
            break;
    }
    return 0;
}

int CVICALLBACK LanguageSelectEnglishCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal(panelHandle,PANEL_RADIOBUTTON_3,0);
            SetCtrlVal(panelHandle,PANEL_RADIOBUTTON_4,0);
            SetCtrlVal(panelHandle,PANEL_RADIOBUTTON_4,1);
			break;
	}
	return 0;
}

int CVICALLBACK LanguageSelectChineseCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal(panelHandle,PANEL_RADIOBUTTON_3,0);
            SetCtrlVal(panelHandle,PANEL_RADIOBUTTON_4,0);
			SetCtrlVal(panelHandle,PANEL_RADIOBUTTON_3,1);
			break;
	}
	return 0;
}
