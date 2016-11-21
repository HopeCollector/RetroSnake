#ifndef _FOOD_H_
#define _FOOD_H_

#include "Heap.h"
#define TOTAL_FOODTYPE 1//有几种食物就是几
#define FOODTYPE_NORMAL 0

struct Food
{
	POINT ptPosition;
};

VOID Food_Reset(INT FoodType);
POINT Food_GetPos(INT FoodType);
#endif