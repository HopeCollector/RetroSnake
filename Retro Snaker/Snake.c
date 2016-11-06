#include "Snake.h"

extern POINT Body[MAX_WORLD * MAX_WORLD] = { 0, 0 };
extern INT BodyLength  = 5;

VOID DrawBody(HDC hdc)
{
	int i;
	//HBRUSH hbrush;

	for (i = 0; i < BodyLength; i++)
	{
		/*if (i == 0)
		{
			hbrush = SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
			DrawPixel_Rect(hdc, Body[i]);
			SelectObject(hdc, hbrush);
		}
		else if (i == BodyLength - 1)
		{
			hbrush = SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));*/
			DrawPixel_Rect(hdc, Body[i]);
			/*SelectObject(hdc, hbrush);
		}
		else
		{
			hbrush = SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
			DrawPixel_Rect(hdc, Body[i]);
			SelectObject(hdc, hbrush);
		}*/
	}
	//DeleteObject(hbrush);
}

VOID MoveBody(VOID)
{
	POINT copy;

	copy.x = Body[0].x;
	copy.y = Body[0].y;

	if (Body[0].x != Body[1].x)
		Body[0].x += Body[0].x > Body[1].x ? 1 : -1;
	else
		Body[0].y += Body[0].y > Body[1].y ? 1 : -1;

	UpdateBody(copy);
}

VOID Snake_FindPath(POINT * Path)
{
	POINT * _temp = (POINT*)malloc(sizeof(POINT) * MAX_WORLD * MAX_WORLD);
	POINT * LogicBody = (POINT*)malloc(sizeof(POINT) * MAX_WORLD * MAX_WORLD);
	POINT LogicTail = {-1, -1}, ptTemp;
	INT i, j = 0;
	BOOL test;
	INT LogicBodyLength = 0;

	FindPath(Body[0], Food, IsKnocked, Body, BodyLength, Path);
	if (Path->x != -1)
	{
		if (BodyLength + 1 <= Path->x)//用来更新虚拟蛇身
		{
			for (i = 0; i < BodyLength + 1; i++)
			{
				LogicBody[i] = Path[i + 1];
				LogicBodyLength++;
			}
		}
		else {
			for (i = 0; i < Path[0].x; i++)
			{
				LogicBody[i] = Path[i + 1];
				LogicBodyLength++;
			}
			for (i = 0; i < BodyLength - Path[0].x + 1; i++)
			{
				LogicBody[i + Path[0].x] = Body[i];
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
	FindFarPoint(Body[0], Food, Path, IsKnocked, Body, BodyLength);
	free(LogicBody);
	free(_temp);
}

BOOL IsEaten(VOID)
{
	BOOL flag = FALSE;

	if ((Body[0].x == Food.x && Body[0].y == Food.y))
		flag = TRUE;

	return flag;
}

BOOL IsKnocked(POINT* Body, INT BodyLength, POINT pt)
{
	BOOL flag = FALSE;
	int i;
	//判断撞墙
	if (pt.x >= MAX_WORLD || pt.x < 0 || pt.y >= MAX_WORLD || pt.y < 0)
		flag = TRUE;
	//判断撞自己
	for (i = 1; i < BodyLength; i++)
		if (pt.x == Body[i].x && pt.y == Body[i].y)
		{
			flag = TRUE;
			break;
		}

	return flag;
}

VOID DrawPixel_Rect(HDC hdc, POINT pt)
{
	Rectangle(hdc, pt.x * clpPixel, pt.y * clpPixel, (pt.x + 1) * clpPixel, (pt.y + 1) * clpPixel);
}

VOID DrawPixel_Ellipse(HDC hdc, POINT pt)
{
	Ellipse(hdc, pt.x * clpPixel, pt.y * clpPixel, (pt.x + 1) * clpPixel, (pt.y + 1) * clpPixel);
}

VOID UpdateBody(POINT copy)
{
	int i;
	POINT tocopy;

	World[Body[0].y][Body[0].x] = 1;
	World[Body[BodyLength - 1].y][Body[BodyLength - 1].x] = 0;
	for (i = 1; i < BodyLength; i++)
	{
		tocopy.x = Body[i].x;
		tocopy.y = Body[i].y;

		Body[i].x = copy.x;
		Body[i].y = copy.y;

		copy.x = tocopy.x;
		copy.y = tocopy.y;
	}
	World[Body[1].y][Body[1].x] = 1;
	World[Body[BodyLength - 1].y][Body[BodyLength - 1].x] = 1;
}

VOID MoveBodyFollowPath(POINT * Path, BOOL* FindPath)
{
	static INT i;
	if (i == 0)
	{
		i = Path->x;
		*FindPath = TRUE;
	}
	Body[0] = Path[i--];
	if (i == 0)
		*FindPath = FALSE;
}

VOID ReSetBody()
{
	INT i;
	for (i = 0; i < BodyLength; i++)
	{
		Body[i].x = 0;
		Body[i].y = 0;
	}
}
