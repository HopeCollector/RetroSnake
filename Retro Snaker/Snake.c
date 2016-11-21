#include "Snake.h"
struct Snake Snake;

VOID Snake_UpdateBody(POINT ptCopy_1)
{
	INT i;
	POINT ptCopy_2;

	for(i = 1; i < Snake.iBodyLength ; i++)
	{
		ptCopy_2 = Snake.ptBody[i];
		Snake.ptBody[i] = ptCopy_1;
		ptCopy_1 = ptCopy_2;
	}
}


VOID Snake_Grow()
{
	Snake.iBodyLength++;
}

VOID Snake_MoveForward()
{
	POINT ptCopy;

	ptCopy = Snake.ptBody[0];

	if (Snake.ptBody[0].x != Snake.ptBody[1].x)
		Snake.ptBody[0].x += Snake.ptBody[0].x > Snake.ptBody[1].x ? 1 : -1;
	else
		Snake.ptBody[0].y += Snake.ptBody[0].y > Snake.ptBody[1].y ? 1 : -1;

	Snake_UpdateBody(ptCopy);
}

VOID Snake_TurnAround(WPARAM wParam)
{
	POINT ptCopy;
	if (Snake_JudgeAuto() || Snake_JudgeIsPressed())
			return ;
		
		ptCopy = Snake.ptBody[0];

		switch (wParam)
		{
		case VK_DOWN:
		case VK_UP:
			if (Snake.ptBody[0].x != Snake.ptBody[1].x)
			{
				Snake.ptBody[0].y += wParam == VK_UP ? -1 : 1;
				Snake.bIsPressed = TRUE;
			}
			break;

		case VK_LEFT:
		case VK_RIGHT:
			if (Snake.ptBody[0].y != Snake.ptBody[1].y)
			{
				Snake.ptBody[0].x += wParam == VK_LEFT ? -1 : 1;
				Snake.bIsPressed = TRUE;
			}
			break;

		default:
			return ;
		}
		if(Snake.bIsPressed)
			Snake_UpdateBody(ptCopy);
}

VOID Snake_Reset()
{
	INT i;

	Snake.iSpeed = 50;
	Snake.iBodyLength = 5;
	Snake.bDead = 0;	
	Snake.bIsPressed = 0;
	Snake.bAuto = 0;
	Snake.bFindPath = 0;

	Snake.ptBody[0].x = MAX_WORLD / 2;
	Snake.ptBody[0].y = MAX_WORLD / 2;
	for (i = 1; i < Snake.iBodyLength; i++)
	{
		Snake.ptBody[i].x = Snake.ptBody[i - 1].x - 1;
		Snake.ptBody[i].y = Snake.ptBody[i - 1].y;
	}
}

VOID Snake_ChangeSpeed(HWND hwnd, INT iSpeed)
{
	KillTimer(hwnd, SnakeSpeed);
	SetTimer(hwnd, SnakeSpeed, iSpeed, NULL);
	Snake.iSpeed = iSpeed;
}

VOID Snake_FindPath(POINT * lpptPath, POINT ptFood, BOOL IsKnocked(POINT* Body, INT BodyLength, POINT pt))
{
	//用虚拟蛇身走一遍寻找到的路径
	//如果虚拟蛇头能找到虚拟蛇尾，则控制真实蛇身移动
	//如果找不到，向着远离食物的地方移动一格
	POINT * _temp = (POINT*)malloc(sizeof(POINT) * MAX_WORLD * MAX_WORLD);
	POINT * LogicBody = (POINT*)malloc(sizeof(POINT) * MAX_WORLD * MAX_WORLD);
	INT i, j = 0;
	INT LogicBodyLength = 0;

	FindPath(Snake.ptBody[0], ptFood, IsKnocked, Snake.ptBody, Snake.iBodyLength, lpptPath);
	if (lpptPath->x != -1)
	{
		if (Snake.iBodyLength + 1 <= lpptPath->x)//用来更新虚拟蛇身
		{
			for (i = 0; i < Snake.iBodyLength + 1; i++)
			{
				LogicBody[i] = lpptPath[i + 1];
				LogicBodyLength++;
			}
		}
		else 
		{
			for (i = 0; i < lpptPath[0].x; i++)
			{
				LogicBody[i] = lpptPath[i + 1];
				LogicBodyLength++;
			}
			for (i = 0; i < Snake.iBodyLength - lpptPath[0].x + 1; i++)
			{
				LogicBody[i + lpptPath[0].x] = Snake.ptBody[i];
				LogicBodyLength++;
			}
		}
		FindPath(LogicBody[0], LogicBody[LogicBodyLength - 1], IsKnocked, LogicBody, LogicBodyLength - 1, _temp);
		if (_temp->x != -1 && _temp->x > 1)
		{
			free(LogicBody);
			free(_temp);
			return;
		}		
	}
	FindFarPoint(Snake.ptBody[0], ptFood, lpptPath, IsKnocked, Snake.ptBody, Snake.iBodyLength);
	free(LogicBody);
	free(_temp);
}

VOID Snake_MoveFallowPath(POINT * lpptPath)
{
	static INT i;
	if (i == 0)
	{
		i = lpptPath->x;
		Snake_SetFindPath(TRUE);
	}
	Snake.ptBody[0] = lpptPath[i--];
	if (i == 0)
		Snake_SetFindPath(FALSE);
}

VOID Snake_SetDead(BOOL bMood)
{
	Snake.bDead = bMood;
}

VOID Snake_SetIsPressed(BOOL bMood)
{
	Snake.bIsPressed = bMood;
}

VOID Snake_SetAuto(BOOL bMood)
{
	Snake.bAuto = bMood;
}

VOID Snake_SetFindPath(BOOL bMood)
{
	Snake.bFindPath = bMood;
}

BOOL Snake_JudgeDead()
{
	return Snake.bDead;
}

BOOL Snake_JudgeIsPressed()
{
	return Snake.bIsPressed;
}

BOOL Snake_JudgeAuto()
{
	return Snake.bAuto;
}

BOOL Snake_JudgeFindPath()
{
	return Snake.bFindPath;
}

POINT* Snake_GetBody()
{
	return Snake.ptBody;
}

POINT Snake_GetHead()
{
	return Snake.ptBody[0];
}

INT Snake_GetBodyLength()
{
	return Snake.iBodyLength;
}