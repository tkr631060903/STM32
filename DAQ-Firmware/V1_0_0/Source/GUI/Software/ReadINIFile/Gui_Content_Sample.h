
#ifndef __GUI_CONTENT_H__
#define __GUI_CONTENT_H__



#include ""


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


#define PANEL           1
#define COMMANDBUTTON   2
#define PICTUREBUTTON   3
#define STRING          4
#define TEXTMSG         5
#define PICTURE         6
#define DECORATION      7


typedef int (*controlCallback)(int panel, int control, int event, void* callbackData,int eventData1, int eventData2);

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
    controlCallback Callback;
    u8  Text[22];
}Gui_Control_Type_Def;