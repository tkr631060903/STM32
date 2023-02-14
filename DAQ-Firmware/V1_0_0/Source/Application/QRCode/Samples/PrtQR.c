#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include "PrtQR.h"
#include "qrencode.h"
#include "qrbits.h"

static int panelHandle;


int main (int argc, char *argv[])
{
	char x,y;
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "PrtQR.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);

	strcpy(strinbuf,"一二三四五六七八九十1234567890abcdefghijklmnopqrstuvwxyz");
	qrencode();
	CanvasClear (panelHandle,PANEL_CANVAS, VAL_ENTIRE_OBJECT);
	for (x = 0; x < WD; x+=1)
	{
		for (y = 0; y < WD; y++)
		{
			if ( QRBIT(x,y) )
				CanvasDrawRect (panelHandle,PANEL_CANVAS,MakeRect(10*(x),10*y,10,10), VAL_DRAW_FRAME_AND_INTERIOR);   
		}
	}

	RunUserInterface ();
	DiscardPanel (panelHandle);
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
