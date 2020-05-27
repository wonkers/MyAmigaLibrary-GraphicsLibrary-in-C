/*for amiga defined types -UWORD, UBYTE etc*/
#include "exec/types.h"

/*Graphics library stuff*/
#include "graphics/gfx.h"
#include "graphics/gfxbase.h"
#include "graphics/gfxmacros.h"
#include "graphics/view.h"
#include "graphics/gels.h"

#include "hardware/custom.h"

/*my custom stuff*/
#include "iffload.h"
#include "views.h"
#include "gels.h"
#include "extras.h"

/*set screen width/height and depth*/
#define WIDTH 320
#define HEIGHT 256
#define DEPTH 5

/*set whether in debug mode or not. 1 = yes/true
 used to display any debugging windows showing values or whatever*/
#define DEBUG 1

struct Bob bob;
struct VSprite vsbob;

/*not used at the moment - for accessing hardware directly*/
extern struct Custom far custom;

/*used to make sure image data is correct*/
VOID TestDraw(IMAGE image);

int main()
{
	UBYTE toggle;
	struct BitMap BitMap;
	IMAGE Cookie;
	IMAGE CookieFace;
	char buf[6];
	char *LeftMouse = (char*)0xBFE001;

	CreateView(WIDTH, HEIGHT, DEPTH);
	
	/*Load bitmap with cookie sprites/bobs*/
	LoadBitMap("dh1:cookie2002", &BitMap);
	Cookie = GetImage(&BitMap, 0, 6, 32, 54);
	
	/*load bitmap with cookie's face*/
	LoadBitMap("shared:cookieface", &BitMap);
	CookieFace = GetImage(&BitMap, 0, 64, 64, 128);
	
	/*load from IFF file direct to screen.bitmap*/
	LoadBitMap("shared:template", &Screen.bitMap[0]);
	LoadBitMap("shared:template", &Screen.bitMap[1]);
	
	/*set current bitmap to bitmap 0*/
	Screen.viewPort.RasInfo[0].BitMap = &Screen.bitMap[0];
	Screen.rastPort.BitMap = &Screen.bitMap[0];
	
	/*blit cookie's face to the rastport*/
	BltBitMapRastPort(&BitMap, 0, 64, &Screen.rastPort, 0, 195, 64, 64, 0x60);
	
	/*Make View*/
	MakeVPort(&Screen.view, &Screen.viewPort);
	MrgCop(&Screen.view);
	LoadView(&Screen.view);
	
	/*remove cookie face bitmap*/
	DeleteBitMap(&BitMap);
	
	/*Create GELS structure and initialise*/
	CreateGELS(&Screen.rastPort, WIDTH, HEIGHT);
	
	/*create cookie character*/
	CreateBob(&vsbob, &bob, 10, 10, &Cookie);
	SetCollisionBob(&vsbob, &bob, &Cookie);
	SetDoubleBufferBob(&vsbob, &bob, &Cookie);
	
	/*add bobs to rast*/
	AddBob(&bob, &Screen.rastPort);
	
	SetAPen(&Screen.rastPort, 1);

	/*current bitmap is 0, set toggle to 1, so after we draw the GList, we switch the bitmap 1;*/
	toggle = 1;
	if(DEBUG)
	{
		Move(&Screen.rastPort, 236, 0);
		Draw(&Screen.rastPort, 319, 0);
		Draw(&Screen.rastPort, 319, 40);
		Draw(&Screen.rastPort, 236, 40);
		Draw(&Screen.rastPort, 236, 0);
	}
	
	
	
	while((*LeftMouse & 0x40) == 0x40)
	{
		itoas(vsbob.X, buf);
		if(DEBUG)
		{
			Move(&Screen.rastPort, 240, 10);
			Text(&Screen.rastPort, "Debugging", strlen("Debugging"));
			
			/*print values to debugging monitor*/
			Move(&Screen.rastPort, 240, 20);
			Text(&Screen.rastPort, "X : ", strlen("X : "));
			Move(&Screen.rastPort, 280, 20);
			Text(&Screen.rastPort, &buf, strlen(&buf));
		}

		/*move bob*/
		/*vsbob.X += 1;*/
		
		/*draw to current bitmap*/
		SortGList(&Screen.rastPort);
		DrawGList(&Screen.rastPort, &Screen.viewPort);
		
		/*need to implement something like this for proper dbuff*/
		
		/*Screen.view->LOFCprList = Screen.Copper[LOF][toggle];
		Screen.view->SHFCprList = Screen.Copper[SHF][toggle];
		LoadView(Screen.view);*/
		
		/*wait until the current scanline reaches the top of screen*/
		/*removing this speeds up drawing alot, but may miss drawing bobs/sprites*/
		WaitTOF();

		/*swap bitmaps - this isn't working as it's supposed to, BUT is keeping the screen clean*/
		Screen.viewPort.RasInfo[0].BitMap = &Screen.bitMap[toggle];
		Screen.rastPort.BitMap = &Screen.bitMap[toggle];
		
		/*flip 0 to 1, or 1 to 0*/
		toggle ^= 1;
	}
	/*relaod the oldview (workbench)*/
	LoadView(Screen.oldView);

	/*Freeup all memory saved for the created bobs*/
	DeleteBob(&vsbob, &bob);
	
	/*Freeup memory in my View - bitmaps, copperlists*/
	FreeCopperMemory();
	
	/*close the GFX library*/
	CloseGfxLibrary();

	return 0;
}

VOID TestDraw(IMAGE image)
{
	WORD d =0;
	WORD plane = 0;
	WORD x, y;
	
	for(plane = 0; plane < image.Depth; plane++)
	{
		for(y = 0; y < image.Height; y++)
		{
			for(x = 0; x < image.Width/8; x+=2)
			{
				Screen.rastPort.BitMap[0].Planes[plane][x + (y*(WIDTH/8))] |= (image.ImageData[d] >> 8);
				Screen.rastPort.BitMap[0].Planes[plane][x + (y*(WIDTH/8)) + 1] |= image.ImageData[d];
				d++;
			}
		}
	}
}
