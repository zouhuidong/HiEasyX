#include "HiEasyX.h"		// 包含 HiEasyX 头文件

int main()
{
	initgraph();			// 初始化窗口

	BEGIN_TASK();			// <<< 启动任务，标识开始绘制

	setbkcolor(BLACK);		// 进行绘制...
	cleardevice();
	settextstyle(42, 0, L"Consolas");
	settextcolor(LIGHTGREEN);
	outtextxy(20, 70, L"Hi~EasyX");
	settextstyle(26, 0, L"system");
	settextcolor(GREEN);
	outtextxy(20, 120, L"Any key to continue");

	END_TASK();				// <<< 完成绘制，结束任务

	FLUSH_DRAW();			// <<< 将绘制内容刷新到屏幕

	getmessage(EM_KEY);		// 任意键继续

	closegraph();			// 关闭窗口
	return 0;
}
