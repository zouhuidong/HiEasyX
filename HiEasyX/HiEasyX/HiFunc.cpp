#include "HiFunc.h"

#include <iostream>
#include <time.h>

ScreenSize GetScreenSize()
{
	int left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	return { left,top,w,h };
}

// 获取图像尺寸
// 可以方便地处理 IMAGE 指针为空，即指向主绘图窗口的情况
void GetImageSize(IMAGE* pImg, int* width, int* height)
{
	// 普通 IMAGE 指针
	if (pImg)
	{
		*width = pImg->getwidth();
		*height = pImg->getheight();
	}

	// nullptr 对应绘图窗口
	else
	{
		IMAGE* pOld = GetWorkingImage();
		SetWorkingImage();
		*width = getwidth();
		*height = getheight();
		SetWorkingImage(pOld);
	}
}

DWORD* ReverseAlpha(DWORD* pBuf, int size)
{
	for (int i = 0; i < size; i++)
		if (pBuf[i] & 0xff000000)
			pBuf[i] &= 0x00ffffff;
		else
			pBuf[i] |= 0xff000000;
	return pBuf;
}

HBITMAP Image2Bitmap(IMAGE* img, bool enable_alpha)
{
	// 测试结论
	// 若图像中有任何像素 alpha 不为 0，则启用 alpha
	// 若图像 alpha 全部为 0，则表示完全不透明

	DWORD* pBuf = GetImageBuffer(img);
	if (!enable_alpha)
		for (int i = 0; i < img->getwidth() * img->getheight(); i++)
			pBuf[i] &= 0x00ffffff;
	return CreateBitmap(img->getwidth(), img->getheight(), 1, 32, pBuf);
}

IMAGE Bitmap2Image(HBITMAP* hBitmap, bool enable_alpha)
{
	BITMAP bm;
	GetObject(*hBitmap, sizeof(bm), &bm);
	int width = bm.bmWidth;
	int height = bm.bmHeight;

	IMAGE img(width, height);
	DWORD* pBuf = GetImageBuffer(&img);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	HDC hMemDC = CreateCompatibleDC(NULL);
	GetDIBits(hMemDC, *hBitmap, 0, height, pBuf, &bmi, DIB_RGB_COLORS);
	DeleteDC(hMemDC);

	if (!enable_alpha)
		for (int i = 0; i < width * height; i++)
			pBuf[i] &= 0x00ffffff;

	return img;
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

bool IsInRect(int x, int y, RECT rct)
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
