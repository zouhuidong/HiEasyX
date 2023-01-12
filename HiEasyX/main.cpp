#include "HiEasyX.h"

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.CenterText(L"Wonderful");

	DRAW_TNS_INIT_GRAPHICS(201, 201);
	{
	//	graphics.FillCircle(100, 100, 100, true, RED, SKYBLUE);
		setfillcolor(GREEN);
		fillcircle(100, 100, 100);
	}
	DRAW_TNS_RENDER_TO(120, 120, canvas.Pt(), 100);

	REDRAW_WINDOW();
	getmessage(EM_CHAR);
	closegraph();
	return 0;
}
