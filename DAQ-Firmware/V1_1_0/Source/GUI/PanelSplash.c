

#include "includes.h"

void SplashPanelTimeoutCallback()
{
    KillTimer(12);
    u16 panelHandle = LoadPanel(PANEL);
    DisplayPanel(panelHandle);
}

int   PanelSplashCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_IDLE:
        break;
    case EVENT_BEFORE_PAINT:
        break;
    case EVENT_AFTER_PAINT:
        LcdRefresh();
        SetTimer(12,100,SplashPanelTimeoutCallback);
        break;
    case EVENT_COMMIT:
        break;
    case EVENT_LEFT_CLICK:
        break;
    case EVENT_LEFT_CLICK_UP:
        break;
    case EVENT_LONG_PRESS:
        break;
    }
    return 1;
}
