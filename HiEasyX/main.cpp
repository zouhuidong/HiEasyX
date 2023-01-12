#include "HiEasyX.h"

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.CenterText(L"Wonderful");

	hiex::Canvas graphics(201, 201);
	graphics.FillCircle(100, 100, 100, true, RED, GREEN);
	ReverseAlpha(graphics.GetBuffer(), graphics.GetBufferSize());

	//canvas.PutImageIn_Alpha(120, 120, &graphics, { 0 }, 140, true);

	canvas.BeginBatchDrawing();
	graphics.RenderTo(0, 0, canvas.Pt());
	canvas.EndBatchDrawing();

	REDRAW_WINDOW();
	getmessage(EM_CHAR);
	closegraph();
	return 0;
}
