#include "fontGfx.h"
#define MAX_CHARS 96
#define WIDTHHEIGHT 8
UBYTE characters[MAX_CHARS][WIDTHHEIGHT];

void SetFontGraphicData(struct RastPort *rastport)
{
    ULONG value;
    int i, j;
    struct TextFont *font;
    font = rastport->Font;

    /*get font characters*/
	for(j = 0; j < MAX_CHARS/sizeof(ULONG); j++)
	{
		for(i = 0; i < WIDTHHEIGHT; i++)
		{
			value = (ULONG)((font->tf_CharData[ j + (font->tf_Modulo/sizeof(ULONG))*i ]));
			characters[j*sizeof(ULONG)][i] = (value & 0xff000000)>>24;
			characters[j*sizeof(ULONG)+1][i] = (value & 0x00ff0000)>>16;
			characters[j*sizeof(ULONG)+2][i] = (value & 0x0000ff00)>>8;
			characters[j*sizeof(ULONG)+3][i] = (value & 0x000000ff);
		}
	}
}
