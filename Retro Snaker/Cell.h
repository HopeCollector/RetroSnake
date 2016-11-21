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
VOID GetCellByPt(CELL * cHead, INT iMaxChild, POINT pt, CELL* cReturn);//������ҵ��ͷ���CELL��ָ�룬��֮�ͷ���NULL
#endif // !_CELL_