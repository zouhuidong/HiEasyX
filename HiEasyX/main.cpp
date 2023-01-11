#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(640, 480);
	hiex::Canvas canvas;

	wnd.BindCanvas();

	wnd.CloseWindow();
	return 0;
}