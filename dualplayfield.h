#ifndef DUALPLAYFIELD_H
#define DUALPLAYFIELD_H

#include <exec/types.h>
#include <graphics/rastport.h>
#include <graphics/view.h>

#define BACK 0
#define FRONT 1
#define WIDTH 320
#define HEIGHT 256
#define DEPTH 3



short CreateDualPlayField();
void FreeDPFBitMaps();
void FreeCopper();
void FreeDualPlayField();

#endif