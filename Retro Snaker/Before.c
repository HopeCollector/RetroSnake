#include <Windows.h>
#include <stdlib.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID DrawBody(HDC, LPPOINT, INT);
VOID MoveBody(LPPOINT, INT);
BOOL IsKnocked(INT, INT, LPPOINT, INT);
VOID ReBoot(VOID);

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
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	POINT copy, tocopy;
	static LPCSTR szScore[5];
	static INT score;
	static BOOL FirstDraw;
	static BOOL IsPressed = FALSE, Dead = FALSE;
	static RECT rect, RectScore_text, RectScore_num, DeadBox;
	static POINT Body[1000], Food;
	static int BodyLong = 5;
	static int cxClient, cyClient, cxChar, cyChar, cxLpMax, cyLpMax;
	static int speed = 500;
	int i;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, speed, NULL);
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		DeleteDC(hdc);
		//初始化
		FirstDraw = TRUE;
		IsPressed = FALSE;
		Dead = FALSE;
		BodyLong = 5;
		score = 0;
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxLpMax = cxClient * 5 / 6 / cxChar;
		cyLpMax = cyClient / cxChar;
		//设置绘图矩形
		SetRect(&rect, 0, 0, cxClient * 5 / 6, cyClient);
		SetRect(&RectScore_text, cxClient * 5 / 6, 0, cxClient, cyClient / 2 - 4 * cyChar);
		SetRect(&RectScore_num, cxClient * 5 / 6, 0, cxClient, cyClient / 2);
		SetRect(&DeadBox, cxLpMax / 4, cyLpMax / 4, cxLpMax * 3 / 4, cyLpMax * 3 / 4);
		//初始化蛇的位置
		Body[0].x = cxLpMax / 2;
		Body[0].y = cyLpMax / 2;
		for (i = 1; i < BodyLong; i++)
		{
			Body[i].x = Body[i - 1].x - 1;
			Body[i].y = Body[i - 1].y;
		}
		//初始化食物
		Food.x = rand() % (cxLpMax - 1);
		Food.y = rand() % (cyLpMax - 1);
		return 0;

	case WM_TIMER:
		IsPressed = FALSE;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_KEYDOWN:
		copy.x = Body[0].x;
		copy.y = Body[0].y;

		switch (wParam)
		{

		case VK_UP:
			if (Body[0].x != Body[1].x && !IsPressed)
			{
				//Body[0].x = Body[1].x;
				Body[0].y --;

				IsPressed = TRUE;
			}
			break;

		case VK_DOWN:
			if(Body[0].x != Body[1].x && !IsPressed)
			{
				//Body[0].x = Body[1].x;
				Body[0].y ++;

				IsPressed = TRUE;
			}
			break;

		case VK_RIGHT:
			if (Body[0].y != Body[1].y && !IsPressed)
			{
				//Body[0].y = Body[1].y;
				Body[0].x ++;

				IsPressed = TRUE;
			}
			break;

		case VK_LEFT:
			if (Body[0].y != Body[1].y && !IsPressed)
			{
				//Body[0].y = Body[1].y;
				Body[0].x --;

				IsPressed = TRUE;
			}
			break;

		default:
			break;
		}
		for (i = 1; i < BodyLong; i++)
		{
			tocopy.x = Body[i].x;
			tocopy.y = Body[i].y;

			Body[i].x = copy.x;
			Body[i].y = copy.y;

			copy.x = tocopy.x;
			copy.y = tocopy.y;

		}

		return 0;

	case WM_CHAR:
		switch (wParam)
		{
		case 'a':
			speed -= 50;
			goto out;
		case 's':
			speed += 50;
			out:
		case 'r':
			SendMessage(hwnd, WM_CREATE, 0, 0);
			SendMessage(hwnd, WM_SIZE, 0, MAKELPARAM(cxClient, cyClient));
			SendMessage(hwnd, WM_PAINT, 0, 0);
			break;

		}
		return 0;

case WM_PAINT:
	if (!Dead)
	{
		hdc = BeginPaint(hwnd, &ps);

		MoveToEx(hdc, cxClient * 5 / 6, 0, NULL);
		LineTo(hdc, cxClient * 5 / 6, cyClient);
		SelectObject(hdc, CreatePen(PS_DASH, 0, 0));
		Rectangle(hdc, cxClient * 5 / 6, 0, cxClient, cyClient / 2);

		DrawText(hdc, TEXT("Score"), 5, &RectScore_text,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		wsprintf(szScore, TEXT("%04d"), score);
		DrawText(hdc, szScore, 4, &RectScore_num,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));

		Rectangle(hdc, 0, 0, cxClient * 5 / 6, cyClient);

		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1, 1, NULL);
		SetViewportExtEx(hdc, cxChar, cxChar, NULL);

		Ellipse(hdc, Food.x - 1, Food.y - 1, Food.x, Food.y);

		MoveBody(Body, BodyLong);
		DrawBody(hdc, Body, BodyLong);
		if (IsKnocked(cxLpMax, cyLpMax, Body, BodyLong))
		{
			Dead = TRUE;
			MessageBox(hwnd, TEXT("YOU DEAD"), TEXT("HAHAHAHA~~"), MB_OK);
		}
		if (Body[0].x == Food.x && Body[0].y == Food.y)
		{
			score += 1;
			BodyLong += 1;
			Food.x = rand() % (cxLpMax - 1);
			Food.y = rand() % (cyLpMax - 1);
			Ellipse(hdc, Food.x - 1, Food.y - 1, Food.x, Food.y);
		}
		EndPaint(hwnd, &ps);
		}

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

VOID DrawBody(HDC hdc, LPPOINT Body, INT BodyLong)
{
	int i;

	for (i = 0; i < BodyLong; i++)
	{
		Rectangle(hdc, Body[i].x - 1, Body[i].y - 1, Body[i].x, Body[i].y);
	}
}

VOID MoveBody(LPPOINT Body, INT BodyLong)
{
	int i;
	POINT copy, tocopy;

	copy.x = Body[0].x;
	copy.y = Body[0].y;

	if (Body[0].x != Body[1].x)
		Body[0].x += Body[0].x > Body[1].x ? 1 : -1;
	else
		Body[0].y += Body[0].y > Body[1].y ? 1 : -1;

	for (i = 1; i < BodyLong; i++)
	{
		tocopy.x = Body[i].x;
		tocopy.y = Body[i].y;

		Body[i].x = copy.x;
		Body[i].y = copy.y;

		copy.x = tocopy.x;
		copy.y = tocopy.y;

	}
}

BOOL IsKnocked(INT cxLpMax, INT cyLpMax, LPPOINT Body, INT BodyLong)
{
	int i;
	BOOL flag = FALSE;
	//判断撞墙
	if (Body[0].x >= cxLpMax)
		flag = TRUE;
	if (Body[0].x <= 0)
		flag = TRUE;
	if (Body[0].y >= cyLpMax)
		flag = TRUE;
	if (Body[0].y <= 0)
		flag = TRUE;

	//判断撞自己
	for (i = 1; i < BodyLong; i++)
	{
		if (Body[0].x == Body[i].x && Body[0].y == Body[i].y)
			flag = TRUE;
	}
	return flag;
}