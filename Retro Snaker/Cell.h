#pragma once
#ifndef _CELL_
#define _CELL_
#include <Windows.h>

typedef struct
{
	POINT pt;
	BOOL bIsJudged;
	INT iDistance;
	POINT ptComfrom;
}CELL;

VOID GetNewCell(CELL*);
VOID GetCellByPt(CELL * cHead, INT iMaxChild, POINT pt, CELL* cReturn);//如果能找到就返回CELL的指针，反之就返回NULL
#endif // !_CELL_