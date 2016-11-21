#include "Food.h"

struct Food Food[TOTAL_FOODTYPE];

VOID Food_Reset(INT FoodType)
{
	Food[FoodType].ptPosition.x = rand() % MAX_WORLD;
	Food[FoodType].ptPosition.y = rand() % MAX_WORLD;
}

POINT Food_GetPos(INT FoodType)
{
	return Food[FoodType].ptPosition;
}