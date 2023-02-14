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

#define  PANEL                            1       /* callback function: PanelDashboardCallback */
#define  PANEL_TEXTMSG                    2       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  3       /* control type: textMsg, callback function: (none) */
#define  PANEL_STRING_4                   4       /* control type: string, callback function: (none) */
#define  PANEL_STRING_3                   5       /* control type: string, callback function: (none) */
#define  PANEL_STRING                     6       /* control type: string, callback function: (none) */
#define  PANEL_SPLITTER                   7       /* control type: splitter, callback function: (none) */
#define  PANEL_STRING_2                   8       /* control type: string, callback function: (none) */

#define  PANEL_2                          2       /* callback function: PanelPopupCallback */
#define  PANEL_2_TEXTMSG_12               2       /* control type: textMsg, callback function: (none) */
#define  PANEL_2_COMMANDBUTTON            3       /* control type: command, callback function: Panel61OKCallback */
#define  PANEL_2_STRING                   4       /* control type: string, callback function: (none) */

#define  PANEL_6                          3       /* callback function: PanelSplashCallback */
#define  PANEL_6_TEXTMSG_13               2       /* control type: textMsg, callback function: (none) */
#define  PANEL_6_TEXTMSG_12               3       /* control type: textMsg, callback function: (none) */
#define  PANEL_6_TEXTMSG_11               4       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int   Panel61OKCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int   PanelDashboardCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int   PanelPopupCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int   PanelSplashCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
