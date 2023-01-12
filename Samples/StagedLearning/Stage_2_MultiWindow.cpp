/**
 * @brief 	第 2 阶段：创建多窗口
 * @note
 * 			此例将 EasyX 官网的两个示例分别运行在两个绘图窗口中 <p>
 *			它们分别是“字符阵”和“鼠标操作” <p>
 *
 * @author 	huidong <mailhuid@163.com>
 * @date 	2023.01.13
*/

#include "HiEasyX.h"

// EasyX 示例“字符阵”
void CharacterArray()
{
	// 定时绘制
	static clock_t tRecord = 0;
	static int  x, y, i = 0;
	static char c;

	// 第一次绘制，设置绘图属性
	if (!tRecord)
	{
		settextstyle(16, 8, _T("Courier"));
		settextcolor(GREEN);
		setbkcolor(BLACK);
		cleardevice();
	}

	// 定时绘制
	if (clock() - tRecord >= 10)
	{
		// 在随机位置显示三个随机字母
		for (int j = 0; j < 3; j++)
		{
			x = (rand() % 80) * 8;
			y = (rand() % 20) * 24;
			c = (rand() % 26) + 65;
			outtextxy(x, y, c);
		}

		// 画线擦掉一个像素行
		line(0, i, 639, i);

		tRecord = clock();

		if (i >= 479)	i = -1;
		i++;
	}
}

// EasyX 示例“鼠标操作”
void OnMouse()
{
	ExMessage m;
	while (peekmessage(&m, EM_MOUSE))
	{
		switch (m.message)
		{
		case WM_MOUSEMOVE:
			putpixel(m.x, m.y, RED);		// 鼠标移动的时候画红色的小点
			break;

		case WM_LBUTTONDOWN:
			if (m.ctrl)						// 如果点左键的同时按下了 Ctrl 键
				rectangle(m.x - 10, m.y - 10, m.x + 10, m.y + 10);
			else
				rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
			break;
		}
	}
}

int main()
{
	// 设置随机种子
	srand((unsigned)time(NULL));

	// 创建两个绘图窗口
	HWND hWnd1 = hiex::initgraph_win32(640, 480, false, L"字符阵");
	HWND hWnd2 = hiex::initgraph_win32(640, 480, false, L"鼠标操作示例");

	while (true)
	{
		// 若窗口 1 未被关闭
		if (hiex::IsAliveWindow(hWnd1))
		{
			BEGIN_TASK_WND(hWnd1);	// 设置窗口 1 为目标绘图窗口，并启动一个绘图任务
			CharacterArray();		// 绘制
			END_TASK();				// 结束任务
			REDRAW_WINDOW();		// 刷新绘制内容到窗口
		}

		// 若窗口 2 未被关闭
		if (hiex::IsAliveWindow(hWnd2))
		{
			BEGIN_TASK_WND(hWnd2);
			OnMouse();
			END_TASK();
			REDRAW_WINDOW();
		}

		// 若所有窗口都被关闭，则结束程序
		if (!hiex::IsAnyWindow())
		{
			break;
		}

		// 降低 CPU 占用
		Sleep(!(clock() % 77) ? 50 : 1);
	}

	return 0;
}
