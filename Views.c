#include "exec/types.h"
#include "exec/exec.h"

#include "graphics/view.h"
#include "graphics/gfxbase.h"
#include "graphics/gfxmacros.h" /*includes rastport.h*/

#include "Views.h"


SCREENVIEW Screen;
struct GfxBase *GfxBase;

UWORD ColorTable16[16] =
{
	0x000, 0xeca, 0xc00, 0xf60,
	0x090, 0x3f1, 0x00f, 0x2cd,
	0xf0c, 0xa0f, 0x950, 0xfca,
	0xfe0, 0xccc, 0x888, 0x444
};

UWORD ColorTable32[32] =
{
	0x000, 0xfff, 0xf00, 0xc00,
	0x800, 0x0f0, 0x0c0, 0x080,
	0xf0f, 0xc0c, 0x8a8, 0x04f,
	0x04c, 0x028, 0xff0, 0xca0,
	0x620, 0xe52, 0xc86, 0xfb9,
	0x222, 0x444, 0x555, 0x666,
	0x777, 0x888, 0x999, 0xaaa,
	0xccc, 0xddd, 0xeee, 0x0ff
};

void CreateView(UWORD Width, UWORD Height, UBYTE Depth)
{
	Screen.Width = Width;
	Screen.Height = Height;
	Screen.Depth = Depth;
	
	
	if ((GfxBase = (struct GfxBase*)
		OpenLibrary("graphics.library", 0)) == NULL)
	{
		printf("Graphics failed to open\n");
		Exit(1000);
	}
	
	Screen.oldView = GfxBase->ActiView;
	
	InitView(&Screen.view);
	InitVPort(&Screen.viewPort);
	Screen.view.ViewPort = &Screen.viewPort;
	InitBitMap(&Screen.bitMap[0], Depth, Width, Height);
	InitBitMap(&Screen.bitMap[1], Depth, Width, Height);
	InitRastPort(&Screen.rastPort[0]);
	InitRastPort(&Screen.rastPort[1]);
	Screen.rastPort[0].BitMap = &Screen.bitMap[0];
	Screen.rastPort[1].BitMap = &Screen.bitMap[1];
	
	
	CreateRasInfo();
	CreateViewPort();
	AllocateBitMapMemory();
	
	MakeVPort(&Screen.view, &Screen.viewPort);
	MrgCop(&Screen.view);
	
	/*store lists for double buffering*/
	Screen.LOF[0] = Screen.view.LOFCprList;
	Screen.SHF[0] = Screen.view.SHFCprList;
	
	/*prepare rasinfo for double buffer*/
	Screen.viewPort.RasInfo = &Screen.rasInfo[1];
	Screen.rasInfo[1].Next = &Screen.rasInfo[0];

	
	/*important, otherwise no 2nd copper list*/
	Screen.view.LOFCprList = NULL;
	Screen.view.SHFCprList = NULL;
	
	/*same as above for double buffering*/
	MakeVPort(&Screen.view, &Screen.viewPort);
	MrgCop(&Screen.view);
	
	/*store 2nd copper list*/
	Screen.LOF[1] = Screen.view.LOFCprList; 
	Screen.SHF[1] = Screen.view.SHFCprList;
	
	LoadView(&Screen.view);
	
}

void CreateRasInfo()
{
	BYTE i;
	for(i = 0; i < 2; i++)
	{
		Screen.rasInfo[i].BitMap = &Screen.bitMap[i];
		Screen.rasInfo[i].RxOffset = 0;
		Screen.rasInfo[i].RyOffset = 0;
		Screen.rasInfo[i].Next = NULL;
	}
}
void CreateViewPort()
{
	UBYTE colours = 2;
	UBYTE i;

	Screen.viewPort.RasInfo = &Screen.rasInfo[0];
	Screen.rasInfo[0].Next = &Screen.rasInfo[1];
	
	Screen.viewPort.DWidth = Screen.Width;
	Screen.viewPort.DHeight = Screen.Height;
	Screen.viewPort.DxOffset = 0;
	Screen.viewPort.DyOffset = 0;
	Screen.viewPort.Modes = 0;
	if(Screen.Width > 320)
		Screen.viewPort.Modes |= HIRES;
	if(Screen.Height > 256)
		Screen.viewPort.Modes |= LACE;
	
	Screen.viewPort.Modes |= SPRITES;
	
	for(i = 0; i < Screen.Depth-1; i++)
	{
		colours *= 2;
	}

	Screen.viewPort.ColorMap = (struct ColorMap *)GetColorMap(colours);
	
	if(Screen.Depth == 4)
		LoadRGB4(&Screen.viewPort, &ColorTable16[0], colours);
	else if(Screen.Depth == 5)
		LoadRGB4(&Screen.viewPort, &ColorTable32[0], colours);
}
void AllocateBitMapMemory()
{
	UBYTE i, j;
	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < Screen.Depth; i++)
		{
			if((Screen.bitMap[j].Planes[i] = (PLANEPTR)AllocRaster(Screen.Width, Screen.Height)) == NULL)
			{
				printf("Cannot allocate memory for bitmap.\n");
				exit(1000);
			}
			BltClear((UBYTE*)Screen.bitMap[j].Planes[i], RASSIZE(Screen.Width, Screen.Height), 0);
		}
	}
}

void FreeCopperMemory()
{
	UBYTE i, j;
	FreeColorMap(Screen.viewPort.ColorMap);
	
	FreeCprList(Screen.LOF[0]);
	FreeCprList(Screen.SHF[0]);
	FreeCprList(Screen.LOF[1]);
	FreeCprList(Screen.SHF[1]);
	
	FreeVPortCopLists(&Screen.viewPort);
	

	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < Screen.Depth; i++)
		{
			FreeRaster(Screen.bitMap[j].Planes[i], Screen.Width, Screen.Height);
		}
	}
}

void MakeDisplay(UBYTE toggle)
{
	Screen.view.LOFCprList = Screen.LOF[toggle];
	Screen.view.SHFCprList = Screen.SHF[toggle];
	LoadView(&Screen.view);
}
void CloseGfxLibrary()
{
	CloseLibrary(GfxBase);
}
