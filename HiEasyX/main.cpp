#include "HiEasyX.h"

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.CenterText(L"Wonderful");

	// 由于 canvas 绑定了窗口，所以由外部向 canvas 上绘图前，要先启动窗口任务，
	// 此处使用 BeginBatchDrawing 是因为它会在内部启动窗口任务。
	canvas.BeginBatchDrawing();

	// 绘制透明图形
	DRAW_TNS_INIT_GRAPHICS(201, 201);
	{
		graphics.FillCircle(100, 100, 100, true, RED, SKYBLUE);
	}
	DRAW_TNS_RENDER_TO(120, 120, canvas.Pt(), 100);

	// 对应 BeginBatchDrawing
	canvas.EndBatchDrawing();

	REDRAW_WINDOW();
	getmessage(EM_CHAR);
	closegraph();
	return 0;
}