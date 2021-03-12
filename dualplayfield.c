#ifndef DUALPLAYFIELD_H
#include "dualplayfield.h"
#endif

struct View View;
struct ViewPort ViewPort;
struct RasInfo RasInfos[2]; 
struct BitMap BitMaps[2];
struct RastPort RastPorts[2];


short CreateDualPlayField()
{
    int i, j;
	InitView(&View);
	InitVPort(&ViewPort);
	View.ViewPort = &ViewPort;

	InitBitMap(&BitMaps[0],DEPTH, WIDTH, HEIGHT);
	InitBitMap(&BitMaps[1],DEPTH, WIDTH, HEIGHT);

	for(i = 0; i < 2; i++)
	{
		RasInfos[i].BitMap = &BitMaps[i];
		RasInfos[i].RxOffset = 0;
		RasInfos[i].RyOffset = 0;
		RasInfos[i].Next = NULL;
	}

	ViewPort.DxOffset = 0;
	ViewPort.DyOffset = 0;
	ViewPort.DWidth = WIDTH;
	ViewPort.DHeight = HEIGHT;
	ViewPort.RasInfo = &RasInfos[BACK];
	ViewPort.Modes = DUALPF|PFBA|SPRITES;
	RasInfos[BACK].Next = &RasInfos[FRONT];
	ViewPort.ColorMap = (struct ColorMap*)GetColorMap(16);

	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < DEPTH; j++)
		{
			if((BitMaps[i].Planes[j] = (PLANEPTR)AllocRaster(WIDTH,HEIGHT)) == NULL)
			{
                FreeDPFBitMaps();
				return FALSE;
			} 
			BltClear((UBYTE *)BitMaps[i].Planes[j], RASSIZE(WIDTH, HEIGHT), 0);
		}
	}

	InitRastPort(&RastPorts[0]);
	RastPorts[0].BitMap = &BitMaps[0];
	InitRastPort(&RastPorts[1]);
	RastPorts[1].BitMap = &BitMaps[1];

	MakeVPort(&View, &ViewPort);
	MrgCop(&View);

	ViewPort.RasInfo = &RasInfos[BACK];
	RasInfos[BACK].Next = &RasInfos[FRONT];

	LoadView(&View);
	return TRUE;
}
void FreeDPFBitMaps()
{
    int i;
    for(i = 0; i < DEPTH; i++)
	{
        if(BitMaps[0].Planes[i])
		    FreeRaster(BitMaps[0].Planes[i], WIDTH, HEIGHT);
        if(BitMaps[1].Planes[i])
		    FreeRaster(BitMaps[1].Planes[i], WIDTH, HEIGHT);
    }
}
void FreeCopper()
{
    FreeVPortCopLists(&ViewPort);
    FreeCprList(View.LOFCprList);
    FreeCprList(View.SHFCprList);
}
void FreeDualPlayField()
{
    FreeDPFBitMaps();
    FreeCopper();
    FreeColorMap(ViewPort.ColorMap);
}
