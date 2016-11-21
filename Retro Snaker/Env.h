#ifndef _ENVORIMENT_H_
#define _ENVORIMENT_H_

#include "Snake.h"
#include "Map.h"
#include "Food.h"

VOID ENV_DrawRect(HDC hdc, POINT pt);
VOID ENV_DrawEllipse(HDC hdc, POINT pt);

BOOL ENV_IsKnocked(POINT* Body, INT BodyLength, POINT pt);
BOOL ENV_IsEaten();
VOID ENV_ResetFood(INT FoodType);
VOID ENV_DrawMap(HDC hdc);
VOID ENV_DrawFood(HDC hdc, INT FoodType);
VOID ENV_DrawSnake(HDC hdc);
VOID ENV_DrawBody(HDC hdc);
VOID ENV_GameOver(HWND hwnd, POINT* Path);
VOID ENV_SuperFood(INT FoodType);
RECT ENV_SetScoreWindow(HWND hwnd);
#endif