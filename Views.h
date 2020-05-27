#define VIEWS

#define LOF 0
#define SHF 1

/*Double Buffered Screen*/
typedef struct ScreenView
{
	int Width;
	int Height;
	int Depth;
	struct View view;
	struct ViewPort viewPort;
	
	/*2 RasInfos are only required for Dual Playfield*/
	struct RasInfo rasInfo[2];
	
	/*2 Bitmaps are only required for Double Buffering/Dual Playfields*/
	struct BitMap bitMap[2];
	
	struct RastPort rastPort;
	struct View *oldView;
	struct CprList *Copper[2][2];
}SCREENVIEW;

extern SCREENVIEW Screen;

void CreateView(UWORD Width, UWORD Height, UBYTE Depth);

void CreateRasInfo();
void CreateViewPort();
void AllocateBitMapMemory();
void FreeCopperMemory();
void CloseGfxLibrary();

