///////////////////////////////////////////////////////////
//
//	第 4 阶段：重载窗口过程函数
// 
//	此例中，演示了如何重载窗口过程函数，并在关闭窗口时弹窗确认
// 
//	绘制内容	：视觉错觉艺术
//	原 作 者	：梦萤花火
//

#include "HiEasyX.h"

int w = 830;	// 窗口宽
int h = 580;	// 窗口高

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:

		// 关闭窗口时提示是否确认关闭
		if (MessageBox(hWnd, L"确定关闭窗口？", L"提示", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		break;

	default:
		return HIWINDOW_DEFAULT_PROC;
		break;
	}

	return 0;
}

// 视觉错觉艺术
// 作者：梦萤花火
// 博客：https://codebus.cn/dudugang/drawing
void Draw()
{
	int x;								// X 轴坐标
	int y;								// y 轴坐标
	int g_multiples = 0;				// 倍数：作用是对弧度角进行改变
	float g_PI = (float)3.1415;			// 圆周率
	float g_radianAngle;				// 起始角的弧度

	cleardevice();
	setcolor(RGB(185, 230, 0));
	setfillcolor(RGB(185, 230, 0));
	solidrectangle(10, 10, getwidth() - 10, getheight() - 10);
	for (size_t j = 0; j < 11.0 / h * getheight() - 0.5; j++)
	{
		for (size_t i = 0; i < 16.0 / w * getwidth() - 0.5; i++)
		{
			x = 15 + 50 * (int)i;
			y = 15 + 50 * (int)j;
			g_radianAngle = 0 + g_multiples * g_PI / 4;
			i < 15 ? g_multiples++ : g_multiples = g_multiples;
			rectangle(x, y, x + 50, y + 50);
			setfillcolor(RGB(250, 250, 250));
			solidcircle(x + 25, y + 25, 20);
			setfillcolor(RGB(80, 80, 80));
			solidpie(x + 5, y + 5, x + 50 - 5, y + 50 - 5, g_radianAngle, g_radianAngle + g_PI);
			setfillcolor(RGB(158, 2, 251));
			solidcircle(x + 25, y + 25, 15);
		}
	}
}

int main()
{
	// 创建绘图窗口，并指定窗口过程函数
	hiex::initgraph_win32(w, h, false, L"", WndProc);
	
	hiex::AutoExit();	// 窗口关闭时，自动退出程序

	while (true)
	{
		BEGIN_TASK();
		Draw();
		END_TASK();
		FLUSH_DRAW();

		while (!hiex::isWindowSizeChanged())
			Sleep(50);
	}
}

