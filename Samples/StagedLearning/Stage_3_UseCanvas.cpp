/**
 * @brief 	第 3 阶段：使用画布进行绘制
 * @note
 * 			Canvas 封装了 EasyX 的所有绘图函数，使用更方便，而且支持透明通道。 <p>
 *			此例将画布和窗口绑定，并直接在画布上绘图。 <p>
 *
 * @author 	huidong <mailhuid@163.com>
 * @date 	2023.01.13
*/

#include "HiEasyX.h"

int main()
{
	hiex::Window wnd;
	wnd.Create();				// 创建窗口

	hiex::Canvas canvas;
	wnd.BindCanvas(&canvas);	// 绑定窗口的画布，之后就可以直接在画布上绘制

	bool first = true;
	while (wnd.IsAlive())
	{
		if (wnd.IsSizeChanged() || first)
		{
			canvas.Clear();		// 使用画布绘制时无需启动窗口任务
			canvas.Circle(
				canvas.GetWidth() / 2,
				canvas.GetHeight() / 2,
				100
			);

			wnd.Redraw();		// 仍需发送窗口重绘消息

			first = false;
		}

		Sleep(clock() % 77 ? 1 : 50);
	}

	return 0;
}
