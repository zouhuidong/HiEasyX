> [回到教程目录](./index.md)

# 创建托盘

看一个很简单的示例代码即可：

```cpp
#include "HiEasyX.h"

#define IDC_A	101
#define IDC_B	102

void OnTray(UINT id)
{
	BEGIN_TASK();

	switch (id)
	{
	case IDC_A:
		outtextxy(100, 100, L"A");
		break;

	case IDC_B:
		outtextxy(100, 100, L"B");
		break;
	}

	END_TASK();
	REDRAW_WINDOW();
}

int main()
{
	hiex::Window wnd;
	wnd.Create();

	wnd.CreateTray(L"Tray Name");

	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, IDC_A, L"选项 A");
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);			// 分隔符
	AppendMenu(hMenu, MF_STRING, IDC_B, L"选项 B");

	wnd.SetTrayMenu(hMenu);					// 设置菜单
	wnd.SetTrayMenuProcFunc(OnTray);		// 设置菜单响应函数

	hiex::init_end();						// 阻塞等待所有窗口关闭
	DestroyMenu(hMenu);						// 销毁菜单，释放内存
	return 0;
}

```

运行此示例代码，将会在托盘中创建您的程序图标。只要在托盘菜单里面点击某一项，程序就会作出反应。

注意，在 HiEasyX 中，托盘是和窗口挂钩的，每个窗口原则上只能创建一个托盘。

> [回到教程目录](./index.md)
