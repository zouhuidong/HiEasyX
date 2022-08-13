#include "HiEasyX.h"

void OnClick()
{
	BEGIN_TASK();
	outtextxy(50, 50, L"You clicked the button");
	END_TASK();
	FLUSH_DRAW();
}

int main()
{
	hiex::Window wnd(640, 480);

	hiex::SysButton btn(wnd.GetHandle(),10,10,100,30,L"Button 1");
	btn.RegisterMessage(OnClick);

	hiex::init_end();
	return 0;
}
