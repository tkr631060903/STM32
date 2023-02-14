#include <formatio.h>
#include "inifile.h"
#include <cvirte.h>
#include <userint.h>
#include "ReadINIFile.h"

static int panelHandle;

static IniText g_myInifile = 0;


//支持的版本号
#define SUPPORTED_VERSION 102

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ReadINIFile.uir", PANEL)) < 0)
		return -1;
	if(argc>1 && strlen(argv[1])!=0)
	{
		if(TuiFilePathClickCallback (0,0,EVENT_LEFT_CLICK_UP,argv[1],1234,0))
		{
			if(GenerateCFileCallback(0,0,EVENT_COMMIT,NULL,1234,0)>=0)
			{
				//成功
				//MessagePopup("提示",".tui文件已经成功转为C语言文件及头文件");
				return 0;
			}
		}
	}

	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);

	return 0;
}

int CVICALLBACK TuiFilePathClickCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	char tempFileName[MAX_PATHNAME_LEN];
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:


			if(eventData1 == 1234)
			{
				strcpy(tempFileName, (char*)callbackData);
			}
			else
			{
				if (FileSelectPopupEx ("", "*.tui", "*.tui","Specify tui file to open", VAL_LOAD_BUTTON,0, 0, tempFileName)!= VAL_EXISTING_FILE_SELECTED)
					return 0;
			}
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
			if (Ini_ReadFromFile (g_myInifile, tempFileName))
			{
				printf(tempFileName);
				MessagePopup("Inifile","Error reading Inifile");
				return 0;
			}

			int    sections = 0;

			if (g_myInifile)
			{
				sections = Ini_NumberOfSections (g_myInifile);
				if (sections>1)
				{
					int integerValue;
					if (Ini_GetInt (g_myInifile, "TUI File Header",
									"Version", &integerValue)
							> 0)
					{
						if(integerValue	==SUPPORTED_VERSION)
						{
							//
						}
						else
						{
							MessagePopup("提醒","文件版本不兼容");
							return 0;
						}
					}
					else
					{
						MessagePopup("提醒","文件损坏，无法找到版本信息");
						return 0;
					}
				}
				else
				{
					MessagePopup("提醒","文件不兼容");
					return 0;
				}
			}

			SetCtrlVal (panel, PANEL_STRING, tempFileName);
			break;
	}
	return 1;
}
int c2i(char ch)
{
	// 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2
	if(isdigit(ch))
		return ch - 48;

	// 如果是字母，但不是A~F,a~f则返回
	if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )
		return -1;

	// 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10
	// 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10
	if(isalpha(ch))
		return isupper(ch) ? ch - 55 : ch - 87;

	return -1;
}

/*
 * 功能：将十六进制字符串转换为整型(int)数值
 * */
int hex2dec(char *hex)
{
	int len;
	int num = 0;
	int temp;
	int bits;
	int i;

	// 此例中 hex = "1de" 长度为3, hex是main函数传递的
	len = strlen(hex);

	for (i=0, temp=0; i<len; i++, temp=0)
	{
		// 第一次：i=0, *(hex + i) = *(hex + 0) = '1', 即temp = 1
		// 第二次：i=1, *(hex + i) = *(hex + 1) = 'd', 即temp = 13
		// 第三次：i=2, *(hex + i) = *(hex + 2) = 'd', 即temp = 14
		temp = c2i( *(hex + i) );
		// 总共3位，一个16进制位用 4 bit保存
		// 第一次：'1'为最高位，所以temp左移 (len - i -1) * 4 = 2 * 4 = 8 位
		// 第二次：'d'为次高位，所以temp左移 (len - i -1) * 4 = 1 * 4 = 4 位
		// 第三次：'e'为最低位，所以temp左移 (len - i -1) * 4 = 0 * 4 = 0 位
		bits = (len - i - 1) * 4;
		temp = temp << bits;

		// 此处也可以用 num += temp;进行累加
		num = num | temp;
	}

	// 返回结果
	return num;
}
void ConvertColor(char* color)
{
	char buf[256];
	strcpy(buf,color);
	if(buf[0]=='0' && buf[1]=='X')
	{
		unsigned long bit32color ;
		bit32color =  hex2dec(buf+2);

		unsigned char blue, green, red;
		blue =  (unsigned char)(bit32color %256);
		green = (unsigned char)(bit32color/256%256);
		red = (unsigned char)((bit32color>>16)%256);

		blue >>= 3;
		green >>=2;
		red >>=3;

		unsigned short s16blue, s16red,s16green;
		s16blue = blue;
		s16red = red;
		s16green = green;

		s16blue <<= 11;
		s16green <<= 5;
		s16red = s16red;

		unsigned short fcolor = s16blue+ s16red+ s16green;
		sprintf(color,"0x%04X",fcolor);
	}
	else
	{
		//do nothing
	}
	return;

}

int CVICALLBACK GenerateCFileCallback (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			int    sections = 0;

			FILE* fp;

			fp= fopen("Gui_Content.h","w");
			if(fp== NULL)
			{
				MessagePopup("提醒","Gui_Content.h文件被占用");
				return -2;
			}
			else
			{
				//do nothing
			}


			if (g_myInifile)
			{
				sections = Ini_NumberOfSections (g_myInifile);
				if (sections>1)
				{
					//do nothing
				}
				else
				{
					MessagePopup("提醒","该界面没有什么元素");
					fclose(fp);
					return -3;
				}
			}
			else
			{
				MessagePopup("提醒","无法找到指定的tui文件");
				fclose(fp);
				return -3;
			}

			FILE* ff = fopen("Gui_Content_Sample.h" , "rb"); // 应该二进制读
			if(ff ==NULL)
			{
				MessagePopup("提醒","找不到Gui_Content_Sample.h文件或者文件被占用");
				fclose(fp);
				return -3;
			}
			fseek(ff , 0 , SEEK_END);
			int file_size = ftell(ff);

			fseek(ff , 0 , SEEK_SET);
			char* fileContent = (char*)malloc((file_size + 1) * sizeof(char));
			fread(fileContent , file_size , sizeof(char) , ff);
			fileContent[file_size ] = '\0'; // 应该封闭掉
			fclose(ff);
			/*
			int fileHandle = OpenFile ("Gui_Content_Sample.h",VAL_READ_ONLY,VAL_OPEN_AS_IS,VAL_ASCII);
			if(fileHandle ==-1)
			{
				MessagePopup("提醒","找不到Gui_Content_Sample.h文件或者文件被占用");
				fclose(fp);
				return -3;
			}

			int fileSize;
			GetFileSize ("Gui_Content_Sample.h",&fileSize);

			char* fileContent;
			fileContent= (char*)malloc(fileSize+1000);
			memset(fileContent,0,fileSize+1000);
			SetFilePtr(fileHandle, 0, 0);
			ReadFile(fileHandle,fileContent,fileSize+2);
			CloseFile (fileHandle);
			*/
			//开始写文件
			fprintf(fp,fileContent);
			free(fileContent);

			fprintf(fp,"\n\n#define Gui_Control_Length %d\n\n",sections-1);
			fprintf(fp,"\n\n\n#endif");
			fclose(fp);

			fp =fopen("Gui_Content.c","w");
			if(fp== NULL)
			{
				MessagePopup("提醒","Gui_Content.c文件被占用");
				return -2;
			}
			else
			{
				//do nothing
			}
			fprintf(fp,"#include \"includes.h\"\n\nGui_Control_Type_Def GuiCtrl[%d]={\n\n",sections);
			fprintf(fp,"{0,	TYPE_UNDEFINED,	NULL,	NULL,	0,	0,	0,	0,	0,	0,	0x0000,	0x0000,	NULL,	\"---\"},\n\n");

			char   *sectionName = NULL;
			int zindex[2][1000]= {0};
			//for(i=2;i<=sections;i++)
			int lastPanel=0;
			
			for(i=2; i<= sections; i++)
			{
				if(Ini_NthSectionName (g_myInifile, i, &sectionName)==0)
				{
					MessagePopup("","无法读取section 名字");
					continue;
				}
				zindex[0][i]=i; //ATTR_ZPLANE_POSITION
				int z = 0;
				Ini_GetInt (g_myInifile, sectionName,"ATTR_ZPLANE_POSITION", &z);
				zindex[1][i]=z;
				
				//判断是否是面板
				char   *sv;
				int retRes = 0;
				if ((retRes=Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_CTRL_STYLE", &sv))<= 0 || i==sections)  //panel
				{
					int rankLast = i-1;
					if(i==sections && retRes>0)
						rankLast = i;
					if(lastPanel!=0 && lastPanel+2<=rankLast)
					{
						int k,p,f,temp;
						for(k=lastPanel+1; k<rankLast; k++)
						{
							p=k;
							for(f=k+1; f<=rankLast; f++)
								if(zindex[1][p]<zindex[1][f])
									p=f;
							if(p!=k)
							{
								temp=zindex[0][k];
								zindex[0][k]=zindex[0][p];
								zindex[0][p]=temp;
								temp=zindex[1][k];
								zindex[1][k]=zindex[1][p];
								zindex[1][p]=temp;
							}
						}
					}
					lastPanel = i;
				}
				else//control
				{
					
				}
			}
			//按照zindex从大到小排序
			

			for(i=2; i<= sections; i++)
			{

				if(Ini_NthSectionName (g_myInifile, zindex[0][i], &sectionName)==0)
				{
					MessagePopup("","无法读取section 名字");
					continue;
				}

				unsigned short itemid;
				char itemtype[256]= {0};
				static char panelid[256]= {0};
				char controlid[512]= {0};
				unsigned short x1;
				unsigned short y1;
				unsigned short x2;
				unsigned short y2;
				char  dimmed;
				char  visible;
				char textcolor[256]= {0};
				char bgcolor[256]= {0};
				char callback[256]= {0};
				char extradata[256]= {0};
				char  text[22]= {0};

				itemid = (unsigned short)(i-1);
				int t[4];
				Ini_GetInt (g_myInifile, sectionName,"ATTR_LEFT", &t[0]);
				Ini_GetInt (g_myInifile, sectionName,"ATTR_TOP", &t[1]);
				x1= (unsigned short)t[0];
				y1=(unsigned short)t[1];
				Ini_GetInt (g_myInifile, sectionName,"ATTR_WIDTH", &t[2]);
				t[2]+=x1;
				x2=(unsigned short)t[2];
				Ini_GetInt (g_myInifile, sectionName,"ATTR_HEIGHT", &t[3]);
				t[3]+=y1;
				y2=(unsigned short)t[3];

				Ini_GetBoolean (g_myInifile, sectionName, "ATTR_DIMMED",&t[0]) ;
				dimmed= (char)t[0];
				Ini_GetBoolean (g_myInifile, sectionName, "ATTR_VISIBLE",&t[1]) ;
				visible = (char)t[1];

				char* cStr;
				Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_CALLBACK_NAME", &cStr);
				strcpy(callback,cStr);
				if(callback ==NULL || strlen(callback)==0)
					strcpy(callback , "NULL");
				strcpy(extradata,"0");



				char   *stringValue;
				if (Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_CTRL_STYLE", &stringValue)> 0)
				{
					char   *controlVal;
					Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_CONSTANT_NAME", &controlVal) ;
					sprintf(controlid,"%s_%s",panelid,controlVal);


					char* tmp;
					if(strcmp(stringValue,"CTRL_SQUARE_COMMAND_BUTTON_LS")==0)
					{
						strcpy(itemtype,"TYPE_COMMANDBUTTON");

						Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_COLOR", &tmp);
						strcpy(textcolor,tmp);
						ConvertColor(textcolor);
						Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_CMD_BUTTON_COLOR", &tmp);
						strcpy(bgcolor,tmp);
						ConvertColor(bgcolor);
						Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_TEXT", &tmp);
						strcpy(text,tmp);
					}
					else
						if(strcmp(stringValue,"CTRL_PICTURE_COMMAND_BUTTON_LS")==0)
						{
							strcpy(itemtype,"TYPE_PICTUREBUTTON");
							Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_COLOR", &tmp);
							strcpy(textcolor,tmp);
							ConvertColor(textcolor);
							Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_CMD_BUTTON_COLOR", &tmp);
							strcpy(bgcolor,tmp);
							ConvertColor(bgcolor);
							Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_TEXT", &tmp);
							strcpy(text,tmp);

							Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_TEXT", &tmp);
							strcpy(extradata,tmp);
						}
						else
							if(strcmp(stringValue,"CTRL_STRING_LS")==0)
							{
								strcpy(itemtype,"TYPE_STRING");
								Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TEXT_COLOR", &tmp);
								strcpy(textcolor,tmp);
								ConvertColor(textcolor);
								Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TEXT_BGCOLOR", &tmp);
								strcpy(bgcolor,tmp);
								ConvertColor(bgcolor);
								Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_DFLT_VALUE", &tmp);
								strcpy(text,tmp);

								//String 类型没有ATTR_HEIGHT
								Ini_GetInt (g_myInifile, sectionName,"ATTR_TEXT_POINT_SIZE", &t[3]);
								t[3]+=y1;
								t[3]+=10;
								y2=(unsigned short)t[3];
							}
							else
								if(strcmp(stringValue,"CTRL_TEXT_MSG")==0)
								{
									strcpy(itemtype,"TYPE_TEXTMSG");
									Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TEXT_COLOR", &tmp);
									strcpy(textcolor,tmp);
									ConvertColor(textcolor);
									Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TEXT_BGCOLOR", &tmp);
									strcpy(bgcolor,tmp);
									ConvertColor(bgcolor);
									Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_DFLT_VALUE", &tmp);
									strcpy(text,tmp);
								}
								else
									if(strcmp(stringValue,"CTRL_PICTURE_LS")==0)
									{
										strcpy(itemtype,"TYPE_PICTURE");
										Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_COLOR", &tmp);
										strcpy(textcolor,tmp);
										ConvertColor(textcolor);
										Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_PICT_BGCOLOR", &tmp);
										strcpy(bgcolor,tmp);
										ConvertColor(bgcolor);
										Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_TEXT", &tmp);
										strcpy(text,tmp);

										Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_LABEL_TEXT", &tmp);
										strcpy(extradata,tmp);
									}
									else
										if(strcmp(stringValue,"CTRL_RECESSED_BOX_LS")==0)
										{
											strcpy(itemtype, "TYPE_DECORATION");
											Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_FRAME_COLOR", &tmp);
											strcpy(textcolor,tmp);
											ConvertColor(textcolor);
											Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_FRAME_COLOR", &tmp);
											strcpy(bgcolor,tmp);
											ConvertColor(bgcolor);
											Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TOOLTIP_TEXT", &tmp);
											strcpy(text,tmp);
										}
										else
											if(strcmp(stringValue,"CTRL_VERTICAL_SPLITTER_LS")==0)  //竖线
											{
												strcpy(itemtype, "TYPE_VERTICAL_SPLITTER");
												Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_FRAME_COLOR", &tmp);
												strcpy(textcolor,tmp);
												ConvertColor(textcolor);
												Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_FRAME_COLOR", &tmp);
												strcpy(bgcolor,tmp);
												ConvertColor(bgcolor);
												Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TOOLTIP_TEXT", &tmp);
												strcpy(text,tmp);
												int thickTmp = 1;
												Ini_GetInt (g_myInifile, sectionName,"ATTR_FRAME_THICKNESS", &thickTmp);
												x2 = x1+ (unsigned short)thickTmp;
											}
											else
												if(strcmp(stringValue,"CTRL_HORIZONTAL_SPLITTER_LS")==0) //横线
												{
													strcpy(itemtype, "TYPE_HORIZONTAL_SPLITTER");
													Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_FRAME_COLOR", &tmp);
													strcpy(textcolor,tmp);
													ConvertColor(textcolor);
													Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_FRAME_COLOR", &tmp);
													strcpy(bgcolor,tmp);
													ConvertColor(bgcolor);
													Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TOOLTIP_TEXT", &tmp);
													strcpy(text,tmp);
													int thickTmp = 1;
													Ini_GetInt (g_myInifile, sectionName,"ATTR_FRAME_THICKNESS", &thickTmp);
													y2 = y1+ (unsigned short)thickTmp;
												}
												else
												{

												}
				}
				else
				{
					strcpy(itemtype,"TYPE_PANEL");

					char* _t_panelName;
					if(Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_CONSTANT_NAME", &_t_panelName)<=0)
					{
						MessagePopup("","遇到了无法支持的section");
						continue;
					}
					strcpy(panelid,_t_panelName);
					strcpy(controlid,"NULL");

					char* str;
					Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TITLE_COLOR", &str);
					strcpy(textcolor,str);
					ConvertColor(textcolor);

					Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_BACKCOLOR", &str);
					strcpy(bgcolor,str);
					ConvertColor(bgcolor);

					Ini_GetPointerToRawString (g_myInifile,sectionName, "ATTR_TITLE", &str);
					strcpy(text,str);

					x2 = x2 - x1;
					y2 = y2 - y1;

					x1 = 0;
					y1 = 0;
				}
				/*
				u16 itemid;
				u8 itemtype[256];
				static u8 panelid[256];
				u8 controlid[512];
				u16 x1;
				u16 y1;
				u16 x2;
				u16 y2;
				u8  dimmed;
				u8  visible;
				u8 textcolor[256];
				u8 bgcolor[256];
				u8 callback[256];
				u8  text[22];
				*/
				fprintf(fp,"{%d,	%s,	%s,	%s,	%d,	%d,	%d,	%d,	%d,	%d,	%s,	%s,	%s,	NULL,\"%s\"},\n\n",itemid, itemtype, panelid, controlid, x1,y1,x2,y2, dimmed, visible, textcolor, bgcolor, callback, text);
			}

			fprintf(fp,"};\n");
			fclose(fp);

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
