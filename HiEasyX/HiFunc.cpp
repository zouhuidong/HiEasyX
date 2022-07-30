#include "HiFunc.h"

#include <iostream>
#include <time.h>


// 获取图像尺寸
// 可以方便地处理 IMAGE 指针为空，即指向主绘图窗口的情况
void GetImageSize(IMAGE* pImg, int& width, int& height)
{
	// 普通 IMAGE 指针
	if (pImg)
	{
		width = pImg->getwidth();
		height = pImg->getheight();
	}

	// nullptr 对应绘图窗口
	else
	{
		IMAGE* pOld = GetWorkingImage();
		SetWorkingImage();
		width = getwidth();
		height = getheight();
		SetWorkingImage(pOld);
	}
}

HBITMAP Image2Bitmap(IMAGE* img)
{
	return CreateBitmap(img->getwidth(), img->getheight(), 1, 32, (void*)GetImageBuffer(img));
}

HICON Bitmap2Icon(HBITMAP hBmp)
{
	BITMAP bmp = {};
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	HBITMAP hbmMask = CreateCompatibleBitmap(GetDC(nullptr), bmp.bmWidth, bmp.bmHeight);

	ICONINFO ii = { 0 };
	ii.fIcon = TRUE;
	ii.hbmColor = hBmp;
	ii.hbmMask = hbmMask;

	HICON hIcon = CreateIconIndirect(&ii);
	DeleteObject(hbmMask);

	return hIcon;
}

void HpSleep(int ms)
{
	static clock_t oldclock = clock();		// 静态变量，记录上一次 tick
	static int c = CLOCKS_PER_SEC / 1000;

	oldclock += ms * c;						// 更新 tick

	if (clock() > oldclock)					// 如果已经超时，无需延时
		oldclock = clock();
	else
		while (clock() < oldclock)			// 延时
			Sleep(1);						// 释放 CPU 控制权，降低 CPU 占用率
//			Sleep(0);						// 更高精度、更高 CPU 占用率
}

bool isInRect(int x, int y, RECT rct)
{
	if (rct.left > rct.right)    std::swap(rct.left, rct.right);
	if (rct.top > rct.bottom)    std::swap(rct.top, rct.bottom);
	return x >= rct.left && x <= rct.right && y >= rct.top && y <= rct.bottom;
}

UINT GetExMessageType(ExMessage msg)
{
	switch (msg.message)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		return EM_MOUSE;
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return EM_KEY;
		break;
	case WM_CHAR:
		return EM_CHAR;
		break;
	case WM_ACTIVATE:
	case WM_MOVE:
	case WM_SIZE:
		return EM_WINDOW;
		break;
	default:
		return 0;
		break;
	}
}
