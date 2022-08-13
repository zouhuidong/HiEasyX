/////////////////////////////////////////////////////////////
//
//	第 1 阶段：使用系统 GUI
//	
//	此例中，仅做简单演示。
// 
//	对于更多控件的使用示例，请看示例文件中的 OverviewSample.cpp
//	可以在 HiSysGUI.h 中查看支持的系统控件，
//	并查看它们的头文件以了解详细用法。
//

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

	hiex::SysButton btn(wnd.GetHandle(), 10, 10, 100, 30, L"Button 1");
	btn.RegisterMessage(OnClick);

	hiex::init_end();
	return 0;
}
