#pragma once
#include "AStar.h"

#ifndef _SNAKE_
#define _SNAKE_

extern POINT Body[MAX_WORLD * MAX_WORLD];
extern INT BodyLength;
extern INT cxClient, cyClient, cWorldLength, clpPixel;
extern POINT Food;
extern INT World[MAX_WORLD][MAX_WORLD];
#endif // !_SNAKE_

VOID Snake_FindPath(POINT* ptReturn);
BOOL IsKnocked(POINT* Body, INT BodyLength, POINT pt);
BOOL IsEaten(VOID);
VOID DrawBody(HDC hdc);
VOID MoveBody(VOID);
VOID DrawPixel_Rect(HDC hdc, POINT pt);
VOID DrawPixel_Ellipse(HDC hdc, POINT pt);
VOID UpdateBody(POINT copy);
VOID MoveBodyFollowPath(POINT * Path, BOOL* FindPath);
VOID ReSetBody();