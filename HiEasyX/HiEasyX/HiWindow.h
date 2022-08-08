////////////////////////////////////
//
//	HiWindow.h
//	HiEasyX 库的窗口模块
//

#pragma once

#include "HiFunc.h"
#include <graphics.h>
#include <windowsx.h>
#include <vector>
#include <thread>


#define __HIWINDOW_H__

// 补充绘图窗口初始化参数
// 普通窗口
#define EW_NORMAL							0

// 托盘消息
#define WM_TRAY								(WM_USER + 100)

// 无窗口时的索引
#define NO_WINDOW_INDEX						-1

// 窗口过程函数默认返回值
#define HIWINDOW_DEFAULT_PROC				(LRESULT)(-10086)

// 系统控件创建消息
// lParam 传入 CREATESTRUCT*
#define WM_SYSCTRL_CREATE					(WM_USER + 101)

namespace HiEasyX
{
	class Canvas;
	class SysControlBase;

	////////////****** 类型定义 ******////////////

	//	Easy Window 窗口过程函数
	//
	//	窗口过程函数规范
	// 
	//	函数签名：
	//		LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 
	//	注意事项：
	//		若要以默认方式处理消息，则返回 HIWINDOW_DEFAULT_PROC 即可（不要使用 DefWindowProc 函数）
	/*
		// 示例函数
		LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_PAINT:
				BEGIN_TASK_WND(hWnd);
				circle(100, 100, 70);
				END_TASK();
				break;

			case WM_CLOSE:
				DestroyWindow(hWnd);
				break;

			case WM_DESTROY:
				// TODO: 在此处释放申请的内存
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
	*/

	// 窗口
	struct EasyWindow
	{
		bool isAlive;								// 窗口是否存在

		HWND hWnd;									// 窗口句柄
		HWND hParent;								// 父窗口句柄

		IMAGE* pImg;								// 窗口图像
		IMAGE* pBufferImg;							// 图像缓冲区
		Canvas* pBufferImgCanvas;					// 图像缓冲区绑定的画布指针

		WNDPROC funcWndProc;						// 窗口消息处理函数

		std::vector<ExMessage> vecMessage;			// 模拟 EasyX 窗口消息队列

		bool isUseTray;								// 是否使用托盘
		NOTIFYICONDATA nid;							// 托盘信息
		bool isUseTrayMenu;							// 是否使用托盘菜单
		HMENU hTrayMenu;							// 托盘菜单

		void(*funcTrayMenuProc)(UINT);				// 托盘菜单消息处理函数
													// 给出此函数是为了方便响应托盘的菜单消息
													// 如需响应完整的托盘消息，请自定义窗口过程函数并处理 WM_TRAY 消息

		bool isNewSize;								// 窗口大小是否改变
		bool isBusyProcessing;						// 是否正忙于处理内部消息（指不允许用户启动任务的情况）

		int nSkipPixels;							// 绘制时跳过的像素点数量（降质性速绘）

		std::vector<SysControlBase*> vecSysCtrl;	// 记录创建的系统控件
	};

	// 窗口类
	class Window
	{
	private:

		int m_nWindowIndex = NO_WINDOW_INDEX;
		bool m_isCreated = false;
		bool m_isPreStyle = false;
		bool m_isPrePos = false;
		long m_lPreStyle;
		POINT m_pPrePos;

	public:

		Window();

		Window(
			int w,
			int h,
			int flag = EW_NORMAL,
			LPCTSTR lpszWndTitle = L"",
			WNDPROC WindowProcess = nullptr,
			HWND hParent = nullptr
		);

		HWND InitWindow(
			int w = 640,
			int h = 480,
			int flag = EW_NORMAL,
			LPCTSTR lpszWndTitle = L"",
			WNDPROC WindowProcess = nullptr,
			HWND hParent = nullptr
		);

		void CloseWindow();

		void SetProcFunc(WNDPROC WindowProcess);

		HWND GetHandle();

		EasyWindow GetInfo();

		bool isAlive();

		IMAGE* GetImage();

		Canvas* GetCanvas();

		void BindCanvas(Canvas* pCanvas);

		void WaitMyTask();

		bool SetWorkingWindow();

		void SetQuickDraw(UINT nSkipPixels);

		// 输出绘图缓冲（并非重绘窗口）
		void FlushDrawing();

		// 重绘窗口
		void Redraw();

		bool BeginTask();

		void EndTask(bool flush = true);

		bool isInTask();

		bool isSizeChanged();

		void CreateTray(LPCTSTR lpszTrayName);

		void DeleteTray();

		void SetTrayMenu(HMENU hMenu);

		void SetTrayMenuProcFunc(void(*pFunc)(UINT));

		void PreSetStyle(long lStyle);

		void PreSetPos(int x, int y);

		long GetStyle();

		int SetStyle(long lNewStyle);

		long GetExStyle();

		int	SetExStyle(long lNewExStyle);

		POINT GetPos();

		SIZE GetWindowSize();

		int GetWindowWidth();

		int GetWindowHeight();

		int GetClientWidth();

		int GetClientHeight();

		void Move(int x, int y);

		void MoveRel(int dx, int dy);

		void Resize(int w, int h);

		// 是否为用户正在使用的窗口
		bool isForegroundWindow();

		ExMessage Get_Message(BYTE filter = -1);

		void Get_Message(ExMessage* msg, BYTE filter = -1);

		bool Peek_Message(ExMessage* msg, BYTE filter = -1, bool removemsg = true);

		void Flush_Message(BYTE filter = -1);
	};

	////////////****** 窗体相关函数 ******////////////

	// 创建 Win32 绘图窗口（默认支持窗口双击消息）（跳过了 EasyX 原生窗口创建的步骤）
	// w, h				窗口大小
	// flag				窗口样式标识
	// lpszWndTitle		窗口标题
	// WindowProcess	窗口消息处理函数的指针，为空表示使用默认消息处理函数（详见头文件中“窗口消息处理函数规范”）
	// hParent			父窗口句柄，为空则表示该窗口独立存在（若填写，该窗口将成为模态窗口）
	//
	// 注：若创建大量绘图窗口，请适量添加延时，否则会导致一些问题。
	HWND initgraph_win32(
		int w = 640,
		int h = 480,
		int flag = EW_NORMAL,
		LPCTSTR lpszWndTitle = L"",
		WNDPROC WindowProcess = nullptr,
		HWND hParent = nullptr
	);

	// 关闭某一绘图窗口，若句柄为 nullptr 则关闭所有绘图窗口
	void closegraph_win32(HWND hWnd = nullptr);

	// 设置某窗口的过程函数（传入空句柄可以标识当前活动窗口）
	void SetWndProcFunc(HWND hWnd, WNDPROC WindowProcess);

	// 得到当前绘图窗口的句柄
	HWND GetHWnd_win32();

	// 结束初始化窗口（阻塞）
	void init_end();

	// 设置：当窗口都被销毁时，自动退出程序
	void AutoExit();

	// 是否还存在未销毁的绘图窗口
	bool isAnyWindow();

	// 判断一窗口是否还存在（不含已被关闭的窗口）
	// 传入空句柄可以标识当前活动窗口
	bool isAliveWindow(HWND hWnd = nullptr);

	// 获取某窗口的图像指针
	IMAGE* GetWindowImage(HWND hWnd = nullptr);

	// 获取窗口画布指针
	Canvas* GetWindowCanvas(HWND hWnd = nullptr);

	// 绑定窗口画布指针
	// 绑定后，使用画布绘图时将自动开启任务，无需用户开启，但不会自动刷新屏幕
	// 绑定画布后，如在外部操作画布，则必须先启动窗口任务
	void BindWindowCanvas(Canvas* pCanvas, HWND hWnd = nullptr);

	// 将绘制在 EasyX 中的内容显示到目标窗口上（任意窗口）
	void FlushDrawingToWnd(IMAGE* pImg, HWND hWnd);

	// 阻塞等待绘图任务完成
	// 如果传入句柄，则只有该句柄窗口是活动窗口时才等待
	void WaitForTask(HWND hWnd = nullptr);

	// 得到当前绘图窗口的详细信息
	EasyWindow GetWorkingWindow();

	// 等待当前任务完成并设置活动窗口，返回是否设置成功
	bool SetWorkingWindow(HWND hWnd);

	// 设置加速绘制跳过多少像素点
	// 此加速效果是有损的，加速效果与跳过的像素点数正相关。
	void QuickDraw(UINT nSkipPixels, HWND hWnd = nullptr);

	// 强制重绘绘图窗口（在 WM_PAINT 消息内绘图不需要使用此函数）
	void EnforceRedraw(HWND hWnd = nullptr);

	// 为当前活动窗口启动任务，返回是否启动成功（若已在任务中也返回 true）
	// 调用 EasyX 函数进行绘图或获取消息时，都应该先启动任务再进行调用。
	bool BeginTask();

	// 终止当前任务，（可选）输出绘图缓冲
	void EndTask(bool flush = true);

	// 判断当前是否有任务在进行
	// 若传入句柄，则额外判断它是否为活动窗口
	bool isInTask(HWND hWnd = nullptr);

	// 判断某窗口的大小是否改变
	// 传入空句柄可以标识当前活动窗口
	bool isWindowSizeChanged(HWND hWnd = nullptr);

	// 为窗口创建一个托盘
	// 传入托盘提示文本
	// 注意：每个窗口仅能稳定占有一个托盘
	void CreateTray(LPCTSTR lpszTrayName, HWND hWnd = nullptr);

	// 删除某窗口的托盘，传入空指针可以标识当前活动窗口
	void DeleteTray(HWND hWnd = nullptr);

	// 设置托盘菜单（允许在任何时候设置）
	void SetTrayMenu(HMENU hMenu, HWND hWnd = nullptr);

	// 设置托盘菜单消息处理函数
	void SetTrayMenuProcFunc(void(*pFunc)(UINT), HWND hWnd = nullptr);

	// 判断自定义程序图标的启用状态
	bool GetCustomIconState();

	// 使用图标资源作为程序图标
	// 参数传入图标资源 ID（大图标和小图标）
	// 注：必须在第一次创建窗口前就调用该函数才能生效。默认情况下，程序将自绘 EasyX 程序图标
	void SetCustomIcon(int nIcon, int nIconSm);

	// 获取 HiWindow 自绘默认窗口图标的 IMAGE
	IMAGE GetDefaultIconImage();

	// 在创建窗口前设置窗口样式，仅对此操作后首个新窗口生效
	// 注意：新窗口的所有样式都将被当前样式覆盖
	void PreSetWindowStyle(long lStyle);

	// 在创建窗口前设置窗口位置，仅对此操作后首个新窗口生效
	void PreSetWindowPos(int x, int y);

	// 设置当前窗口样式
	int SetWindowStyle(long lNewStyle, HWND hWnd = nullptr);

	// 设置当前窗口扩展样式
	int SetWindowExStyle(long lNewExStyle, HWND hWnd = nullptr);

	// 获取窗口位置
	POINT GetWindowPos(HWND hWnd = nullptr);

	// 获取窗口大小
	SIZE GetWindowSize(HWND hWnd = nullptr);

	// 移动窗口
	void MoveWindow(int x, int y, HWND hWnd = nullptr);

	// 相对移动窗口
	void MoveWindowRel(int dx, int dy, HWND hWnd = nullptr);

	// 重设窗口大小
	void ResizeWindow(int w, int h, HWND hWnd = nullptr);


	////////////****** 消息相关函数 ******////////////

	//// ExMessage 式函数

	// 阻塞等待，直到获取到一个新消息
	ExMessage getmessage_win32(BYTE filter = -1, HWND hWnd = nullptr);

	// 阻塞等待，直到获取到一个新消息
	void getmessage_win32(ExMessage* msg, BYTE filter = -1, HWND hWnd = nullptr);

	// 获取一个消息，立即返回是否获取成功
	bool peekmessage_win32(ExMessage* msg, BYTE filter = -1, bool removemsg = true, HWND hWnd = nullptr);

	// 清除所有消息记录
	void flushmessage_win32(BYTE filter = -1, HWND hWnd = nullptr);

	//// MOUSEMSG 式函数（兼容）

	// 检查是否存在鼠标消息
	bool MouseHit_win32(HWND hWnd = nullptr);

	// 阻塞等待，直到获取到一个新的鼠标消息
	MOUSEMSG GetMouseMsg_win32(HWND hWnd = nullptr);

	// 获取一个新的鼠标消息，立即返回是否获取成功
	bool PeekMouseMsg_win32(MOUSEMSG* pMsg, bool bRemoveMsg = true, HWND hWnd = nullptr);

	// 清空鼠标消息
	void FlushMouseMsgBuffer_win32(HWND hWnd = nullptr);

	//// 转换

	// MOUSEMSG 转 ExMessage
	ExMessage To_ExMessage(MOUSEMSG msg);

	// ExMessage 转 MOUSEMSG
	MOUSEMSG To_MouseMsg(ExMessage msgEx);
}

////////////****** 任务指令宏定义 ******////////////

// 启动一段（绘图）任务（绘制到当前绘图窗口）
#define BEGIN_TASK()\
	if (HiEasyX::SetWorkingWindow(nullptr))\
	{\
		if (HiEasyX::BeginTask())\
		{(0)	/* 此处强制要求加分号 */

// 启动一段（绘图）任务（指定目标绘图窗口）
#define BEGIN_TASK_WND(hWnd)\
	/* 设置工作窗口时将自动等待任务 */\
	if (HiEasyX::SetWorkingWindow(hWnd))\
	{\
		if (HiEasyX::BeginTask())\
		{(0)

// 结束一段（绘图）任务，并输出绘图缓存（须与 BEGIN_TASK 连用）
#define END_TASK()\
			HiEasyX::EndTask();\
		}\
	}(0)

// 要求窗口重绘
#define FLUSH_DRAW()			HiEasyX::EnforceRedraw()

////////////****** 窗口样式宏定义 ******////////////

// 是否禁用当前窗口改变大小
#define DisableResizing(state)			(state ? HiEasyX::SetWindowStyle(GetWindowStyle(HiEasyX::GetHWnd_win32()) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX) :\
										HiEasyX::SetWindowStyle(GetWindowStyle(HiEasyX::GetHWnd_win32()) | WS_SIZEBOX | WS_MAXIMIZEBOX))

// 是否禁用当前窗口的系统菜单
#define DisableSystemMenu(state)		(state ? HiEasyX::SetWindowStyle(GetWindowStyle(HiEasyX::GetHWnd_win32()) & ~WS_SYSMENU) :\
										HiEasyX::SetWindowStyle(GetWindowStyle(HiEasyX::GetHWnd_win32()) | WS_SYSMENU))

// 开启 / 关闭当前窗口的工具栏样式
#define EnableToolWindowStyle(state)	(state ? HiEasyX::SetWindowExStyle(GetWindowExStyle(HiEasyX::GetHWnd_win32()) | WS_EX_TOOLWINDOW) :\
										HiEasyX::SetWindowExStyle(GetWindowExStyle(HiEasyX::GetHWnd_win32()) & ~WS_EX_TOOLWINDOW))

////////////****** 键盘消息宏定义 ******////////////

// 判断全局按键状态
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

// 判断当前活动窗口是否接受到某按键消息
#define KEY_DOWN_WND(VK_NONAME) (GetForegroundWindow() == HiEasyX::GetHWnd_win32() && KEY_DOWN(VK_NONAME))

////////////****** EasyX 原生函数的宏替换 ******////////////

// 若使用 EasyX 原生函数创建窗口，则关闭窗口时自动退出程序
#define initgraph(...)			HiEasyX::initgraph_win32(__VA_ARGS__);\
								HiEasyX::AutoExit()

#define closegraph				HiEasyX::closegraph_win32

// 默认使用双缓冲，故 BeginBatchDraw 无意义
#define BeginBatchDraw()
#define FlushBatchDraw()		FLUSH_DRAW()
#define EndBatchDraw()			FLUSH_DRAW()

#define GetHWnd					HiEasyX::GetHWnd_win32

#define getmessage				HiEasyX::getmessage_win32
#define peekmessage				HiEasyX::peekmessage_win32
#define flushmessage			HiEasyX::flushmessage_win32

#define MouseHit				HiEasyX::MouseHit_win32
#define GetMouseMsg				HiEasyX::GetMouseMsg_win32
#define PeekMouseMsg			HiEasyX::PeekMouseMsg_win32
#define FlushMouseMsgBuffer		HiEasyX::FlushMouseMsgBuffer_win32
