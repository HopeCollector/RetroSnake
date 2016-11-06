#include "Snake.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc_Score(HWND, UINT, WPARAM, LPARAM);

INT World[MAX_WORLD][MAX_WORLD];
//蛇,食物
POINT Food;
//分数
INT score;
//尺寸
INT cxClient, cyClient, cWorldLength, clpPixel;

TCHAR szChildClassName[] = TEXT("SCORE_CHILD");


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int CmdShow)
{
	TCHAR szAppName[] = TEXT("Retro Snake Main");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndclass);

	wndclass.lpfnWndProc = WndProc_Score;
	wndclass.lpszClassName = szChildClassName;
	wndclass.hIcon = NULL;

	RegisterClass(&wndclass);

	hwnd = CreateWindow(szAppName, TEXT("Retro Snaker"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, CmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
//利用数组记录蛇的位置，方便以后增加寻路算法
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndChild;
	HDC hdc;
	PAINTSTRUCT ps;
	static BOOL Dead, IsPressed, Auto, FindPath;
	static RECT rtWorld, rtScore;
	POINT Copy;
	INT i, x, y;
	static POINT *Path;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 50, NULL);

		Path = (POINT*)malloc(sizeof(POINT) * MAX_WORLD * MAX_WORLD);

		Dead = FALSE;
		IsPressed = FALSE;
		Auto = TRUE;
		FindPath = FALSE;

		BodyLength = 5;
		score = 0;
		hwndChild = CreateWindow(szChildClassName, NULL,
			WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd, (HMENU)1,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

		for (x = 0; x < MAX_WORLD; x++)
			for (y = 0; y < MAX_WORLD; y++)
				World[x][y] = 0;
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		cWorldLength = min(cxClient, cyClient);
		clpPixel = cWorldLength / MAX_WORLD;

		MoveWindow(hwndChild, cWorldLength, 0, cxClient - cWorldLength, cyClient / 2, TRUE);

		//设置矩形
		SetRect(&rtWorld, 1, 1, cWorldLength - 1, cWorldLength - 1);
		SetRect(&rtScore, cWorldLength, 0, cxClient, cyClient / 2);

		//初始化蛇身
		Body[0].x = MAX_WORLD / 2;
		Body[0].y = MAX_WORLD / 2;
		World[MAX_WORLD / 2][MAX_WORLD / 2] = 1;
		for (i = 1; i < BodyLength; i++)
		{
			Body[i].x = Body[i - 1].x - 1;
			Body[i].y = Body[i - 1].y;

			World[Body[i].y][Body[i].x] = 1;
		}

		//初始化食物
		World[Food.x = rand() % MAX_WORLD][Food.y = rand() % MAX_WORLD] = 2;
		while (IsKnocked(Body, BodyLength, Food)) {
			World[Food.x][Food.y] = 0;
			World[Food.x = rand() % MAX_WORLD][Food.y = rand() % MAX_WORLD] = 2;
		}
		//绘制开始界面
		hdc = GetDC(hwnd);
		//边界
		SelectObject(hdc, CreatePen(PS_DASH, 0, 0));

		MoveToEx(hdc, cWorldLength, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);

		DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
		Rectangle(hdc, 0, 0, cWorldLength, cWorldLength);

		//食物
		DrawPixel_Ellipse(hdc, Food);

		
		ReleaseDC(hwnd, hdc);

		return 0;

	case WM_TIMER:
		if (!Dead)
		{
			InvalidateRect(hwnd, &rtWorld, TRUE);
			InvalidateRect(hwndChild, NULL, TRUE);
			IsPressed = FALSE;

			if (Auto)
			{
				Copy.x = Body[0].x;
				Copy.y = Body[0].y;
				if (!FindPath)
					Snake_FindPath(Path);
				MoveBodyFollowPath(Path, &FindPath);
				UpdateBody(Copy);
			}
		}
		
		return 0;

	case WM_KEYDOWN:
		if (IsPressed || Auto)
			return 0;
		
		Copy.x = Body[0].x;
		Copy.y = Body[0].y;

		switch (wParam)
		{
		case VK_DOWN:
		case VK_UP:
			if (Body[0].x != Body[1].x)
			{
				Body[0].y += wParam == VK_UP ? -1 : 1;
				IsPressed = TRUE;
			}
			break;

		case VK_LEFT:
		case VK_RIGHT:
			if (Body[0].y != Body[1].y)
			{
				Body[0].x += wParam == VK_LEFT ? -1 : 1;
				IsPressed = TRUE;
			}
			break;

		default:
			return 0;
		}
		if(IsPressed)
		{
			UpdateBody(Copy);
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}
		return 0;

	case WM_CHAR:
		if (wParam == 'a')
			Auto = TRUE;
		return 0;

	case WM_PAINT://唯一的功能是，画一条蛇的身子
		if (!Dead)
		{
			hdc = BeginPaint(hwnd, &ps);

			DrawPixel_Ellipse(hdc, Food);
			if(!IsPressed && !Auto)
				MoveBody();
			DrawBody(hdc);
			EndPaint(hwnd, &ps);
			if (IsEaten())
			{
				score++;
				BodyLength++;
				srand((unsigned)time(NULL));
				World[Food.x = rand() % MAX_WORLD][Food.y = rand() % MAX_WORLD] = 2;
				while (IsKnocked(Body, BodyLength, Food) || IsEaten()) {
					World[Food.x][Food.y] = 0;
					World[Food.x = rand() % MAX_WORLD][Food.y = rand() % MAX_WORLD] = 2;
				}
				InvalidateRect(hwndChild, &rtScore, TRUE);
				FindPath = FALSE;
				if (score == MAX_WORLD * MAX_WORLD - 7)
				{
					KillTimer(hwnd, 1);
					free(Path);
					switch (MessageBox(hwnd, TEXT("Mission Complete"), TEXT("HaHaHa"),
						MB_RETRYCANCEL))
					{
					case IDRETRY:
						ReSetBody();
						SendMessage(hwnd, WM_CREATE, 0, 0);
						SendMessage(hwnd, WM_SIZE, 0, MAKELPARAM(cxClient, cyClient));
						SendMessage(hwnd, WM_PAINT, 0, 0);
						return 0;

					case IDCANCEL:
						SendMessage(hwnd, WM_DESTROY, 0, 0);
						return 0;
					}
				}
			}
			if (IsKnocked(Body, BodyLength, Body[0]))
			{
				KillTimer(hwnd, 1);
				free(Path);
				Dead = TRUE;
				switch (MessageBox(hwnd, TEXT("YOU DEAD"), TEXT("HaHaHa~~"),
					MB_RETRYCANCEL))
				{
				case IDRETRY:
					ReSetBody();
					SendMessage(hwnd, WM_CREATE, 0, 0);
					SendMessage(hwnd, WM_SIZE, 0, MAKELPARAM(cxClient, cyClient));
					SendMessage(hwnd, WM_PAINT, 0, 0);
					return 0;

				case IDCANCEL:
					SendMessage(hwnd, WM_DESTROY, 0, 0);
					return 0;
				}
			}
		}

		return 0;

	case WM_DESTROY:
		//KillTimer(hwnd, 1);
		//free(Path);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc_Score(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Score[5];
	RECT rect_text;
	RECT rect_num;

	switch (message)
	{
	case WM_PAINT:

		GetClientRect(hwnd, &rect_num);
		rect_text.left = rect_text.top = 0;
		rect_text.right = rect_num.right;
		rect_text.bottom = rect_num.bottom - 4 * clpPixel;

		hdc = BeginPaint(hwnd, &ps);
		//计分板
		DrawText(hdc, TEXT("SCORE"), 5, &rect_text,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		wsprintf(Score, TEXT("%04d"), score);
		DrawText(hdc, Score, 4, &rect_num,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}