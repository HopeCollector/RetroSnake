#pragma once

#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include <Windows.h>
#endif // !1

#ifndef _CELL_
#define _CELL_

typedef struct
{
	POINT pt;
	BOOL bIsJudged;
	INT iDistance;
	POINT ptComfrom;
}CELL;
#endif // !_CELL_

VOID GetNewCell(CELL*);
VOID GetCellByPt(CELL * cHead, INT iMaxChild, POINT pt, CELL* cReturn);//如果能找到就返回CELL的指针，反之就返回NULL