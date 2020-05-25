#define IFFLOAD_H

#ifndef TYPES
#define TYPES
typedef struct _ColorMapEntry
{
	BYTE Red;
	BYTE Green;
	BYTE Blue;
}CMAPENTRY;

typedef struct _ColorMapChunk
{
	char ChunkId[4];
	LONG Size;
	/*CMAPENTRY Map[16];*/
	APTR Map;
}CMAP;

typedef struct _BitMapHeader
{
	char ChunkId[4];
	LONG Size;
	UWORD Width;
	UWORD Height;
	UWORD Left;
	UWORD Top;
	BYTE BitPlanes;
	BYTE Masking;
	BYTE Compression;
	BYTE Padding;
	UWORD Transparency;
	BYTE XAspectRatio;
	BYTE YAspectRatio;
	UWORD PageWidth;
	UWORD PageHeight;
}BMHD;

typedef struct _BodyChunk
{
	char ChunkId[4];
	LONG Size;
	/*BYTE ImageData[];*/
	/*unsigned char *ImageData;*/
}BODY;

typedef struct _Chunk
{
	char ChunkId[4];
	LONG Size;
	char TypeID[4];
	BMHD BitMapHeader;
	/*CMAP CMap;*/
	/*BODY Body;*/
}CHUNK;

#define MY_IMAGE
typedef struct MyImage
{
	UWORD Width;
	UWORD Height;
	UWORD Depth;
	UWORD * ImageData;
}IMAGE;

#endif

VOID LoadBitMap(UBYTE *file, struct BitMap *BitMap);
VOID DeleteBitMap(struct BitMap *bitmap);
IMAGE GetImage(struct BitMap *bitmap, UWORD x1, UWORD y1, UWORD x2, UWORD y2);