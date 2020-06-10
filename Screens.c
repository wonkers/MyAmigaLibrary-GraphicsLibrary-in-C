#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <graphics/gfx.h>
#include <graphics/gfxmacros.h>
#include <graphics/gfxbase.h>

#include "Screens.h"

struct IntuitionBase *IntuitionBase;
struct Screen *IntScreen;
struct NewScreen NewScreen = 
{
	0,0,
	0, 0, 0,
	1, 0,
	0,
	SCREENQUIET,
	NULL,
	"",
	NULL, NULL
};

VOID CreateScreen(UWORD width, UWORD height, UBYTE depth)
{
	if((IntuitionBase = (struct IntuitionBase *)
		OpenLibrary("intuition.library", 0)) == NULL)
		{
			printf("Failed to Open Intuition Library\n");
			exit(0);
		}
	
	NewScreen.Width = width;
	NewScreen.Height = height;
	NewScreen.Depth = depth;
	
	
	IntScreen = (struct Screen *) OpenScreen(&NewScreen);
	if(IntScreen == 0)
	{
		printf("No Screen!\n");
		CleanExit();
	}
	
	OffGadget(IntScreen->FirstGadget->NextGadget, IntScreen, NULL);
	RemoveGadget(IntScreen, IntScreen->FirstGadget->NextGadget);
	OffGadget(IntScreen->FirstGadget, IntScreen, NULL);
	RemoveGadget(IntScreen, IntScreen->FirstGadget);
	
	
}

VOID CleanExit()
{
	ClearMemory();
	exit(0);
}
VOID ClearMemory()
{
	if(IntScreen != NULL)
		CloseScreen(IntScreen);
	
	/*close intuition if open*/
	if(IntuitionBase != NULL)
		CloseLibrary(IntuitionBase);
	
}
struct RastPort *RastPort()
{
	return &IntScreen->RastPort;
}
