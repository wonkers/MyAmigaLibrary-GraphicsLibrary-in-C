
/*#include <proto/all.h>
#include <stdlib.h>*/

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
#define DEBUG 0

struct Bob bob, facebob;
struct VSprite vsbob, vsface;
extern struct Custom far custom;

char *numbers[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

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

	/*y 6, */
	CreateView(WIDTH, HEIGHT, DEPTH);
	
	/*Load bitmap with cookie sprites/bobs*/
	LoadBitMap("dh1:cookie2002", &BitMap);
	Cookie = GetImage(&BitMap, 0, 6, 32, 54);
	
	/*load bitmap with cookie's face*/
	LoadBitMap("shared:cookieface", &BitMap);
	CookieFace = GetImage(&BitMap, 0, 64, 64, 128);
	
	/*load screen.bitmap from IFF file direct to screen.bitmap*/
	LoadBitMap("shared:template", &Screen.bitMap[0]);
	LoadBitMap("shared:template", &Screen.bitMap[1]);
	
	/*blit cookie's face to the rastport*/
	BltBitMapRastPort(&BitMap, 0, 64, &Screen.rastPort[0], 0, 195, 64, 64, 0x60);
	BltBitMapRastPort(&BitMap, 0, 64, &Screen.rastPort[1], 0, 195, 64, 64, 0x60);
	
	/*remove cookie face bitmap*/
	DeleteBitMap(&BitMap);
	
	/*Create GELS structure and initialise*/
	CreateGELS(&Screen.rastPort[0], WIDTH, HEIGHT);
	
	/*set the 2nd rastport's GelsInfo - uses the same as 1st rastport*/
	SetGelsInfo(&Screen.rastPort[1]);
	
	/*create cookie character*/
	CreateBob(&vsbob, &bob, 10, 10, &Cookie);
	SetCollisionBob(&vsbob, &bob, &Cookie);
	SetDoubleBufferBob(&vsbob, &bob, &Cookie);
	
	/*add bobs to rast*/
	AddBob(&bob, &Screen.rastPort[0]);
	
	SetAPen(&Screen.rastPort[0], 1);

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
			/*print values to monitor*/
			Move(&Screen.rastPort, 240, 20);
			Text(&Screen.rastPort, "X : ", strlen("X : "));
			Move(&Screen.rastPort, 280, 20);
			Text(&Screen.rastPort, &buf, strlen(&buf));
		}

		/*move bob*/
		vsbob.X += 1;
		
		/*draw to RastPort*/
		SortGList(&Screen.rastPort[toggle]);
		DrawGList(&Screen.rastPort[toggle], &Screen.viewPort);
		
		/*load new view*/
		MakeDisplay(toggle);
		
		/*wait until the current scanline reaches the top of screen*/
		/*removing this speeds up drawing alot, but may miss drawing bobs/sprites*/
		WaitTOF();

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
				Screen.rastPort[0].BitMap[0].Planes[plane][x + (y*(WIDTH/8))] |= (image.ImageData[d] >> 8);
				Screen.rastPort[0].BitMap[0].Planes[plane][x + (y*(WIDTH/8)) + 1] |= image.ImageData[d];
				d++;
			}
		}
	}
}
