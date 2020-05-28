#define GELS_H

#ifndef MY_IMAGE
#define MY_IMAGE
typedef struct MyImage
{
	UWORD Width;
	UWORD Height;
	UWORD Depth;
	UWORD * ImageData;
}IMAGE;



#endif

VOID CreateGELS(struct RastPort *RastPort, int width, int height);
VOID CreateBob(struct VSprite *vsbob, struct Bob * bob, int x, int y, IMAGE *image); 
VOID SetCollisionBob(struct VSprite *vsbob, struct Bob *bob, IMAGE *image);
VOID SetDoubleBufferBob(struct VSprite *vsbob, struct Bob *bob, IMAGE *image);
VOID DeleteBob(struct VSprite *vsbob, struct Bob *bob);
VOID SetGelsInfo(struct RastPort *RastPort);