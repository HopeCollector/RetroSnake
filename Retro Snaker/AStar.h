#pragma once
#include "Heap.h"

#ifndef _NEIGHBOR_
#define _NEIGHBOR_

extern INT NEIGHBORE[4][2];

#endif // _NEIGHBOR_

BOOL Compare(CELL * cell1, CELL * cell2, POINT ptGoal);//C1´ó·µ»ØTRUE
VOID FindPath(POINT ptStart, POINT ptGoal, BOOL Unavalible(POINT* Body, INT BodyLength, POINT pt), POINT* Body, INT BodyLength, POINT* ptReturn);
VOID FindFarPoint(POINT ptCenter, POINT ptGoal, POINT* ptReturn);
FLOAT Distance(POINT pt1, POINT pt2);