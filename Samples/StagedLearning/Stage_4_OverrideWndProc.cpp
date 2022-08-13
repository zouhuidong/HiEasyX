///////////////////////////////////////////////////////////
//
//	HiEasyX 使用示例
//
//	展示了如何处理关闭窗口消息
//	
//	绘图部分源码来自 https://codebus.cn/dudugang/drawing
//

#include "HiEasyX.h"
#include <time.h>

int x;								// X 轴坐标
int y;								// y 轴坐标
int g_multiples = 0;				// 倍数：作用是对弧度角进行改变
float g_PI = (float)3.1415;			// 圆周率
float g_radianAngle;				// 起始角的弧度
int w = 830;						// 窗口宽
int h = 580;						// 窗口高

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

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return HIWINDOW_DEFAULT_PROC;
		break;
	}

	return 0;
}

int main()
{
	// 创建绘图窗口，并指定窗口过程函数
	hiex::initgraph_win32(w, h, false, L"", WndProc);

	// 设置背景色
	setbkcolor(WHITE);

	while (true)
	{
		// 启动一个绘图任务
		// 由于只创建了一个窗口，所以不需要指定目标绘图窗口
		// 否则就需要调用 BEGIN_TASK_WND() 宏，指定目标绘图窗口
		BEGIN_TASK();

		// 进行一些绘制，由于调用的都是 EasyX 绘图函数，不再注释
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

		// HiEasyX 默认使用双缓冲绘图，此处输出绘图缓冲
		// 注意：一段绘图任务结束，必须以此宏结尾（即 BEGIN_TASK 和 END_TASK 必须连用）
		END_TASK();

		// 不在窗口过程函数的 WM_PAINT 消息内绘图时，必须强制重绘
		// 由于没有自定义窗口过程函数，所以当然也要调用此宏强制重绘
		FLUSH_DRAW();

		while (true)
		{
			// 窗口大小改变时重绘
			if (hiex::isWindowSizeChanged())
			{
				break;
			}

			// 若窗口被关闭，则结束程序
			if (!hiex::isAnyWindow())
			{
				return 0;
			}

			Sleep(10);
		}
	}
}

