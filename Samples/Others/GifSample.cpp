/////////////////////////
//
//	演示播放 gif 动图
//

#include "HiEasyX.h"

int main()
{
	HiEasyX::Window window(640, 480);
	HiEasyX::Window windowChild(300, 200, EW_NORMAL, L"Child Window", nullptr, window.GetHandle());
	HiEasyX::Canvas canvas, canvasChild;
	window.BindCanvas(&canvas);
	windowChild.BindCanvas(&canvasChild);
	DisableResizing(windowChild.GetHandle(), true);

	HWND hwnd = initgraph(200, 200);
	BEGIN_TASK_WND(hwnd);
	rectangle(20, 20, 50, 50);
	END_TASK();

	canvasChild.FillCircle(canvasChild.GetWidth() / 2, canvasChild.GetHeight() / 2, 50);
	windowChild.Redraw();

	canvas.SetFont(72);
	canvas.SetBkMode(TRANSPARENT);

	HiEasyX::Gif gif;
	gif.load(L"test.gif");
	HiEasyX::Canvas canvasGif(200, 200);
	gif.setSize(200, 200);
	gif.bind(canvasGif.GetHDC());

	clock_t t = clock();
	size_t count = 0;
	while (true)
	{
		canvas.Clear();
		canvas.CenterText_Format(24, L"HiEasyX %.5f", PI);

		gif.draw();
		gif.toggle();
		canvas.PutImageIn_Alpha(0, 0, &canvasGif, { 0 }, 100);

		// 已输出帧数
		canvas.OutTextXY_Format(0, 0, 6, L"%d", count);

		window.Redraw();

		if (!window.isAlive())
		{
			exit(0);
		}

		//Sleep(50);

		count++;
	}

	_gettch();
	closegraph();

	return 0;
}
