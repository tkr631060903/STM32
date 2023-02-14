
#ifndef __GUI_CONTENT_H__
#define __GUI_CONTENT_H__

#define CVICALLBACK   

#include "GuiMaker.h"


#define VAL_RED                         0x001F /* 16 standard colors */
#define VAL_GREEN                       0x07E0
#define VAL_BLUE                        0xF800
#define VAL_CYAN                        0xFFE0
#define VAL_MAGENTA                     0xF81F
#define VAL_YELLOW                      0xF800
#define VAL_DK_RED                      0x0010
#define VAL_DK_BLUE                     0x8000
#define VAL_DK_GREEN                    0x0200
#define VAL_DK_CYAN                     0x8400
#define VAL_DK_MAGENTA                  0x8010
#define VAL_DK_YELLOW                   0x0410
#define VAL_LT_GRAY                     0x8410
#define VAL_DK_GRAY                     0x4208
#define VAL_BLACK                       0x0000
#define VAL_WHITE                       0xFFFF

#define VAL_PANEL_GRAY                  VAL_LT_GRAY
#define VAL_GRAY                        VAL_DK_MAGENTA
#define VAL_OFFWHITE                    VAL_LT_GRAY
#define VAL_TRANSPARENT                 VAL_WHITE

#define TYPE_UNDEFINED       0
#define TYPE_PANEL           1
#define TYPE_COMMANDBUTTON   2
#define TYPE_PICTUREBUTTON   3
#define TYPE_STRING          4
#define TYPE_TEXTMSG         5
#define TYPE_PICTURE         6
#define TYPE_DECORATION      7
#define TYPE_VERTICAL_SPLITTER	8
#define TYPE_HORIZONTAL_SPLITTER 9


typedef int (*controlCallbackType)(int panel, int control, int event, void* callbackData,int eventData1, int eventData2);
typedef int (*panelCallbackType)(int panel, int event, void* callbackData,int eventData1, int eventData2);

typedef struct
{
    u16 ItemId;
    u16 ItemType;
    u16 PanelId;
    u16 ControlId;
    u16 X1;
    u16 Y1;
    u16 X2;
    u16 Y2;
    u8  Dimmed;
    u8  Visible;
    u16 TextColor;
    u16 BgColor;
    controlCallbackType Callback;
	char* ExtraData;
    char  Text[18];
}Gui_Control_Type_Def;

extern Gui_Control_Type_Def GuiCtrl[];


