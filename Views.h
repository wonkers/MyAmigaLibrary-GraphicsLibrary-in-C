#define VIEWS


/*Double Buffered Screen*/
typedef struct ScreenView
{
	int Width;
	int Height;
	int Depth;
	struct View view;
	struct ViewPort viewPort;
	struct RasInfo rasInfo[2];
	struct BitMap bitMap[2];
	struct RastPort rastPort[2];
	struct View *oldView;
	
	struct cprlist *LOF[2];
	struct cprlist *SHF[2];
}SCREENVIEW;

extern SCREENVIEW Screen;

void CreateView(UWORD Width, UWORD Height, UBYTE Depth);

void CreateRasInfo();
void CreateViewPort();
void AllocateBitMapMemory();
void FreeCopperMemory();
void MakeDisplay(UBYTE toggle);
void CloseGfxLibrary();

