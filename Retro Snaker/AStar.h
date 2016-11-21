#pragma once
#include "Heap.h"

#ifndef	_ASTAR_H_
#define _ASTAR_H_

BOOL Compare(CELL * cell1, CELL * cell2, POINT ptGoal);//C1´ó·µ»ØTRUE
VOID FindPath(POINT ptStart, POINT ptGoal, BOOL Unavalible(POINT* Body, INT BodyLength, POINT pt), POINT* Body, INT BodyLength, POINT* ptReturn);
VOID FindFarPoint(POINT ptCenter, POINT ptGoal, POINT* ptReturn, BOOL Unavalible(POINT* Body, INT BodyLength, POINT pt), POINT* Body, INT BodyLength);
FLOAT Distance(POINT pt1, POINT pt2);

#endif // _ASTAR_H_

