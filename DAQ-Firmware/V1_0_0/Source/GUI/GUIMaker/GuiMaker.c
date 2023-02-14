#include <cvirte.h>		
#include <userint.h>
#include "GuiMaker.h"

static int panel2;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panel2 = LoadPanel (0, "GuiMaker.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panel2);
	RunUserInterface ();
	DiscardPanel (panel2);
	return 0;
}

