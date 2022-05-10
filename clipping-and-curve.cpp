#include <Windows.h>
#include <cmath>

int Round(double x)
{
	return (int)(x + 0.5);
}

class Point {

public:
	int x, y;
	Point(int x = 0, int y = 0): x(x), y(y) {}

};

void DrawHermiteCurve(HDC hdc, Point& p1, Point& t1, Point& p2, Point& t2, int xs, int ys, int R, COLORREF c)
{
	int a0 = p1.x,
		a1 = t1.x,
		a2 = -3 * p1.x - 2 * t1.x + 3 * p2.x - t2.x,
		a3 = 2 * p1.x + t1.x - 2 * p2.x + t2.x;

	int b0 = p1.y,
		b1 = t1.y,
		b2 = -3 * p1.y - 2 * t1.y + 3 * p2.y - t2.y,
		b3 = 2 * p1.y + t1.y - 2 * p2.y + t2.y;

	for (double t = 0; t <= 1; t += 0.001)
	{
		double tSquare = t*t,
			tCube = tSquare * t;

		double x = a0 + a1 * t + a2 * tSquare + a3 * tCube;
		double y = b0 + b1 * t + b2 * tSquare + b3 * tCube;

		SetPixel(hdc, x, y, RGB(255, 0, 0));
	}
}

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
	static int circleIndex = 0;
	static int curveIndex = 0;
	static Point p[4];
	switch (mcode)
	{
	case WM_LBUTTONDOWN:
		if (circleIndex == 0) {
			xs = LOWORD(lp);
			ys = HIWORD(lp);
			circleIndex++;
		}
		else {
			xe = LOWORD(lp);
			ye = HIWORD(lp);
			r = sqrt((xe - xs) * (xe - xs) +
				(ye - ys) * (ye - ys));
			BresenhamCircle(hdc, xs, ys, r, RGB(0, 0, 0));
			circleIndex = 0;
		}
		break;
	case WM_RBUTTONDOWN:
		p[curveIndex] = Point(LOWORD(lp), HIWORD(lp));
		if (curveIndex == 3) {
			Point P1(3 * (p[1].x - p[0].x), 3 * (p[1].y - p[0].y));
			Point P2(3 * (p[3].x - p[2].x), 3 * (p[3].y - p[2].y));
			DrawHermiteCurve(hdc, p[0], P1, p[3], P2, xs, ys, r, RGB(255, 0, 0));
			ReleaseDC(hwnd, hdc);
			curveIndex = 0;
		}
		else curveIndex++;
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