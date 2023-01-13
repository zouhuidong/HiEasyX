#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);

	// 编辑框
	hiex::SysEdit edit;
	edit.PreSetStyle({ true, false, true, true });	// 预设样式可以不用创建结构体，直接简写成这样
	edit.Create(wnd.GetHandle(), 10, 10, 280, 140, L"Type here~");
	edit.SetFont(24, 0, L"微软雅黑");

	// 按钮
	hiex::SysButton btn;
	btn.Create(wnd.GetHandle(), 190, 160, 100, 30, L"Submit");

	while (wnd.IsAlive())
	{
		// 按下按钮时，弹窗显示输入的文本
		if (btn.IsClicked())
			MessageBox(wnd.GetHandle(), edit.GetText().c_str(), L"Submit", MB_OK);
		Sleep(50);
	}

	return 0;
}