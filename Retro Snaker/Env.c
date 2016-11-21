#include "Env.h"
#include <stdlib.h>

VOID ENV_DrawRect(HDC hdc, POINT pt)
{
	INT iPixel = Map_GetPixel();
	Rectangle(hdc, pt.x * iPixel, pt.y * iPixel, (pt.x + 1) * iPixel, (pt.y + 1) * iPixel);
}

VOID ENV_DrawEllipse(HDC hdc, POINT pt)
{
	INT iPixel = Map_GetPixel();
	Ellipse(hdc, pt.x * iPixel, pt.y * iPixel, (pt.x + 1) * iPixel, (pt.y + 1) * iPixel);
}


BOOL ENV_IsKnocked(POINT* Body, INT BodyLength, POINT pt)
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

BOOL ENV_IsEaten()
{
	INT i;
	BOOL flag = FALSE;
	POINT ptHead;
	POINT ptFoodPos;

	ptHead = Snake_GetHead();
	for(i = 0; i < TOTAL_FOODTYPE; i++)
	{
		ptFoodPos = Food_GetPos(i);
		if ((ptHead.x == ptFoodPos.x && ptHead.y == ptFoodPos.y))
		{
			flag = TRUE;
			if(i != FOODTYPE_NORMAL)
				ENV_SuperFood(i);
			break;
		}
	}

	return flag;
}

VOID ENV_ResetFood(INT FoodType)
{
	srand((unsigned)time(NULL));
	do
	{
		Food_Reset(FoodType);
	} while (ENV_IsKnocked(Snake_GetBody(), Snake_GetBodyLength(), Food_GetPos(FoodType)) || ENV_IsEaten());
	
}

VOID ENV_DrawMap(HDC hdc)
{
	POINT ptClient = Map_GetClientSize();
	SelectObject(hdc, CreatePen(PS_DASH, 0, 0));

	MoveToEx(hdc, Map_GetWorldLength(), ptClient.y / 2, NULL);
	LineTo(hdc, ptClient.x, ptClient.y / 2);

	DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
	Rectangle(hdc, 0, 0, Map_GetWorldLength(), Map_GetWorldLength());
}

VOID ENV_DrawFood(HDC hdc, INT FoodType)
{
	POINT ptFoodPos = Food_GetPos(FoodType);
	ENV_DrawEllipse(hdc, ptFoodPos);
}

VOID ENV_DrawSnake(HDC hdc)
{
	static POINT ptTail = {-1, -1};
	POINT* Body = Snake_GetBody();
	INT BodyLength = Snake_GetBodyLength();
	
	ENV_DrawRect(hdc, Body[0]);
	if (ptTail.x != -1)
	{
		if (ptTail.x != Body[BodyLength].x ||
			ptTail.y != Body[BodyLength].y)
		{
			SelectObject(hdc, GetStockObject(WHITE_PEN));
			ENV_DrawRect(hdc, ptTail);
			SelectObject(hdc, GetStockObject(BLACK_PEN));
		}
	}
	ptTail = Body[BodyLength - 1];
	
}

VOID ENV_DrawBody(HDC hdc)
{
	int i;
	POINT* Body = Snake_GetBody();
	INT BodyLength = Snake_GetBodyLength();
	for (i = 0; i < BodyLength; i++)
		ENV_DrawRect(hdc, Body[i]);
}

VOID ENV_GameOver(HWND hwnd, POINT* Path)
{
	//用来处理游戏结束之后的事
	POINT Client = Map_GetClientSize();

	KillTimer(hwnd, SnakeSpeed);
	free(Path);
	switch (MessageBox(hwnd, TEXT("GAME OVER"), TEXT("HaHaHa"),
		MB_RETRYCANCEL))
	{
	case IDRETRY:
		SendMessage(hwnd, WM_CREATE, 0, 0);
		SendMessage(hwnd, WM_SIZE, 0, MAKELPARAM(Client.x, Client.y));
		SendMessage(hwnd, WM_PAINT, 0, 0);
		break;

	case IDCANCEL:
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	}
}

VOID ENV_SuperFood(INT FoodType)
{
	//超级食物会有不同的效果
}

RECT ENV_SetScoreWindow(HWND hwnd)
{
	RECT rtScore;
	POINT ptClient = Map_GetClientSize();
	INT WorldLength = Map_GetWorldLength();

	MoveWindow(hwnd, WorldLength, 0, ptClient.x - WorldLength, ptClient.y / 2, TRUE);
	SetRect(&rtScore, WorldLength, 0, ptClient.x, ptClient.y / 2);
	return rtScore;
}
