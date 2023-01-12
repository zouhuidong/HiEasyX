#include "HiEasyX.h"
#include <conio.h>

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.CenterText_Format(64, L"Hello, %ls", L"world");

	canvas.GP_SetLineColor(RGBA(20, 20, 200, 100));
	canvas.GP_SetLineWidth(20);
	canvas.GP_EnableAlpha(true);

	canvas.GP_Line(200, 200, 300, 300);
	
	//hiex::Gdiplus_Starup();
	canvas.GP_Line(300, 200, 300, 300);
	//hiex::Gdiplus_Shutdown();

	_getch();


	closegraph();
	return 0;
}
