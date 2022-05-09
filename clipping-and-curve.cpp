#include <Windows.h>
#include <cmath>

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc + x, yc - y, c);
}

void BresenhamCircle(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0,
		y = R;

	int d = 1 - R,
		d1 = 3,
		d2 = 5 - 2 * R;

	Draw8Points(hdc, xc, yc, 0, R, c);

	while (x < y)
	{
		if (d < 0) {
			d += d1;
			d2 += 2;
		}
		else {
			d += d2;
			d2 += 4;
			y--;
		}
		x++;
		d1 += 2;

		Draw8Points(hdc, xc, yc, x, y, c);

	}
}

int xs, ys, xe, ye, r;

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	HDC hdc = GetDC(hwnd);
	static int index = 0;
	switch (mcode)
	{
	case WM_LBUTTONDOWN:
		if (index == 0) {
			xs = LOWORD(lp);
			ys = HIWORD(lp);
			index++;
		}
		else {
			xe = LOWORD(lp);
			ye = HIWORD(lp);
			r = sqrt((xe - xs) * (xe - xs) +
				(ye - ys) * (ye - ys));
			BresenhamCircle(hdc, xs, ys, r, RGB(0, 0, 0));
			index = 0;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, mcode, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}