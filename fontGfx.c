#include "fontGfx.h"

UBYTE characters[96][8];

void SetFontGraphicData(struct RastPort *rastport)
{
    ULONG value;
    int i, j;
    struct TextFont *font;
    font = rastport->Font;

    /*get font characters*/
	for(j = 0; j < 96/4; j++)
	{
		for(i = 0; i < 8; i++)
		{
			value = (ULONG)((font->tf_CharData[ j + (font->tf_Modulo/sizeof(ULONG))*i ]));
			characters[j*4][i] = (value & 0xff000000)>>24;
			characters[j*4+1][i] = (value & 0x00ff0000)>>16;
			characters[j*4+2][i] = (value & 0x0000ff00)>>8;
			characters[j*4+3][i] = (value & 0x000000ff);
		}
	}
}
