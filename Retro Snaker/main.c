#include "Env.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc_Score(HWND, UINT, WPARAM, LPARAM);

INT score;
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndChild;
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rtWorld, rtScore;
	POINT Copy;
	static POINT *Path;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, SnakeSpeed, 200, NULL);

		Path = (POINT*)malloc(sizeof(POINT) * MAX_WORLD * MAX_WORLD);

		Snake_Reset();
		score = 0;
		hwndChild = CreateWindow(szChildClassName, NULL,
			WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd, (HMENU)1,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

		return 0;

	case WM_SIZE:
		rtWorld = Map_SetMapSize(lParam);
		rtScore = ENV_SetScoreWindow(hwndChild);
		//初始化食物
		ENV_ResetFood(FOODTYPE_NORMAL);
		//绘制开始界面
		hdc = GetDC(hwnd);
		ENV_DrawMap(hdc);
		ENV_DrawFood(hdc, FOODTYPE_NORMAL);
		ENV_DrawBody(hdc);
		ReleaseDC(hwnd, hdc);

		return 0;

	case WM_TIMER:
		if (!Snake_JudgeDead())
		{
			InvalidateRect(hwnd, &rtWorld, TRUE);
			InvalidateRect(hwndChild, NULL, TRUE);

			if (Snake_JudgeAuto())
			{
				Copy = Snake_GetHead();
				if (!Snake_JudgeFindPath())
				{
					Snake_FindPath(Path, Food_GetPos(FOODTYPE_NORMAL), ENV_IsKnocked);
					Snake_SetFindPath(TRUE);
				}
				Snake_MoveFallowPath(Path);
				Snake_UpdateBody(Copy);
			}
		}
		return 0;

	case WM_KEYDOWN:
		Snake_TurnAround(wParam);		
		return 0;

	case WM_CHAR:
		if (wParam == 'a')
		{
			Snake_SetAuto(TRUE);
		}
		return 0;

	case WM_PAINT:
		if (!Snake_JudgeDead())
		{
			hdc = BeginPaint(hwnd, &ps);
			ENV_DrawFood(hdc, FOODTYPE_NORMAL);
			if (!Snake_JudgeAuto() && !Snake_JudgeIsPressed())
				Snake_MoveForward();
			ENV_DrawBody(hdc);
			EndPaint(hwnd, &ps);

			Snake_SetIsPressed(FALSE);
			if (ENV_IsEaten())
			{
				score++;
				Snake_Grow();
				ENV_ResetFood(FOODTYPE_NORMAL);
				InvalidateRect(hwndChild, &rtScore, TRUE);
				Snake_SetFindPath(FALSE);
			}
			if ((score == MAX_WORLD * MAX_WORLD - 7) || (ENV_IsKnocked(Snake_GetBody(), Snake_GetBodyLength(), Snake_GetHead())))
			{
				Snake_SetDead(TRUE);
				ENV_GameOver(hwnd, Path);
				return 0;
			}
		}

		return 0;

	case WM_DESTROY:
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
		rect_text.bottom = rect_num.bottom - 4 * Map_GetPixel();

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