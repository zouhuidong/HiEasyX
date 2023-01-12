/**
 * @brief 	第 1 阶段：使用系统 GUI
 * @note
 * 			此例中，仅对按钮控件的调用做简单演示。 <p>
 *			对于更多控件的使用示例，请看示例文件中的 GUISample.cpp。 <p>
 * 			若您想了解更多，可以查阅此项目文档或头文件。  <p>
 *
 * @author 	huidong <mailhuid@163.com>
 * @date 	2023.01.12
*/

#include "HiEasyX.h"

void OnClick()
{
	BEGIN_TASK();
	outtextxy(50, 50, L"You clicked the button");
	END_TASK();
	REDRAW_WINDOW();
}

int main()
{
	hiex::Window wnd(640, 480);

	hiex::SysButton btn(wnd.GetHandle(), 10, 10, 100, 30, L"Button 1");
	btn.RegisterMessage(OnClick);

	hiex::init_end();
	return 0;
}
