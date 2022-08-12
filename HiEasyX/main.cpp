///////////////////////////////////////////////////////////
//
//	程序：比较原生 EasyX 和使用 EasyWin32 后的绘图效率
//	结论：原生 EasyX 比 EasyWin32 绘图效率更高
//	
//	作者：huidong<huidong_mail@163.com>
//	环境：VisualStudio 2022 | EasyX_20220116 | Windows 10
//	日期：2022.4.16
//

// 是否定义符号 EASYX 决定程序是否以原生 EasyX 的方式运行
//#define EASYX

#ifdef EASYX
#include <easyx.h>
#include <time.h>
#include <math.h>
#define BEGIN_TASK()
#define END_TASK()
#define TITLE L"A moving ball [ EasyX ]"
#else
#include "HiEasyX.h"
#define TITLE L"A moving ball [ HiEasyX ]"
#endif // EASYX

int main()
{
	int w = 640, h = 480;
	initgraph(w, h);
	BeginBatchDraw();
	BEGIN_TASK();
	setfillcolor(RGB(20, 150, 40));
	END_TASK();

	clock_t t = clock();
	int count = 0;
	wchar_t strFPS[64] = TITLE;
	for (float i = 0, k = (float)0.1, r = 0; i < w; i += k)
	{
		r = (float)pow(i / 5, 1.1);

		if (EasyWin32::BeginTask())
		{
			cleardevice();
			outtextxy(0, 0, strFPS);
			solidcircle((int)i, h / 2, (int)r);
			EasyWin32::EndTask();
			FlushBatchDraw();
		}

		if (i + r >= w - 1 || (i <= 1 && k < 0))	k *= -1;
		if ((double)(clock() - t) / CLOCKS_PER_SEC >= 1)
		{
			wsprintf(strFPS, TITLE L" | %d fps", count);
			t = clock();
			count = 0;
		}
		count++;
	}

	closegraph();
	return 0;
}
