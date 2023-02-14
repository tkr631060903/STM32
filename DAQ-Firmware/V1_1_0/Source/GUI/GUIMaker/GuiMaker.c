#include <cvirte.h>     
#include <userint.h>
#include "GuiMaker.h"

static int panel2;

int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  /* out of memory */
    if ((panel2 = LoadPanel (0, "GuiMaker.uir", PANEL_2)) < 0)
        return -1;
    DisplayPanel (panel2);
    RunUserInterface ();
    DiscardPanel (panel2);
    return 0;
}

int CVICALLBACK PanelPopupCallback (int panel, int event, void *callbackData,
                                    int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_GOT_FOCUS:

            break;
        case EVENT_LOST_FOCUS:

            break;
        case EVENT_CLOSE:

            break;
    }
    return 0;
}

int CVICALLBACK PanelSplashCallback (int panel, int event, void *callbackData,
                                     int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_GOT_FOCUS:

            break;
        case EVENT_LOST_FOCUS:

            break;
        case EVENT_CLOSE:

            break;
    }
    return 0;
}

int CVICALLBACK PanelDashboardCallback (int panel, int event, void *callbackData,
                                        int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_GOT_FOCUS:

            break;
        case EVENT_LOST_FOCUS:

            break;
        case EVENT_CLOSE:

            break;
    }
    return 0;
}

int CVICALLBACK Panel61OKCallback (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:

            break;
    }
    return 0;
}
