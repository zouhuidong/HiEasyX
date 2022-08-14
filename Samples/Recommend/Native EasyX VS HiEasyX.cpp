/**
 * @brief 	比较原生 EasyX 和 HiEasyX 的绘图效率
 * @author 	huidong <mailhuid@163.com>
 * @date 	2022.08.14
*/

// 定义此宏以使用原生 EasyX，否则使用 HiEasyX
//#define EASYX

#ifdef EASYX
#include <easyx.h>
#include <time.h>
#include <math.h>
#define BEGIN_TASK()
#define END_TASK()
#define TITLE L"Native EasyX"
#else
#include "HiEasyX.h"
#define TITLE L"HiEasyX"
#endif // EASYX

#ifndef EASYX

hiex::SysButton btn;
LPCTSTR lpszMode[] = { L"Real Mode",L"Normal Mode",L"Fast Mode",L"Very Fast Mode",L"Fastest Mode" };

void OnBtn()
{
	static int index = 0;
	index++;
	if (index == 5)
		index = 0;

	btn.SetText(lpszMode[index]);
	hiex::SetDrawMode((hiex::DrawMode)index);
}

#endif


int main()
{
	int w = 640, h = 480;
	//int w = 1280, h = 960;
	initgraph(w, h);
	BeginBatchDraw();
	BEGIN_TASK();
	setfillcolor(RGB(20, 150, 40));
	END_TASK();

#ifndef EASYX
	btn.Create(GetHWnd(), 30, 30, 120, 30, L"");
	btn.RegisterMessage(OnBtn);
	OnBtn();
#endif

	clock_t t = clock();
	int count = 0;
	wchar_t strFPS[64] = TITLE;
	for (float i = 0, k = (float)0.1, r = 0; i < w; i += k)
	{
		r = (float)pow(i / 5, 1.1);
		BEGIN_TASK();
		cleardevice();
		outtextxy(0, 0, strFPS);
		solidcircle((int)i, h / 2, (int)r);
		END_TASK();
		FlushBatchDraw();
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
