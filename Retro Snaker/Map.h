#ifndef _MAP_H_
#define _MAP_H_

#include "Heap.h"

struct Map
{
	INT ixClient;
	INT iyClient;
	INT iWorldLength;
	INT iPixel;
};

RECT Map_SetMapSize(LPARAM lParam);
POINT Map_GetClientSize();
INT Map_GetWorldLength();
INT Map_GetPixel();
#endif