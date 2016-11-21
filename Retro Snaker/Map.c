#include "Map.h"

struct Map Map;

RECT Map_SetMapSize(LPARAM lParam)
{
	RECT rtWorld;
	Map.ixClient = LOWORD(lParam);
	Map.iyClient = HIWORD(lParam);

	Map.iWorldLength = min(Map.ixClient, Map.iyClient);
	Map.iPixel = Map.iWorldLength / MAX_WORLD;
	SetRect(&rtWorld, 1, 1, Map.iWorldLength - 1, Map.iWorldLength - 1);
	return rtWorld;
}

POINT Map_GetClientSize()
{
	POINT ptReturn;
	ptReturn.x = Map.ixClient;
	ptReturn.y = Map.iyClient;
	return ptReturn;
}

INT Map_GetWorldLength()
{
	return Map.iWorldLength;
}

INT Map_GetPixel()
{
	return Map.iPixel;
}