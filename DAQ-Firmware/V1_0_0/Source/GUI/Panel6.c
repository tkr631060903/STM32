

#include "includes.h"

/*
0   lack of paper
1   Touch screen error
2   Height Module Error
3   Weight module error
4   over temperature
5   calib failed
6   1...99 error
7   low battery
*/
int DispErrorMsg(u8 errCode)
{
    u16 panelHandle;
    
    panelHandle = LoadPanel(PANEL_5);
    SetCtrlVal(panelHandle,PANEL_5_PICTURE_2,5825154+2*367*39*(errCode%8));
    InstallPopup(panelHandle);
    while(InstallPopup(panelHandle)==0)
    {
        if(PEN_STATUS)
        {
            PEN_STATUS =0;
            RemovePopup(panelHandle);
            DelayMs(250);
            return 0;
        }
    }
    return 1;
}

int   Panel6CloseCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_5);
        RemovePopup(panelHandle);
        break;
    }
    return 1;
}