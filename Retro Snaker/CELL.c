#include "Cell.h"

VOID GetNewCell(CELL * cell)
{
	cell->pt.x = 0;
	cell->pt.y = 0;
	cell->bIsJudged = FALSE;
	cell->iDistance = -1;
	cell->ptComfrom.x = 0;
	cell->ptComfrom.y = 0;
}

VOID GetCellByPt(CELL * cHead, INT iMaxChild, POINT pt, CELL* cReturn)
{
	INT i = 0;

	do {
		if (cHead[i].pt.x == pt.x &&
			cHead[i].pt.y == pt.y)
		{
			*cReturn = cHead[i];
			return;
		}
		i++;
	} while (i <= iMaxChild);
	GetNewCell(cReturn);
	return;
}