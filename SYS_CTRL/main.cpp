#include "../HiEasyX/HiEasyX.h"

#define IDC_EDIT 101

HWND editWindow = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;//定义静态字体变量

	switch (msg)
	{
	case WM_CREATE:
	{
		//获取客户区窗口大小参数
		RECT rct;
		GetClientRect(hWnd, &rct);
		//创建edit编辑窗口，子窗口，可视，有边框，多行，识别enter为回车，失去焦点后光标不消失
		editWindow = CreateWindow(
			L"EDIT",
			nullptr,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL | WS_VSCROLL | ES_AUTOVSCROLL,
			0, 100, rct.right, rct.bottom,
			hWnd,
			(HMENU)IDC_EDIT,
			nullptr,
			nullptr
		);

		hFont = CreateFont(
			24, 0, 0, 0, 0,
			FALSE, FALSE,
			0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
			L"Consolas");
		//创建字体
		//HWND hStatic = CreateWindow("STATIC", "静态文本", WS_CHILD | WS_VISIBLE, 10, 10, 100, 25, hwnd, (HMENU)IDC_FILTER_STATIC, g_hInstance, NULL);//创建静态文本
		SendMessage(editWindow, WM_SETFONT, (WPARAM)hFont, TRUE);//发送设置字体消息	

		//Edit_SetPasswordChar(editWindow, L'*');

		Edit_SetText(editWindow, L"注意：\r\n刚刚");
		Edit_SetReadOnly(editWindow, true);

		break;
	}

	case WM_CTLCOLOREDIT://设置Edit颜色
	{
		if (editWindow == (HWND)lParam)//这里的1是静态文本框的ID
		{
			SetTextColor((HDC)wParam, RGB(0, 122, 204));
			SetBkColor((HDC)wParam, RGB(30, 30, 30));
			//SetBkMode((HDC)wParam, TRANSPARENT);
			return (INT_PTR)CreateSolidBrush(RGB(30, 30, 30));
			//return (LRESULT)GetStockObject(WHITE_BRUSH);
		}
		break;
	}

	case WM_SIZE:
	{
		RECT rct;
		GetClientRect(hWnd, &rct);
		//Edit_SetRect(editWindow, &rct);
		SetWindowPos(editWindow, 0, 0, 100, rct.right, rct.bottom, 0);
		break;
	}

	case WM_PAINT:
	{
		BEGIN_TASK_WND(hWnd);

		circle(30, 30, 30);

		END_TASK();

		break;
	}

	case WM_DESTROY:
	{
		DeleteObject(hFont);//删除所创建字体对象
		PostQuitMessage(0);
		break;
	}

	default:
		return HIWINDOW_DEFAULT_PROC;
		break;

	}

	return 0;
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance = GetModuleHandle(0);
	static HWND hBtn = nullptr;

	switch (msg)
	{
	case WM_CREATE:
	{
		hBtn = CreateWindow(L"Button", L"Press me", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 80, 25, hWnd, nullptr, hInstance, 0);
		break;
	}

	case WM_PAINT:
		BEGIN_TASK_WND(hWnd);
		circle(100, 100, 70);
		END_TASK();
		break;

	case WM_MOVE:
	{
		

		break;
	}

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// TODO: 在此处释放申请的内存

		//FreeLibrary()

		PostQuitMessage(0);
		break;

	default:
		return HIWINDOW_DEFAULT_PROC;	// 标识使用默认消息处理函数继续处理

		// 若要以默认方式处理，请勿使用此语句
		//return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

int main()
{
	HWND hWnd = HiEasyX::initgraph_win32(640, 480, EW_NORMAL, 0, WndProc2);
	//HiEasyX::SetWndProcFunc(nullptr, WndProc2);

	//Sleep(3000);
	//HiEasyX::SysButton btn;
	//btn.Create(hWnd, 10, 50, 80, 25, L"Hello");

	HiEasyX::init_end();
	return 0;
}
