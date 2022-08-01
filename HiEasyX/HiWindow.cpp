#include "HiWindow.h"

#include "HiStart.h"
#include "HiCanvas.h"


// 预留消息空间
#define MSG_RESERVE_SIZE 100

namespace HiEasyX
{
	////////////****** 全局变量 ******////////////


	WNDCLASSEX				g_WndClassEx;								// 窗口类
	wchar_t					g_lpszClassName[] = L"HiEasyX";				// 窗口类名
	int						g_nSysW = 0, g_nSysH = 0;					// 系统分辨率
	HWND					g_hConsole;									// 控制台句柄
	HINSTANCE				g_hInstance = GetModuleHandle(0);			// 程序实例

	std::vector<EasyWindow>	g_vecWindows;								// 窗口表（管理多窗口）
	int						g_nFocusWindowIndex = NO_WINDOW_INDEX;		// 当前操作焦点窗口索引

	const int				g_nTypesNum = 4;							// 消息类型数量
	BYTE					g_pMsgTypes[g_nTypesNum] = {				// 消息类型数组
		EM_MOUSE, EM_KEY, EM_CHAR, EM_WINDOW
	};

	bool					g_isInTask = false;							// 标记处于任务中

	HICON					g_hIconDefault;								// 默认程序图标
	int						g_nCustomIcon = 0;							// 自定义程序图标资源 ID，为 0 表示不使用
	int						g_nCustomIconSm = 0;
	HICON					g_hCustomIcon;								// 自定义程序图标
	HICON					g_hCustomIconSm;

	bool					g_isPreStyle = false;						// 是否预设窗口样式
	bool					g_isPrePos = false;							// 是否预设窗口位置
	long					g_lPreStyle;								// 创建窗口前的预设样式
	POINT					g_pPrePos;									// 创建窗口前的预设窗口位置

	UINT					g_uWM_TASKBARCREATED;						// 系统任务栏消息代码

	////////////****** 函数定义 ******////////////

	// 检验窗口索引是否合法
	bool isValidWindowIndex(int index)
	{
		return index >= 0 && index < (int)g_vecWindows.size();
	}

	// 当前是否存在操作焦点窗口（若存在，则一定是活窗口）
	bool isFocusWindowExisted()
	{
		return isValidWindowIndex(g_nFocusWindowIndex);
	}

	// 获取当前操作焦点窗口
	EasyWindow& GetFocusWindow()
	{
		static EasyWindow wndEmpty;
		if (isFocusWindowExisted())
		{
			return g_vecWindows[g_nFocusWindowIndex];
		}
		else
		{
			wndEmpty = {};
			return wndEmpty;
		}
	}

	// 通过句柄获得此窗口在窗口记录表中的索引
	// 传入 nullptr 代表当前活动窗口
	// 未找到返回 NO_WINDOW_INDEX
	int GetWindowIndex(HWND hWnd)
	{
		if (hWnd == nullptr)
		{
			return g_nFocusWindowIndex;
		}
		int index = NO_WINDOW_INDEX;
		for (int i = 0; i < (int)g_vecWindows.size(); i++)
		{
			if (hWnd == g_vecWindows[i].hWnd)
			{
				index = i;
				break;
			}
		}
		return index;
	}

	bool isAnyWindow()
	{
		for (EasyWindow& i : g_vecWindows)
			if (i.isAlive)
				return true;
		return false;
	}

	bool isAliveWindow(HWND hWnd)
	{
		if (hWnd)
		{
			int index = GetWindowIndex(hWnd);
			if (isValidWindowIndex(index))
			{
				return g_vecWindows[index].isAlive;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return isFocusWindowExisted();
		}
	}

	bool isAliveWindow(int index)
	{
		return isValidWindowIndex(index) && g_vecWindows[index].isAlive;
	}

	// 等待窗口内部消息处理完成
	void WaitForProcessing(int index)
	{
		// 死窗口可能正在销毁，故不用 isAliveWindow
		if (isValidWindowIndex(index))
		{
			while (g_vecWindows[index].isBusyProcessing)
			{
				HpSleep(1);
			}
		}
	}

	// 将绘制在 EasyX 中的内容显示到目标窗口上
	void FlushDrawingToWnd(IMAGE* pImg, HWND hWnd)
	{
		HDC hdc = GetDC(hWnd);
		HDC hdcImg = GetImageHDC(pImg);
		RECT rctWnd;
		GetClientRect(hWnd, &rctWnd);
		BitBlt(hdc, 0, 0, rctWnd.right, rctWnd.bottom, hdcImg, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdc);
	}

	void WaitForTask(HWND hWnd)
	{
		// 未设置句柄时只需要等待，若设置了则需要判断该句柄是否对应活动窗口
		if (!hWnd || (isFocusWindowExisted() && GetFocusWindow().hWnd == hWnd))
		{
			while (g_isInTask)
			{
				HpSleep(1);
			}
		}
	}

	// 销毁窗口，释放内存
	void DelWindow(int index)
	{
		if (!isValidWindowIndex(index))
		{
			return;
		}

		// 释放绘图缓冲
		if (g_vecWindows[index].pImg)
		{
			delete g_vecWindows[index].pImg;
			g_vecWindows[index].pImg = nullptr;
		}
		if (g_vecWindows[index].pBufferImg)
		{
			delete g_vecWindows[index].pBufferImg;
			g_vecWindows[index].pBufferImg = nullptr;
		}

		// 释放消息列表内存
		std::vector<ExMessage>().swap(g_vecWindows[index].vecMessage);

		DestroyWindow(g_vecWindows[index].hWnd);
		PostQuitMessage(0);
	}

	// 此函数用于内部调用，按窗口索引关闭窗口
	void closegraph_win32(int index)
	{
		if (!isAliveWindow(index))
		{
			return;
		}

		// 等待任务结束
		g_vecWindows[index].isBusyProcessing = true;
		WaitForTask(g_vecWindows[index].hWnd);

		g_vecWindows[index].isAlive = false;

		// 若已设置父窗口为模态窗口，则需要将父窗口恢复正常
		if (g_vecWindows[index].hParent != nullptr)
		{
			EnableWindow(g_vecWindows[index].hParent, true);
			SetForegroundWindow(g_vecWindows[index].hParent);
		}

		// 卸载托盘
		DeleteTray(g_vecWindows[index].hWnd);

		// 如果活动窗口被销毁，则需要重置活动窗口索引
		if (index == g_nFocusWindowIndex)
		{
			g_nFocusWindowIndex = NO_WINDOW_INDEX;
		}

		// 销毁窗口
		DelWindow(index);
		g_vecWindows[index].isBusyProcessing = false;
	}

	// 此函数用于外部调用，只是向目标窗口线程发送关闭窗口消息
	void closegraph_win32(HWND hWnd)
	{
		// 关闭全部
		if (hWnd == nullptr)
		{
			for (int i = 0; i < (int)g_vecWindows.size(); i++)
			{
				if (g_vecWindows[i].isAlive)
				{
					// 必须交由原线程销毁窗口，才能使窗口销毁
					// 发送 WM_DESTROY 时特殊标记 wParam 为 1，表示程序命令销毁窗口
					SendMessage(g_vecWindows[i].hWnd, WM_DESTROY, 1, 0);
				}
			}
		}
		else if (isAliveWindow(hWnd))
		{
			SendMessage(hWnd, WM_DESTROY, 1, 0);
		}
	}

	IMAGE* GetWindowImage(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			return g_vecWindows[index].pBufferImg;
		}
		else
		{
			return nullptr;
		}
	}

	Canvas* GetWindowCanvas(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			return g_vecWindows[index].pBufferImgCanvas;
		}
		else
		{
			return nullptr;
		}
	}

	void BindWindowCanvas(Canvas* pCanvas, HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			g_vecWindows[index].pBufferImgCanvas = pCanvas;
			pCanvas->BindToWindow(g_vecWindows[index].hWnd, g_vecWindows[index].pBufferImg);
		}
	}

	void init_end()
	{
		// 阻塞，直到所有窗口都被关闭
		while (isAnyWindow())
		{
			Sleep(100);
		}
	}

	void AutoExit()
	{
		std::thread([]() {
			init_end();
			exit(0);
			}).detach();
	}

	HWND GetHWnd_win32()
	{
		return isFocusWindowExisted() ? GetFocusWindow().hWnd : nullptr;
	}

	EasyWindow GetWorkingWindow()
	{
		return GetFocusWindow();
	}

	bool SetWorkingWindow(HWND hWnd)
	{
		if (GetFocusWindow().hWnd == hWnd)
		{
			if (GetWorkingImage() != GetFocusWindow().pBufferImg)
			{
				SetWorkingImage(GetFocusWindow().pBufferImg);
			}
			return true;
		}

		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			WaitForTask();
			WaitForProcessing(index);
			g_nFocusWindowIndex = index;

			SetWorkingImage(GetFocusWindow().pBufferImg);
			return true;
		}
		else
		{
			return false;
		}
	}

	void QuickDraw(UINT nSkipPixels, HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
			g_vecWindows[index].nSkipPixels = nSkipPixels;
	}

	void EnforceRedraw(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
			InvalidateRect(g_vecWindows[index].hWnd, nullptr, false);
	}

	// 复制缓冲区
	void FlushDrawing(int index)
	{
		if (isAliveWindow(index))
		{
			int w = g_vecWindows[index].pImg->getwidth();
			int h = g_vecWindows[index].pImg->getheight();

			if (g_vecWindows[index].nSkipPixels == 0)
			{
				// fastest
				memcpy(
					GetImageBuffer(g_vecWindows[index].pImg),
					GetImageBuffer(g_vecWindows[index].pBufferImg),
					sizeof(DWORD) * w * h
				);
			}
			else
			{
				int len = w * h;
				DWORD* buf[2] = {
					GetImageBuffer(g_vecWindows[index].pImg) ,
					GetImageBuffer(g_vecWindows[index].pBufferImg)
				};
				for (int i = 0; i < len; i++)
				{
					if (buf[0][i] == buf[1][i])
					{
						i += g_vecWindows[index].nSkipPixels;
						continue;
					}
					buf[0][i] = buf[1][i];
					buf[0][i] = buf[1][i];
				}
			}
		}
	}

	bool BeginTask()
	{
		// 不做窗口匹配判断，只检验是否处于任务中
		if (!g_isInTask && isFocusWindowExisted())
		{
			WaitForProcessing(g_nFocusWindowIndex);
			g_isInTask = true;
		}
		return g_isInTask;
	}

	void EndTask(bool flush)
	{
		if (g_isInTask)
		{
			if (flush && isFocusWindowExisted())
			{
				FlushDrawing(g_nFocusWindowIndex);
			}

			g_isInTask = false;
		}
	}

	bool isInTask(HWND hWnd)
	{
		return g_isInTask && (hWnd ? GetFocusWindow().hWnd == hWnd : true);
	}

	// 根据窗口大小重新调整该窗口画布大小
	void ResizeWindowImage(int index)
	{
		if (isAliveWindow(index))
		{
			RECT rctWnd;
			for (int i = 0; i < 2; i++)
			{
				if (GetClientRect(g_vecWindows[index].hWnd, &rctWnd))	// 客户区矩形
				{
					g_vecWindows[index].pImg->Resize(rctWnd.right, rctWnd.bottom);
					g_vecWindows[index].pBufferImg->Resize(rctWnd.right, rctWnd.bottom);
					g_vecWindows[index].isNewSize = true;
					break;
				}
			}
		}
	}

	void ShowTray(NOTIFYICONDATA* nid)
	{
		Shell_NotifyIcon(NIM_ADD, nid);
	}

	void CreateTray(LPCTSTR lpszTrayName, HWND hWnd)
	{
		static int id = 0;

		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			HICON hIcon = g_hIconDefault;
			if (g_nCustomIconSm)		hIcon = g_hCustomIconSm;
			else if (g_nCustomIcon)		hIcon = g_hCustomIcon;

			g_vecWindows[index].isUseTray = true;
			g_vecWindows[index].nid.cbSize = sizeof(NOTIFYICONDATA);
			g_vecWindows[index].nid.hWnd = g_vecWindows[index].hWnd;
			g_vecWindows[index].nid.uID = id++;
			g_vecWindows[index].nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			g_vecWindows[index].nid.uCallbackMessage = WM_TRAY;
			g_vecWindows[index].nid.hIcon = hIcon;
			lstrcpy(g_vecWindows[index].nid.szTip, lpszTrayName);
			ShowTray(&g_vecWindows[index].nid);
		}
	}

	void DeleteTray(HWND hWnd)
	{
		// 请勿随意修改
		int index;
		if (hWnd == nullptr)
		{
			if (!isFocusWindowExisted())
			{
				return;
			}
			else
			{
				index = g_nFocusWindowIndex;
			}
		}
		else
		{
			index = GetWindowIndex(hWnd);

			// 死窗口删除时会调用该函数，所以不判断窗口死活
			if (!isValidWindowIndex(index))
			{
				return;
			}
		}
		if (g_vecWindows[index].isUseTray)
		{
			g_vecWindows[index].isUseTray = false;
			Shell_NotifyIcon(NIM_DELETE, &g_vecWindows[index].nid);
		}
	}

	void SetTrayMenu(HMENU hMenu, HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			g_vecWindows[index].isUseTrayMenu = true;
			g_vecWindows[index].hTrayMenu = hMenu;
		}
	}

	void SetTrayMenuProcFunc(void(*pFunc)(UINT), HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			g_vecWindows[index].funcTrayMenuProc = pFunc;
		}
	}

	bool isWindowSizeChanged(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (isValidWindowIndex(index))
		{
			bool b = g_vecWindows[index].isNewSize;
			g_vecWindows[index].isNewSize = false;
			return b;
		}
		else
		{
			return false;
		}
	}

	bool GetCustomIconState()
	{
		return g_nCustomIcon;
	}

	void SetCustomIcon(int nIcon, int nIconSm)
	{
		g_nCustomIcon = nIcon;
		g_nCustomIconSm = nIconSm;
		g_hCustomIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(g_nCustomIcon));
		g_hCustomIconSm = LoadIcon(g_hInstance, MAKEINTRESOURCE(g_nCustomIconSm));
	}

	// 获取消息容器
	std::vector<ExMessage>& GetMsgVector(HWND hWnd)
	{
		static std::vector<ExMessage> vec;
		int index = GetWindowIndex(hWnd);
		if (isAliveWindow(index))
		{
			return GetFocusWindow().vecMessage;
		}
		else
		{
			vec.clear();
			return vec;
		}
	}

	// 移除当前消息
	void RemoveMessage(HWND hWnd)
	{
		GetMsgVector(hWnd).erase(GetMsgVector(hWnd).begin());
	}

	// 清空消息
	// 支持混合消息类型
	void ClearMessage(BYTE filter, HWND hWnd)
	{
		for (size_t i = 0; i < GetMsgVector(hWnd).size(); i++)
			if (filter & GetExMessageType(GetMsgVector(hWnd)[i]))
				GetMsgVector(hWnd).erase(GetMsgVector(hWnd).begin() + i--);
	}

	// 是否有新消息
	// 支持混合消息类型
	bool isNewMessage(BYTE filter, HWND hWnd)
	{
		for (auto& element : GetMsgVector(hWnd))
			if (filter & GetExMessageType(element))
				return true;
		return false;
	}

	// 清除消息，直至获取到符合类型的消息
	// 支持混合消息类型
	ExMessage GetNextMessage(BYTE filter, HWND hWnd)
	{
		if (isNewMessage(filter, hWnd))
		{
			for (size_t i = 0; i < GetMsgVector(hWnd).size(); i++)
			{
				if (filter & GetExMessageType(GetMsgVector(hWnd)[i]))
				{
					for (size_t j = 0; j < i; j++)
					{
						RemoveMessage(hWnd);
					}
					return GetMsgVector(hWnd)[0];
				}
			}
		}
		return {};
	}

	ExMessage getmessage_win32(BYTE filter, HWND hWnd)
	{
		while (!isNewMessage(filter, hWnd))	HpSleep(1);
		ExMessage msg = GetNextMessage(filter, hWnd);
		RemoveMessage(hWnd);
		return msg;
	}

	void getmessage_win32(ExMessage* msg, BYTE filter, HWND hWnd)
	{
		ExMessage msgEx = getmessage_win32(filter);
		if (msg)	*msg = msgEx;
	}

	bool peekmessage_win32(ExMessage* msg, BYTE filter, bool removemsg, HWND hWnd)
	{
		if (isNewMessage(filter, hWnd))
		{
			if (msg)		*msg = GetNextMessage(filter, hWnd);
			if (removemsg)	RemoveMessage(hWnd);
			return true;
		}
		return false;
	}

	void flushmessage_win32(BYTE filter, HWND hWnd)
	{
		ClearMessage(filter, hWnd);
	}

	bool MouseHit_win32(HWND hWnd)
	{
		return isNewMessage(EM_MOUSE, hWnd);
	}

	MOUSEMSG GetMouseMsg_win32(HWND hWnd)
	{
		ExMessage msgEx = GetNextMessage(EM_MOUSE, hWnd);
		return To_MouseMsg(msgEx);
	}

	bool PeekMouseMsg_win32(MOUSEMSG* pMsg, bool bRemoveMsg, HWND hWnd)
	{
		ExMessage msgEx;
		bool r = peekmessage_win32(&msgEx, EM_MOUSE, bRemoveMsg, hWnd);
		*pMsg = To_MouseMsg(msgEx);
		return r;
	}

	void FlushMouseMsgBuffer_win32(HWND hWnd)
	{
		ClearMessage(EM_MOUSE, hWnd);
	}

	ExMessage To_ExMessage(MOUSEMSG msg)
	{
		ExMessage msgEx = {};
		msgEx.message = msg.uMsg;
		msgEx.ctrl = msg.mkCtrl;
		msgEx.shift = msg.mkShift;
		msgEx.lbutton = msg.mkLButton;
		msgEx.mbutton = msg.mkMButton;
		msgEx.rbutton = msg.mkRButton;
		msgEx.x = msg.x;
		msgEx.y = msg.y;
		msgEx.wheel = msg.wheel;
		return msgEx;
	}

	MOUSEMSG To_MouseMsg(ExMessage msgEx)
	{
		MOUSEMSG msg = {};
		msg.uMsg = msgEx.message;
		msg.mkCtrl = msgEx.ctrl;
		msg.mkShift = msgEx.shift;
		msg.mkLButton = msgEx.lbutton;
		msg.mkMButton = msgEx.mbutton;
		msg.mkRButton = msgEx.rbutton;
		msg.x = msgEx.x;
		msg.y = msgEx.y;
		msg.wheel = msgEx.wheel;
		return msg;
	}

	IMAGE GetDefaultIconImage()
	{
		IMAGE* old = GetWorkingImage();
		IMAGE img(32, 32);
		SetWorkingImage(&img);

		setbkcolor(RED);
		setbkmode(TRANSPARENT);

		settextcolor(WHITE);
		settextstyle(48, 0, L"Consolas");

		setfillcolor(BLUE);
		setlinecolor(BLUE);

		cleardevice();
		fillcircle(16, 16, 16);
		outtextxy(4, -8, L'X');

		SetWorkingImage(old);
		return img;
	}

	void PreSetWindowStyle(long lStyle)
	{
		g_isPreStyle = true;
		g_lPreStyle = lStyle;
	}

	void PreSetWindowPos(int x, int y)
	{
		g_isPrePos = true;
		g_pPrePos = { x,y };
	}

	int SetWindowStyle(long lNewStyle, HWND hWnd)
	{
		if (hWnd == nullptr)	hWnd = GetFocusWindow().hWnd;
		return SetWindowLong(hWnd, GWL_STYLE, lNewStyle);
	}

	int SetWindowExStyle(long lNewExStyle, HWND hWnd)
	{
		if (hWnd == nullptr)	hWnd = GetFocusWindow().hWnd;
		return SetWindowLong(hWnd, GWL_EXSTYLE, lNewExStyle);
	}

	POINT GetWindowPos(HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		RECT rct;
		GetWindowRect(hWnd, &rct);
		return { rct.left, rct.top };
	}

	SIZE GetWindowSize(HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		RECT rct;
		GetWindowRect(hWnd, &rct);
		return { rct.right - rct.left, rct.bottom - rct.top };
	}

	void MoveWindow(int x, int y, HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}

	void MoveWindowRel(int dx, int dy, HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		POINT pos = GetWindowPos(hWnd);
		SetWindowPos(hWnd, HWND_TOP, pos.x + dx, pos.y + dy, 0, 0, SWP_NOSIZE);
	}

	void ResizeWindow(int w, int h, HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		SetWindowPos(hWnd, HWND_TOP, 0, 0, w, h, SWP_NOMOVE);
	}

	// 获取默认窗口图标
	HICON GetDefaultAppIcon()
	{
		IMAGE img = GetDefaultIconImage();
		HBITMAP hBmp = Image2Bitmap(&img);
		HICON hIcon = Bitmap2Icon(hBmp);
		DeleteObject(hBmp);
		return hIcon;
	}

	// 窗口过程函数
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		RECT rctWnd;							// 窗口矩形信息
		POINT ptMouse;							// 鼠标位置
		bool isNeedDefaultProc = true;			// 记录是否需要使用默认方法处理消息
		int indexWnd = GetWindowIndex(hwnd);	// 该窗口在已记录列表中的索引

		// 出现未知窗口，则使用默认方法进行处理（这是非正常情况）
		// 死窗口也可能调用过程函数
		if (!isValidWindowIndex(indexWnd))
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		GetWindowRect(hwnd, &rctWnd);
		GetCursorPos(&ptMouse);

		//** 开始处理窗口消息 **//

		// 必须预先处理的一些消息
		switch (msg)
		{
		case WM_CREATE:
			break;

		case WM_SIZE:
			WaitForProcessing(indexWnd);
			g_vecWindows[indexWnd].isBusyProcessing = true;		// 不能再启动任务
			WaitForTask(g_vecWindows[indexWnd].hWnd);			// 等待最后一个任务完成

			ResizeWindowImage(indexWnd);
			if (g_vecWindows[indexWnd].pBufferImgCanvas)
			{
				g_vecWindows[indexWnd].pBufferImgCanvas->UpdateSizeInfo();
			}

			g_vecWindows[indexWnd].isBusyProcessing = false;
			break;

			// 托盘消息
		case WM_TRAY:
			if (g_vecWindows[indexWnd].isUseTray)
			{
				switch (lParam)
				{
					// 左键激活窗口
				case WM_LBUTTONDOWN:
					SetForegroundWindow(hwnd);
					break;

					// 右键打开菜单
				case WM_RBUTTONDOWN:
					if (g_vecWindows[indexWnd].isUseTrayMenu)
					{
						SetForegroundWindow(hwnd);	// 激活一下窗口，防止菜单不消失

						// 显示菜单并跟踪
						int nMenuId = TrackPopupMenu(g_vecWindows[indexWnd].hTrayMenu, TPM_RETURNCMD, ptMouse.x, ptMouse.y, 0, hwnd, nullptr);
						if (nMenuId == 0) PostMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
						if (g_vecWindows[indexWnd].funcTrayMenuProc)
						{
							g_vecWindows[indexWnd].funcTrayMenuProc(nMenuId);
						}

					}
					break;

				default:
					break;
				}
			}
			break;

		default:
			// 系统任务栏重新创建，此时可能需要重新创建托盘
			if (msg == g_uWM_TASKBARCREATED)
			{
				if (g_vecWindows[indexWnd].isUseTray)
				{
					ShowTray(&g_vecWindows[indexWnd].nid);
				}
			}
			break;
		}

		// 消息记录
		switch (msg)
		{
			// EM_MOUSE
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		{
			ExMessage msgMouse = {};
			msgMouse.message = msg;
			msgMouse.x = GET_X_LPARAM(lParam);
			msgMouse.y = GET_Y_LPARAM(lParam);
			msgMouse.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
			msgMouse.shift = LOWORD(wParam) & 0x04 ? true : false;
			msgMouse.ctrl = LOWORD(wParam) & 0x08 ? true : false;
			msgMouse.lbutton = LOWORD(wParam) & 0x01 ? true : false;
			msgMouse.mbutton = LOWORD(wParam) & 0x10 ? true : false;
			msgMouse.rbutton = LOWORD(wParam) & 0x02 ? true : false;

			// 有滚轮消息时，得到的坐标是屏幕坐标，需要转换
			if (msgMouse.wheel)
			{
				POINT p = { msgMouse.x ,msgMouse.y };
				ScreenToClient(g_vecWindows[indexWnd].hWnd, &p);
				msgMouse.x = (short)p.x;
				msgMouse.y = (short)p.y;
			}

			g_vecWindows[indexWnd].vecMessage.push_back(msgMouse);
		}
		break;

		// EM_KEY
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			// code from MSDN
			WORD vkCode = LOWORD(wParam);                                 // virtual-key code
			WORD keyFlags = HIWORD(lParam);
			WORD scanCode = LOBYTE(keyFlags);                             // scan code
			BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED; // extended-key flag, 1 if scancode has 0xE0 prefix

			if (isExtendedKey)
				scanCode = MAKEWORD(scanCode, 0xE0);

			BOOL repeatFlag = (keyFlags & KF_REPEAT) == KF_REPEAT;        // previous key-state flag, 1 on autorepeat
			WORD repeatCount = LOWORD(lParam);                            // repeat count, > 0 if several keydown messages was combined into one message
			BOOL upFlag = (keyFlags & KF_UP) == KF_UP;                    // transition-state flag, 1 on keyup

			// 功能键：不区分左右
			// if we want to distinguish these keys:
			//switch (vkCode)
			//{
			//case VK_SHIFT:   // converts to VK_LSHIFT or VK_RSHIFT
			//case VK_CONTROL: // converts to VK_LCONTROL or VK_RCONTROL
			//case VK_MENU:    // converts to VK_LMENU or VK_RMENU
			//	vkCode = LOWORD(MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX));
			//	break;
			//}

			ExMessage msgKey = {};
			msgKey.message = msg;
			msgKey.vkcode = (BYTE)vkCode;
			msgKey.scancode = (BYTE)scanCode;
			msgKey.extended = isExtendedKey;
			msgKey.prevdown = repeatFlag;

			g_vecWindows[indexWnd].vecMessage.push_back(msgKey);

			// 给控制台发一份，支持 _getch() 系列函数
			PostMessage(g_hConsole, msg, wParam, lParam);
		}
		break;

		// EM_CHAR
		case WM_CHAR:
		{
			ExMessage msgChar = {};
			msgChar.message = msg;
			msgChar.ch = (TCHAR)wParam;
			g_vecWindows[indexWnd].vecMessage.push_back(msgChar);

			// 通知控制台
			PostMessage(g_hConsole, msg, wParam, lParam);
		}
		break;

		// EM_WINDOW
		case WM_ACTIVATE:
		case WM_MOVE:
		case WM_SIZE:
		{
			ExMessage msgWindow = {};
			msgWindow.message = msg;
			msgWindow.wParam = wParam;
			msgWindow.lParam = lParam;
			g_vecWindows[indexWnd].vecMessage.push_back(msgWindow);
		}
		break;

		}

		// 若有独立的消息处理函数则调用
		if (g_vecWindows[indexWnd].funcWndProc)
		{
			isNeedDefaultProc = g_vecWindows[indexWnd].funcWndProc(hwnd, msg, wParam, lParam, g_hInstance);
		}

		// 必须在用户处理消息后做的一些后续工作
		switch (msg)
		{
			// 映射绘图缓存到窗口
		case WM_PAINT:
			FlushDrawingToWnd(g_vecWindows[indexWnd].pImg, g_vecWindows[indexWnd].hWnd);
			DefWindowProc(hwnd, msg, wParam, lParam);
			break;

		case WM_MOVE:
			// 移动窗口超出屏幕时可能导致子窗口显示有问题，所以此时需要彻底重绘
			if (rctWnd.left <= 0 || rctWnd.top <= 0 || rctWnd.right >= g_nSysW || rctWnd.bottom >= g_nSysH)
			{
				InvalidateRect(hwnd, nullptr, false);
			}
			break;

		case WM_DESTROY:
			// WM_DESTROY 消息本无参数，若出现参数则是程序命令销毁窗口
			if (wParam)
			{
				closegraph_win32(indexWnd);
			}
			break;
		}

		// 返回值
		LRESULT lResult = 0;

		// 如需使用默认方法继续处理
		// 注意，此过程中不能返回，必须在函数末尾返回
		if (isNeedDefaultProc)
		{
			switch (msg)
			{
				// 此消息用户可以拦截，故默认处理时直接销毁窗口
			case WM_CLOSE:
				closegraph_win32(indexWnd);
				break;

			default:
				lResult = DefWindowProc(hwnd, msg, wParam, lParam);
				break;
			}
		}

		return lResult;
	}

	void RegisterWndClass()
	{
		HICON hIcon = g_hIconDefault;
		HICON hIconSm = g_hIconDefault;
		if (g_nCustomIcon)		hIcon = g_hCustomIcon;
		if (g_nCustomIconSm)	hIconSm = g_hCustomIconSm;

		g_WndClassEx.cbSize = sizeof(WNDCLASSEX);
		g_WndClassEx.style = CS_VREDRAW | CS_HREDRAW;
		g_WndClassEx.lpfnWndProc = WndProc;
		g_WndClassEx.cbClsExtra = 0;
		g_WndClassEx.cbWndExtra = 0;
		g_WndClassEx.hInstance = g_hInstance;
		g_WndClassEx.hIcon = hIcon;
		g_WndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		g_WndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		g_WndClassEx.lpszMenuName = nullptr;
		g_WndClassEx.lpszClassName = g_lpszClassName;
		g_WndClassEx.hIconSm = hIconSm;

		// 注册窗口类
		if (!RegisterClassEx(&g_WndClassEx))
		{
			std::wstring str = std::to_wstring(GetLastError());
			MessageBox(nullptr, (L"Error registing window class: " + str).c_str(), L"[Error]", MB_OK | MB_ICONERROR);
			exit(-1);
		}
	}

	// 真正创建窗口的函数（阻塞）
	void InitWindow(int w, int h, int flag, LPCTSTR lpszWndTitle, bool(*WindowProcess)(HWND, UINT, WPARAM, LPARAM, HINSTANCE), HWND hParent, int* nDoneFlag, HWND* hWnd)
	{
		static int nWndCount = 0;	// 已创建窗口计数（用于生成窗口标题）
		std::wstring wstrTitle;		// 窗口标题
		EasyWindow wnd;				// 窗口信息
		int nFrameW, nFrameH;		// 窗口标题栏宽高（各个窗口可能不同）

		// 未设置标题
		if (lstrlen(lpszWndTitle) == 0)
		{
			wstrTitle = L"EasyX Window";
			if (nWndCount != 0)
			{
				wstrTitle += L" (" + std::to_wstring(nWndCount + 1) + L")";
			}
		}
		else
		{
			wstrTitle = lpszWndTitle;
		}

		// 第一次创建窗口 --- 初始化各项数据
		if (nWndCount == 0)
		{
			// 发布模式下默认渲染开场动画
#ifndef _DEBUG
#ifndef _NO_START_ANIMATION_

			// 渲染开场动画
			RenderStartScene();

#endif // _NO_START_ANIMATION_
#endif // RELEASE

			// 获取分辨率
			g_nSysW = GetSystemMetrics(SM_CXSCREEN);
			g_nSysH = GetSystemMetrics(SM_CYSCREEN);

			// 默认程序图标
			g_hIconDefault = GetDefaultAppIcon();

			// 注册窗口类
			RegisterWndClass();
			g_hConsole = GetConsoleWindow();

			// 隐藏控制台
			if (g_hConsole)
			{
				ShowWindow(g_hConsole, SW_HIDE);
			}

			// 获取系统任务栏自定义的消息代码
			g_uWM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
		}

		// 控制台
		if (g_hConsole && flag & EW_SHOWCONSOLE)
		{
			ShowWindow(g_hConsole, flag & SW_NORMAL);
		}

		int user_style = WS_OVERLAPPEDWINDOW;
		if (flag & EW_NOMINIMIZE)	// 剔除最小化按钮
		{
			user_style &= ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
		}
		// 此方法不行，在下面处理此属性
		/*if (flag & EW_NOCLOSE)
		{
			user_style &= ~WS_SYSMENU;
		}*/
		if (flag & EW_DBLCLKS)		// 支持双击
		{
			user_style |= CS_DBLCLKS;
		}

		// 创建窗口
		for (int i = 0;; i++)
		{
			wnd.hWnd = CreateWindowEx(
				WS_EX_WINDOWEDGE,
				g_lpszClassName,
				wstrTitle.c_str(),
				g_isPreStyle ? g_lPreStyle : user_style,
				CW_USEDEFAULT, CW_USEDEFAULT,
				w, h,	// 宽高暂时这样设置，稍后获取边框大小后再调整
				hParent,
				nullptr,
				g_hInstance,
				nullptr
			);

			// 创建成功，跳出
			if (wnd.hWnd)
				break;

			// 三次创建窗口失败，不再尝试
			else if (i == 2)
			{
				std::wstring str = std::to_wstring(GetLastError());
				MessageBox(nullptr, (L"Error creating window: " + str).c_str(), L"[Error]", MB_OK | MB_ICONERROR);
				*nDoneFlag = -1;
				return;
			}
		}

		// 剔除关闭按钮
		if (flag & EW_NOCLOSE)
		{
			HMENU hmenu = GetSystemMenu(wnd.hWnd, false);
			RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}

		// 初始化窗口属性
		wnd.isAlive = true;
		wnd.hParent = hParent;
		wnd.pImg = new IMAGE(w, h);
		wnd.pBufferImg = new IMAGE(w, h);
		wnd.pBufferImgCanvas = nullptr;
		wnd.funcWndProc = WindowProcess;
		wnd.vecMessage.reserve(MSG_RESERVE_SIZE);
		wnd.isUseTray = false;
		wnd.nid = { 0 };
		wnd.isUseTrayMenu = false;
		wnd.hTrayMenu = nullptr;
		wnd.funcTrayMenuProc = nullptr;
		wnd.isNewSize = false;
		wnd.isBusyProcessing = false;
		wnd.nSkipPixels = 0;

		g_vecWindows.push_back(wnd);

		// 抢夺焦点
		SetWorkingWindow(wnd.hWnd);

		*hWnd = wnd.hWnd;

		// 窗口创建完毕
		nWndCount++;

		// 注意：
		//	必须在显示窗口前标记已经完成创建窗口。
		//	因为可以在自定义过程函数中创建子窗口，若是不在显示窗口前标记窗口创建完成，
		//	就会导致父窗口过程函数阻塞，接下来显示窗口就会阻塞，进而导致整个窗口假死。
		*nDoneFlag = 1;

		//** 显示窗口等后续处理 **//

		// 获取边框大小，补齐绘图区大小
		RECT rcClient, rcWnd;
		GetClientRect(wnd.hWnd, &rcClient);
		GetWindowRect(wnd.hWnd, &rcWnd);
		nFrameW = (rcWnd.right - rcWnd.left) - rcClient.right;
		nFrameH = (rcWnd.bottom - rcWnd.top) - rcClient.bottom;

		int px = 0, py = 0;
		if (g_isPrePos)
		{
			px = g_pPrePos.x;
			py = g_pPrePos.y;
		}
		SetWindowPos(
			wnd.hWnd,
			HWND_TOP,
			px, py,
			w + nFrameW, h + nFrameH,
			g_isPrePos ? 0 : SWP_NOMOVE
		);

		g_isPreStyle = false;
		g_isPrePos = false;

		ShowWindow(wnd.hWnd, SW_SHOWNORMAL);
		UpdateWindow(wnd.hWnd);

		// 由于 WM_CREATE 消息被未知原因吞噬，所以需要模拟发送此消息
		WndProc(wnd.hWnd, WM_CREATE, 0, 0);

		// 消息派发，阻塞
		// 窗口销毁后会自动退出
		MSG Msg;
		while (GetMessage(&Msg, 0, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}

	HWND initgraph_win32(int w, int h, int flag, LPCTSTR lpszWndTitle, bool(*WindowProcess)(HWND, UINT, WPARAM, LPARAM, HINSTANCE), HWND hParent)
	{
		// 标记是否已经完成窗口创建任务
		int nDoneFlag = 0;
		HWND hWnd = NULL;

		// 存在父窗口时，实现模态窗口
		if (hParent)
		{
			// 禁用父窗口（该窗口被销毁后，父窗口将会恢复正常）
			EnableWindow(hParent, false);
		}

		std::thread(InitWindow, w, h, flag, lpszWndTitle, WindowProcess, hParent, &nDoneFlag, &hWnd).detach();

		while (nDoneFlag == 0)	Sleep(10);		// 等待窗口创建完成
		if (nDoneFlag == -1)
		{
			if (hParent)						// 创建子窗口失败，则使父窗口恢复正常
			{
				EnableWindow(hParent, true);
			}
			return nullptr;
		}
		else
		{
			return hWnd;
		}
	}

	Window::Window()
	{
	}

	Window::Window(int w, int h, int flag, LPCTSTR lpszWndTitle, bool(*WindowProcess)(HWND, UINT, WPARAM, LPARAM, HINSTANCE), HWND hParent)
	{
		InitWindow(w, h, flag, lpszWndTitle, WindowProcess, hParent);
	}

	HWND Window::InitWindow(int w, int h, int flag, LPCTSTR lpszWndTitle, bool(*WindowProcess)(HWND, UINT, WPARAM, LPARAM, HINSTANCE), HWND hParent)
	{
		if (!m_isCreated)
		{
			HWND hwnd = initgraph_win32(w, h, flag, lpszWndTitle, WindowProcess, hParent);
			int index = GetWindowIndex(hwnd);
			m_nWindowIndex = index;
			m_isCreated = true;
			return hwnd;
		}
		return nullptr;
	}

	void Window::CloseWindow()
	{
		closegraph_win32(m_nWindowIndex);
	}

	HWND Window::GetHandle()
	{
		return g_vecWindows[m_nWindowIndex].hWnd;
	}

	EasyWindow Window::GetInfo()
	{
		return g_vecWindows[m_nWindowIndex];
	}

	bool Window::isAlive()
	{
		return isAliveWindow(m_nWindowIndex);
	}

	IMAGE* Window::GetImage()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImg;
	}

	Canvas* Window::GetCanvas()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImgCanvas;
	}

	void Window::BindCanvas(Canvas* pCanvas)
	{
		BindWindowCanvas(pCanvas, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::WaitMyTask()
	{
		WaitForTask(g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::SetWorkingWindow()
	{
		return HiEasyX::SetWorkingWindow(g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::BeginTask()
	{
		if (SetWorkingWindow())
		{
			return HiEasyX::BeginTask();
		}
		else
		{
			return false;
		}
	}

	void Window::EndTask(bool flush)
	{
		HiEasyX::EndTask(flush);
	}

	bool Window::isInTask()
	{
		return HiEasyX::isInTask(g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::isSizeChanged()
	{
		return isWindowSizeChanged(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::CreateTray(LPCTSTR lpszTrayName)
	{
		HiEasyX::CreateTray(lpszTrayName, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::DeleteTray()
	{
		HiEasyX::DeleteTray(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::SetTrayMenu(HMENU hMenu)
	{
		HiEasyX::SetTrayMenu(hMenu, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::SetTrayMenuProcFunc(void(*pFunc)(UINT))
	{
		HiEasyX::SetTrayMenuProcFunc(pFunc, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::PreSetStyle(long lStyle)
	{
		m_isPreStyle = true;
		m_lPreStyle = lStyle;
	}

	void Window::PreSetPos(int x, int y)
	{
		m_isPrePos = true;
		m_pPrePos = { x,y };
	}

	void Window::SetQuickDraw(UINT nSkipPixels)
	{
		QuickDraw(nSkipPixels, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::FlushDrawing()
	{
		BeginTask();
		EndTask();
	}

	void Window::Redraw()
	{
		EnforceRedraw(g_vecWindows[m_nWindowIndex].hWnd);
	}

	long Window::GetStyle()
	{
		return GetWindowStyle(g_vecWindows[m_nWindowIndex].hWnd);
	}

	int Window::SetStyle(long lNewStyle)
	{
		return SetWindowStyle(lNewStyle, g_vecWindows[m_nWindowIndex].hWnd);
	}

	long Window::GetExStyle()
	{
		return GetWindowExStyle(g_vecWindows[m_nWindowIndex].hWnd);
	}

	int Window::SetExStyle(long lNewExStyle)
	{
		return SetWindowExStyle(lNewExStyle, g_vecWindows[m_nWindowIndex].hWnd);
	}

	POINT Window::GetPos()
	{
		return GetWindowPos(g_vecWindows[m_nWindowIndex].hWnd);
	}

	SIZE Window::GetWindowSize()
	{
		return HiEasyX::GetWindowSize(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Move(int x, int y)
	{
		MoveWindow(x, y, g_vecWindows[m_nWindowIndex].hWnd);
	}

	int Window::GetWindowWidth()
	{
		return GetWindowSize().cx;
	}

	int Window::GetWindowHeight()
	{
		return  GetWindowSize().cy;
	}

	void Window::MoveRel(int dx, int dy)
	{
		MoveWindowRel(dx, dy, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Resize(int w, int h)
	{
		ResizeWindow(w, h, g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::isForegroundWindow()
	{
		return GetForegroundWindow() == g_vecWindows[m_nWindowIndex].hWnd;
	}

	int Window::GetClientWidth()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImg->getwidth();
	}

	int Window::GetClientHeight()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImg->getheight();
	}

	ExMessage Window::Get_Message(BYTE filter)
	{
		return getmessage_win32(filter, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Get_Message(ExMessage* msg, BYTE filter)
	{
		return getmessage_win32(msg, filter, g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::Peek_Message(ExMessage* msg, BYTE filter, bool removemsg)
	{
		return peekmessage_win32(msg, filter, removemsg, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Flush_Message(BYTE filter)
	{
		flushmessage_win32(filter, g_vecWindows[m_nWindowIndex].hWnd);
	}
}

