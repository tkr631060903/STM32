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
#define  PANEL_STRING                     2       /* control type: string, callback function: (none) */
#define  PANEL_COMMANDBUTTON              3       /* control type: command, callback function: GenerateCFileCallback */
#define  PANEL_STRING_2                   4       /* control type: string, callback function: (none) */
#define  PANEL_TEXTMSG                    5       /* control type: textMsg, callback function: (none) */
#define  PANEL_COMMANDBUTTON_3            6       /* control type: command, callback function: (none) */
#define  PANEL_PICTUREBUTTON              7       /* control type: pictButton, callback function: (none) */
#define  PANEL_TEXTMSG_2                  8       /* control type: textMsg, callback function: (none) */
#define  PANEL_COMMANDBUTTON_4            9       /* control type: command, callback function: TuiFilePathClickCallback */
#define  PANEL_DECORATION                 10      /* control type: deco, callback function: (none) */
#define  PANEL_PICTURE                    11      /* control type: picture, callback function: (none) */
#define  PANEL_DECORATION_3               12      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK GenerateCFileCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TuiFilePathClickCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
