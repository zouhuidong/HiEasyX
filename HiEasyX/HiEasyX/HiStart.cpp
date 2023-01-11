#include "HiStart.h"

#include <graphics.h>
#include <time.h>

// 定义全局变量
POINT* g_pDst;     // 点集(目标)
POINT* g_pSrc;     // 点集(源)
int g_nWidth;      // 文字的宽度
int g_nHeight;     // 文字的高度
int g_nCount;      // 点集包含的点的数量

// 是否为边缘点
bool IsEdgePoint(int x, int y)
{
	//return getpixel(x, y) == WHITE;

	//POINT t[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
	//POINT t[4] = { {0,2}, {0,-2}, {2,0}, {-2,0} };
	POINT t[4] = { {0,4}, {0,-1}, {4,0}, {-1,0} };
	if (getpixel(x, y) != WHITE)
	{
		return false;
	}
	for (int i = 0; i < 4; i++)
	{
		if (getpixel(x + t[i].x, y + t[i].y) != WHITE)
		{
			return true;
		}
	}
	return false;
}

// 获取目标点集
void GetDstPoints()
{
	// 设置临时绘图对象
	IMAGE img;
	SetWorkingImage(&img);

	// 定义目标字符串
	TCHAR s[] = _T("Hi EasyX");

	// 计算目标字符串的宽高，并调整临时绘图对象的尺寸
	setcolor(WHITE);
	setfont(120, 0, _T("Arial"), 0, 0, 1000, false, false, false);
	g_nWidth = textwidth(s);
	g_nHeight = textheight(s);
	Resize(&img, g_nWidth, g_nHeight);

	// 输出目标字符串至 img 对象
	outtextxy(0, 0, s);

	// 计算构成目标字符串的点的数量
	int x, y;
	g_nCount = 0;
	for (x = 0; x < g_nWidth; x++)
		for (y = 0; y < g_nHeight; y++)
			if (IsEdgePoint(x, y))
				g_nCount++;

	// 计算目标数据
	g_pDst = new POINT[g_nCount];
	int i = 0;
	for (x = 0; x < g_nWidth; x++)
		for (y = 0; y < g_nHeight; y++)
			if (IsEdgePoint(x, y))
			{
				g_pDst[i].x = x + (640 - g_nWidth) / 2;
				g_pDst[i].y = y + (480 - g_nHeight) / 2 - 20;
				i++;
			}

	// 恢复对屏幕的绘图操作
	SetWorkingImage(nullptr);
}

// 获取源点集
void GetSrcPoints()
{
	// 设置随机种子
	srand((unsigned int)time(nullptr));

	// 设置随机的源数据
	g_pSrc = new POINT[g_nCount];
	for (int i = 0; i < g_nCount; i++)
	{
		g_pSrc[i].x = rand() % 640;
		g_pSrc[i].y = rand() % 480;
	}
}

// 全屏模糊处理(忽略屏幕第一行和最后一行)
void Blur(DWORD* pMem)
{
	for (int i = 640; i < 640 * 479; i++)
	{
		pMem[i] = RGB(
			(GetRValue(pMem[i]) + GetRValue(pMem[i - 640]) + GetRValue(pMem[i - 1]) + GetRValue(pMem[i + 1]) + GetRValue(pMem[i + 640])) / 5.f,
			(GetGValue(pMem[i]) + GetGValue(pMem[i - 640]) + GetGValue(pMem[i - 1]) + GetGValue(pMem[i + 1]) + GetGValue(pMem[i + 640])) / 5.1f,
			(GetBValue(pMem[i]) + GetBValue(pMem[i - 640]) + GetBValue(pMem[i - 1]) + GetBValue(pMem[i + 1]) + GetBValue(pMem[i + 640])) / 5.1f);
	}
}

void HiEasyX::RenderStartScene()
{
	initgraph(640, 480);				// 创建绘图窗口
	setbkcolor(RGB(80, 80, 80));
	cleardevice();
	setbkmode(TRANSPARENT);

	DWORD* pBuf = GetImageBuffer();

	GetDstPoints();						// 获取目标点集
	GetSrcPoints();						// 获取源点集

	// 运算
	int x, y;
	for (int i = 2; i <= 256; i += 2)
	{
		//int gray = i - 1 - 50;		// 暗
		//int gray = i - 1 + 10;		// 亮
		int gray = i - 1 + 100;			// 正常
		if (gray < 0) gray = 0;
		if (gray > 255) gray = 255;
		COLORREF c = RGB(gray, gray, gray);
		Blur(pBuf);                     // 全屏模糊处理

		for (int d = 0; d < g_nCount; d++)
		{
			x = g_pSrc[d].x + (g_pDst[d].x - g_pSrc[d].x) * i / 256;
			y = g_pSrc[d].y + (g_pDst[d].y - g_pSrc[d].y) * i / 256;
			pBuf[y * 640 + x] = c;        // 直接操作显示缓冲区画点
			//putpixel(g_pDst[d].x, g_pDst[d].y, WHITE);
		}

		LPCTSTR lpszText = L"Animation Author :  慢羊羊";
		outtextxy(getwidth() - textwidth(lpszText), getheight() - textheight(lpszText), lpszText);

		Sleep(10);                    	// 延时
	}

	// 清理内存
	delete g_pDst;
	delete g_pSrc;

	Sleep(2000);
	ShowWindow(GetHWnd(), SW_HIDE);
}
