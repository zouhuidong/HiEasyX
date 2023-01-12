#include "HiEasyX.h"
#include <conio.h>

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	Sleep(1000);

	canvas.EnableAutoMarkFlushWindow(false);

	canvas.GP_SetLineColor(RGBA(20, 20, 200, 100));
	canvas.GP_SetLineWidth(22);
	canvas.GP_EnableAlpha(true);

	canvas.GP_Line(240, 250, 400, 250);

	canvas.SetTypeface(L"黑体");
	canvas.SetFont(32);
	canvas.SetBkMode(TRANSPARENT);
	canvas.CenterText(L"一级标题");

	BEGIN_TASK();
	hiex::FlushDrawing({ 200,200,300,250 });
	END_TASK(false);
	REDRAW_WINDOW();

	canvas.EnableAutoMarkFlushWindow(true);

	hiex::Canvas img;
	img.Load_Image_Alpha(L"C:/Users/mailk/Pictures/aa.png");

	img.ZoomImage_Rough_Alpha(1000, 0);
	img.ZoomImage_Alpha(1000, 0);
	img.ZoomImage_Win32_Alpha(1000, 0);
	//img.ZoomImage_Win32_Alpha(1000, 1000);

	canvas.PutImageIn_Alpha(0, 0, &img, { 0 }, 180);

	//canvas.ZoomImage_Rough_Alpha(220);

	REDRAW_WINDOW();

	hiex::SysEdit edit;
	hiex::SysEdit::PreStyle prestyle;
	prestyle.center_text = true;
	edit.PreSetStyle(prestyle);
	edit.Create(GetHWnd(), 100, 100, 300, 200, L"aaa");


	_getch();

	closegraph();
	return 0;
}
