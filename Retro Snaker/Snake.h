#ifndef _SNAKE_H_
#define _SNAKE_H_
//私有
#include "AStar.h"
#define SnakeSpeed 1

struct Snake{
	POINT ptBody[MAX_WORLD * MAX_WORLD];
	INT iSpeed;
	INT iBodyLength;
	BOOL bDead;
	BOOL bAuto;
	BOOL bIsPressed;
	BOOL bFindPath;
};

VOID Snake_UpdateBody(POINT ptCopy);

//公有
VOID Snake_Grow();
VOID Snake_MoveForward();
VOID Snake_TurnAround(WPARAM wParam);
VOID Snake_Reset();
VOID Snake_ChangeSpeed(HWND hwnd, INT iSpeed);
VOID Snake_FindPath(POINT * lpptPath, POINT ptFood, BOOL IsKnocked(POINT* Body, INT BodyLength, POINT pt));//判断碰撞的函数有问题
VOID Snake_MoveFallowPath(POINT * lpptPath);

VOID Snake_SetDead(BOOL bMood);
VOID Snake_SetIsPressed(BOOL bMood);
VOID Snake_SetAuto(BOOL bMood);
VOID Snake_SetFindPath(BOOL bMood);

BOOL Snake_JudgeDead();
BOOL Snake_JudgeIsPressed();
BOOL Snake_JudgeAuto();
BOOL Snake_JudgeFindPath();
POINT* Snake_GetBody();
POINT Snake_GetHead();
INT Snake_GetBodyLength();
#endif