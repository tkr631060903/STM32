/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: PanelCallback */
#define  PANEL_PICTURE                    2       /* control type: picture, callback function: (none) */
#define  PANEL_STRING                     3       /* control type: string, callback function: FileSelectCallback */
#define  PANEL_NUMERIC_2                  4       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC                    5       /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON              6       /* control type: command, callback function: SaveCallback */
#define  PANEL_TEXTBOX                    7       /* control type: textBox, callback function: (none) */
#define  PANEL_RADIOBUTTON_2              8       /* control type: radioButton, callback function: PictureMethodSelectCallback */
#define  PANEL_RADIOBUTTON_3              9       /* control type: radioButton, callback function: LanguageSelectChineseCallback */
#define  PANEL_RADIOBUTTON_4              10      /* control type: radioButton, callback function: LanguageSelectEnglishCallback */
#define  PANEL_TEXTMSG_3                  11      /* control type: textMsg, callback function: (none) */
#define  PANEL_RADIOBUTTON                12      /* control type: radioButton, callback function: PictureMethodSelectCallback */
#define  PANEL_TEXTMSG                    13      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  14      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK FileSelectCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LanguageSelectChineseCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LanguageSelectEnglishCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PictureMethodSelectCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
