#include "HiEasyX.h"
#include <conio.h>

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.CenterText_Format(64, L"Hello, %ls", L"world");

	BEGIN_TASK();

	hiex::Gdiplus_Starup();

	Gdiplus::Color color(100, 200, 0, 0);

	hiex::Gdiplus_Line(canvas.GetHDC(), 200, 200, 500, 300, color, 10, Gdiplus::SmoothingModeAntiAlias);

	POINT pp[] = { {100,100},{120,150},{200,230} };
	Gdiplus::PointF* pPts = new Gdiplus::PointF[3];
	for (int i = 0; i < 3; i++)
	{
		pPts[i].X = (float)pp[i].x;
		pPts[i].Y = (float)pp[i].y;
	}

	hiex::Gdiplus_Polygon(canvas.GetHDC(), 3, pPts, color, 10, Gdiplus::SmoothingModeAntiAlias);


	hiex::Gdiplus_Shutdown();

	//hiex::FlushDrawingToWnd();
	END_TASK();

	_getch();


	closegraph();
	return 0;
}
