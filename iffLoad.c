#include "exec/types.h"
#include "exec/exec.h"
#include "proto/dos.h"
#include "graphics/gfx.h"
#include "iffLoad.h"


VOID LoadBitMap(UBYTE *file, struct BitMap *mybm)
{
	int i, bytes, x, y, counter, byte, plane;
	unsigned char code, temp;
	BPTR filePtr;
	CHUNK chunk;
	BODY Body;
	char id[2];
	
	unsigned char * buffer;

	filePtr = Open(file, (LONG)MODE_OLDFILE);
	if(filePtr == 0)
	{
		printf("Could not open file\n");
		Close(filePtr);
		exit(1);
	}
		
	Seek(filePtr, 0, OFFSET_BEGINNING);
	Read(filePtr, (VOID *)&chunk, sizeof(CHUNK));
	
	while(id[0] != 'B' && id[1] != 'O')
	{
		Read(filePtr, (VOID *)&id, 2);
	}
	Body.ChunkId[0] = 'B';
	Body.ChunkId[1] = 'O';
	Read(filePtr, (VOID *)&Body.ChunkId[2], 2);
	Read(filePtr, (VOID *)&Body.Size, 4);
	
	
	
	bytes = chunk.BitMapHeader.Width / 8;
	counter = 0;
	byte = 0;
		
	/*if bitmap has been initialise already, i dont want to do it again
	I 'll check this by checking the depth.  Anything above 6 should me..not initialised*/
	if(mybm->Depth > 6)
	{
		InitBitMap(mybm, chunk.BitMapHeader.BitPlanes, chunk.BitMapHeader.Width, chunk.BitMapHeader.Height);
		
		for(plane = 0; plane < mybm->Depth; plane++)
		{
			if((mybm->Planes[plane] = (PLANEPTR)AllocRaster(mybm->BytesPerRow * 8, mybm->Rows)) == NULL)
			{
				printf("Cannot allocate memory for bitmap.\n");
				exit(1000);
			}
			BltClear((UBYTE*)mybm->Planes[plane], RASSIZE(mybm->BytesPerRow * 8, mybm->Rows), 0);
		}
	}

	for(y = 0; y < mybm->Rows; y++)
	{
		for(x = 0; x < mybm->Depth; x++)
		{
			buffer = &mybm->Planes[x][y * mybm->BytesPerRow];
			counter = 0;
			while(counter < mybm->BytesPerRow)
			{
				Read(filePtr, (unsigned char *)&code, 1);
	
				if(code < 128)
				{
					Read(filePtr, (unsigned char *)&buffer[counter], (code + 1));
					counter = (counter + code + 1);
				}
				else if(code > 128)
				{
					Read(filePtr, (unsigned char *)&temp, 1);
					for(i = counter; i < (counter + 257 - code); i++)
					{
						buffer[i] = temp;
					}
					counter = counter + 257-code;
				}
				else
				{
				}	
			}
		}
	}
	
	Close(filePtr);
}

IMAGE GetImage(struct BitMap *BitMap, UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
	IMAGE image;
	UWORD *ImageData, *myHelper;
	WORD plane, x, y, width, xOffset;
	width = (x2 - x1) / 16; 			/*width of image in WORDS*/
	xOffset = x1/8; 					/*offset from left of image in bytes*/
	
	/*setup IMAGE struct*/
	image.Height = y2 - y1;
	image.Width = x2 - x1;
	image.Depth = BitMap->Depth;
	
	/*allocate CHIP memory for ImageData*/
	ImageData = (UWORD *)AllocMem(RASSIZE(image.Width, image.Height) * image.Depth, MEMF_CHIP|MEMF_CLEAR);
	
	myHelper = ImageData;
	
	for(plane = 0; plane < BitMap->Depth; plane++)
	{
		for(y = y1; y < y2; y++)
		{
			for(x = 0; x < width; x++)
			{
				*myHelper |= BitMap->Planes[plane][y * BitMap->BytesPerRow + xOffset + (x * sizeof(UWORD))];
				*myHelper = *myHelper << 8;
				*myHelper |= BitMap->Planes[plane][y * BitMap->BytesPerRow + xOffset + 1 + (x * sizeof(UWORD))];
				myHelper++;
			}
		}
	}
	
	image.ImageData = ImageData;

	return image;
}

VOID DeleteBitMap(struct BitMap *bm)
{
	WORD plane = 0;
	for(plane = 0; plane < bm->Depth; plane++)
	{
		FreeRaster(bm->Planes[plane], bm->BytesPerRow * 8, bm->Rows);
	}
}


