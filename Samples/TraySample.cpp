////////////////////////////
//
//	HiEasyX 使用示例
//
//	快速创建托盘图标
//

#include "HiEasyX.h"

// 定义托盘菜单的选项 ID
#define IDC_A	101
#define IDC_B	102
#define IDC_C	103
#define IDC_D	104

// 自定义过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 标识是否单击过了托盘图标
	static bool flag = false;

	switch (msg)
	{
		// 处理绘图消息
	case WM_PAINT:
		if (hiex::SetWorkingWindow(hWnd))
		{
			hiex::BeginTask();
			setbkcolor(SKYBLUE);
			cleardevice();
			settextstyle(32, 0, L"system");
			settextcolor(BLACK);
			outtextxy(20, 20, L"Windows Tray Icon Creation Sample");
			if (flag)
			{
				settextstyle(26, 0, L"system");
				settextcolor(PURPLE);
				outtextxy(20, 60, L"You pressed the tray icon.");
			}
			hiex::EndTask();
		}
		break;

		// 处理托盘消息
	case WM_TRAY:
	{
		switch (lParam)
		{
			// 左键按下托盘图标
		case WM_LBUTTONDOWN:
			flag = true;					// 标记按下
			hiex::EnforceRedraw();		// 立即重绘
			break;
		}
	}
	break;

	default:
		return HIWINDOW_DEFAULT_PROC;
		break;
	}

	return 0;
}

// 处理托盘菜单事件
void OnMenu(UINT uId)
{
	switch (uId)
	{
	case IDC_A:
		MessageBox(GetHWnd(), L"A", L"Tip", MB_OK);
		break;
	case IDC_C:
		MessageBox(GetHWnd(), L"C", L"Tip", MB_OK);
		break;
	case IDC_D:
		MessageBox(GetHWnd(), L"D", L"Tip", MB_OK);
		break;
	}
}

int main()
{
	// 创建一个自定义过程函数的绘图窗口
	HWND hWnd1 = hiex::initgraph_win32(640, 480, EW_NORMAL, L"HiEasyX & Tray", WndProc);

	// 制作菜单
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, IDC_A, L"选项 A");
	AppendMenu(hMenu, MF_STRING, IDC_B, L"选项 B");
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);				// 分隔符
	AppendMenu(hMenu, MF_STRING, IDC_C, L"选项 C");
	EnableMenuItem(hMenu, IDC_B, MF_GRAYED);				// 禁用一个选项

	hiex::CreateTray(L"The first");					// 创建托盘
	hiex::SetTrayMenu(hMenu);							// 设置托盘菜单
	hiex::SetTrayMenuProcFunc(OnMenu);					// 设置托盘菜单响应函数

	//
	// 注意：
	//	此示例中为了演示而创建了两个窗口，分别有一个托盘图标。
	//	而在实际应用中几乎不会这样做，一个程序最多一个托盘就够了。
	//	所以托盘图标已固定设置为窗口图标。
	//	HiEasyX 对于托盘功能只是做了简单封装，如果有更多需要，可以自行实现。
	//

	// 创建第二个窗口
	hiex::initgraph_win32(200, 120, EW_NORMAL, L"The Second Window");

	// 此时存在两个窗口，为防止冲突，启动任务
	if(hiex::BeginTask())
	{
		setbkcolor(LIGHTSKYBLUE);
		cleardevice();
		settextcolor(BLACK);
		outtextxy(20, 20, L"The Second Window");

		// 制作菜单
		HMENU hMenu2 = CreatePopupMenu();
		AppendMenu(hMenu2, MF_STRING, IDC_D, L"选项 D");

		hiex::CreateTray(L"The second");				// 创建托盘
		hiex::SetTrayMenu(hMenu2);						// 设置托盘菜单
		hiex::SetTrayMenuProcFunc(OnMenu);				// 设置托盘菜单响应函数
	
		hiex::EndTask();
		hiex::EnforceRedraw();
	}

	hiex::SetWorkingWindow(hWnd1);						// 将任务权交还第一个窗口
	SetForegroundWindow(hWnd1);

	hiex::init_end();									// 阻塞等候
	return 0;
}

