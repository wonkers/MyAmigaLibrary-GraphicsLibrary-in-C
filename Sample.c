
/*#include <proto/all.h>
#include <stdlib.h>*/

/*for amiga defined types -UWORD, UBYTE etc*/
#include "exec/types.h"
#include "exec/memory.h"

/*Graphics library stuff*/
#include "graphics/gfx.h"
#include "graphics/gfxbase.h"
#include "graphics/gfxmacros.h"
#include "graphics/view.h"
#include "graphics/gels.h"
#include "graphics/sprite.h"

#include "hardware/custom.h"

/*my custom stuff*/
#include "iffload.h"
#include "views.h"
#include "gels.h"
#include "extras.h"
#include "Screens.h"

#include "intuition/intuition.h"

/*set screen width/height and depth*/
#define WIDTH 320
#define HEIGHT 256
#define DEPTH 5

/*set whether in debug mode or not. 1 = yes/true
 used to display any debugging windows showing values or whatever*/
#define DEBUG 1

#define PORT1 1
#define PORT2 2

struct Bob bob, burnsbob;
struct VSprite vsbob, burnsVS, LifeMeterSprite, StatusSprite;
extern struct Custom far custom;
extern struct Screen *IntScreen;
struct SimpleSprite simpleSprite;

UWORD colours[3] = 
{
	0xf00, 0x0f0, 0x00f
};

UWORD chip life[] = 
{
	0xffff, 0x0000,
	0xffff, 0x0000,
	
};
UWORD chip status[]=
{
	0x0180, 0x0000,
	0x0180, 0x0000,
	
	0x0000, 0x0000,
	0x0000, 0x0000,
	
	0x0000, 0x0180,
	0x0000, 0x0180,
	
	0x0000, 0x0000,
	0x0000, 0x0000,
	
	0x0180, 0x0180,
	0x0180, 0x0180,
};

VOID TestDraw(IMAGE image);
VOID Wait();

int main()
{
	UBYTE toggle;
	struct BitMap BitMap;
	IMAGE Cookie[14];
	IMAGE Burns;
	char buf[255];
	UWORD x;
	UWORD Frame = 0;
	BYTE line = 0;
	UWORD TargetX;
	UWORD TargetY;
	BYTE clicks = 0;
	UBYTE Facing = 0;
	
	char *LeftMouse = (char*)0xBFE001;
	
	IMAGE statusImg;
	IMAGE vsprImg;
	
	vsprImg.Depth = 2;
	vsprImg.Height = 2;
	vsprImg.Width = 16;
	vsprImg.ImageData = life;
	
	statusImg.Depth = 2;
	statusImg.Height = 10;
	statusImg.Width = 16;
	statusImg.ImageData = status;

	CreateScreen(WIDTH, HEIGHT, 5);
	
	CreateView(WIDTH, HEIGHT, DEPTH);
	SetupDBCopperLists();
	LoadView(&Screen.view);
		
	/*Load bitmap with cookie sprites/bobs*/
	LoadBitMap("shared:cookie2002", &BitMap);
	Cookie[6] = GetImage(&BitMap, 0, 4, 32, 52);
	Cookie[13] = GetImage(&BitMap, 32, 4, 64, 52);
	
	for(x = 0; x < 6; x++)
	{
		Cookie[x] = GetImage(&BitMap, (UWORD)(x*32), 52, (UWORD)((x*32) + 32), 100);
	}
	for(x = 7; x < 13; x++)
	{
		Cookie[x] = GetImage(&BitMap, (UWORD)((x-7)*32), 99, (UWORD)((x-7)*32+32), 147);
	}
	
	
	/*load bitmap with cookie's face*/
	LoadBitMap("shared:cookieface", &BitMap);
	/*CookieFace = GetImage(&BitMap, 0, 64, 64, 128);*/
	Burns = GetImage(&BitMap, 66,  129, 98, 177);
	/*BurnsFace = GetImage(&BitMap, 56, 64, 120, 128);*/
	
	Move(&Screen.rastPort, 130, 123);
	Text(&Screen.rastPort, "LOADING...", strlen("LOADING..."));
	
	/*load screen.bitmap from IFF file direct to screen.bitmap*/
	LoadBitMap("shared:template", &Screen.bitMap[0]);
	LoadBitMap("shared:template", &Screen.bitMap[1]);
	
	/*perhaps should load view here*/
	/*LoadView(&Screen.view);*/
	
	/*blit cookie's face to the rastport*/
	BltBitMapRastPort(&BitMap, 0, 64, &Screen.rastPort, 0, 195, 64, 64, 0x60);
	
	
	/*remove cookie face bitmap*/
	DeleteBitMap(&BitMap);
	
	/*Create GELS structure and initialise*/
	CreateGELS(&Screen.rastPort, WIDTH, HEIGHT);
	
	/*create cookie character*/
	CreateBob(&vsbob, &bob, 10, 10, &Cookie[6]);
	SetCollisionBob(&vsbob, &bob, &Cookie[6]);
	SetDoubleBufferBob(&vsbob, &bob, &Cookie[6]);
	
	CreateBob(&burnsVS, &burnsbob, 226, 48, &Burns);
	SetCollisionBob(&burnsVS, &burnsbob, &Burns);
	SetDoubleBufferBob(&burnsVS, &burnsbob, &Burns);
	
	/*add bobs to rast*/
	AddBob(&bob, &Screen.rastPort);
	AddBob(&burnsbob, &Screen.rastPort);
	
	SetAPen(&Screen.rastPort, 1);
	
	CreateVSprite(&LifeMeterSprite, 180, 220, &vsprImg, colours);
	/*SetCollisionVSprite(&LifeMeterSprite, &vsprImg);*/
	AddVSprite(&LifeMeterSprite, &Screen.rastPort);
	
	CreateVSprite(&StatusSprite, 0, 0, &statusImg, colours);
	AddVSprite(&StatusSprite, &Screen.rastPort);
	
	/*current bitmap is 0, set toggle to 1, so after we draw the GList, we switch the bitmap 1;*/
	toggle = 1;
	
	TargetX = vsbob.X;
	TargetY = vsbob.Y;
	
	while(clicks < 20)
	{
		if((*LeftMouse & 0x40) != 0x40)
		{
			clicks++;
			TargetX = IntScreen->MouseX - 16;
			TargetY = IntScreen->MouseY - vsbob.Height+2;
			
			if(TargetX % 2 != 0)
				TargetX++;
			if(TargetY % 2 != 0)
				TargetY++;
			
			if(TargetX > vsbob.X)
				Facing = 0;
			else
				Facing = 7;
		}
		
		/*move bob*/
		if(vsbob.X < TargetX)
			vsbob.X += 2;
		else if(vsbob.X > TargetX)
			vsbob.X -=2;
		
		if(vsbob.Y < TargetY)
			vsbob.Y += 2;
		else if(vsbob.Y > TargetY)
			vsbob.Y -=2;
		
		LifeMeterSprite.X = vsbob.X;
		LifeMeterSprite.Y = vsbob.Y;
		StatusSprite.X = vsbob.X  + 20;
		StatusSprite.Y = vsbob.Y + 6;
		
		/*draw to RastPort*/
		
		SortGList(&Screen.rastPort);
		DoCollision(&Screen.rastPort);
		DrawGList(&Screen.rastPort, &Screen.viewPort);

		WaitTOF();
		/*load new view*/
		MakeDisplay(toggle);		
		
		Screen.viewPort.RasInfo->BitMap = &Screen.bitMap[toggle];
		Screen.rastPort.BitMap = &Screen.bitMap[toggle];
		
		if(Frame > 23)
		{
			Frame = 0;
		}
		
		if(vsbob.X == TargetX && vsbob.Y == TargetY)
		{
			vsbob.ImageData = Cookie[6+Facing].ImageData ;
		}
		else
			vsbob.ImageData = Cookie[(Frame/4) + Facing].ImageData;
		
		InitMasks(&vsbob);

		/*flip 0 to 1, or 1 to 0*/
		toggle ^= 1;
		Frame++;
	}
	
	for(x = 0; x < 14; x++)
	{
		FreeMem(Cookie[x].ImageData, RASSIZE(Cookie[x].Width, Cookie[x].Height)*Cookie[x].Depth);
	}
	
	ClearMemory();
	/*relaod the oldview (workbench)*/
	LoadView(Screen.oldView);
	
	/*Freeup all memory saved for the created bobs - but not image data*/
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

VOID Wait()
{
	int i, j;
	for(i = 0; i < 2000000; i++)
	{
		for(j = 0; j < 8; j++)
		{
		}
	}
}
