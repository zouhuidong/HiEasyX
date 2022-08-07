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
			rct.left, rct.top, rct.right, rct.bottom,
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
		break;
	}

	case WM_CTLCOLOREDIT://设置Edit颜色
	{
		if (GetDlgItem(hWnd, IDC_EDIT) == (HWND)lParam)//这里的1是静态文本框的ID
		{
			SetTextColor((HDC)wParam, RGB(0, 122, 204));
			//SetBkMode((HDC)wParam, TRANSPARENT);
			return (INT_PTR)CreateSolidBrush(RGB(255, 0, 0));
			//return (BOOL)((HBRUSH)GetStockObject(NULL_BRUSH));
		}
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

int main()
{
	initgraph();

	HiEasyX::SetWndProcFunc(nullptr, WndProc);
	HiEasyX::init_end();

	closegraph();
	return 0;
}
