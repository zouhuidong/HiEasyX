#include "HiEasyX.h"
#include <conio.h>

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.GP_SetLineColor(RGBA(20, 20, 200, 100));
	canvas.GP_SetLineWidth(22);
	canvas.GP_EnableAlpha(true);

	canvas.GP_Line(240, 250, 400, 250);

	canvas.SetTypeface(L"黑体");
	canvas.SetFont(32);
	canvas.SetBkMode(TRANSPARENT);
	canvas.CenterText(L"一级标题");

	_getch();

	closegraph();
	return 0;
}
