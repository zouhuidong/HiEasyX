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

	hiex::EasyX_Gdiplus_Line(10, 10, 400, 300, RGBA(0, 200, 200, 100), 50, true, true, GetWorkingImage());

	hiex::Gdiplus_Shutdown();

	//hiex::FlushDrawingToWnd();
	END_TASK();

	_getch();


	closegraph();
	return 0;
}
