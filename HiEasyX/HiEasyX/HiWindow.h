/**
 * @file	HiWindow.h
 * @brief	HiEasyX 库的窗口模块
 * @author	huidong
*/

#pragma once

#include "HiDef.h"
#include "HiFunc.h"
#include <graphics.h>
#include <windowsx.h>
#include <vector>
#include <thread>


#define __HIWINDOW_H__

// 补充绘图窗口初始化参数
// 普通窗口
#define EW_NORMAL							0

// 无窗口时的索引
#define NO_WINDOW_INDEX						-1

// 窗口过程函数默认返回值
#define HIWINDOW_DEFAULT_PROC				(LRESULT)(-10086)

// 托盘消息
#define WM_TRAY								(WM_USER + 9337)

// 系统控件创建消息
// wParam 传入 SysControlBase*
// lParam 传入 CREATESTRUCT*
#define WM_SYSCTRL_CREATE					(WM_USER + 9338)

// 系统控件析构消息
// wParam 传入 SysControlBase*
#define WM_SYSCTRL_DELETE					(WM_USER + 9339)

// 用户重绘消息，无需参数
// 在快速绘制模式下会发送此消息而非 WN_PAINT
#define WM_USER_REDRAW						(WM_USER + 9340)


namespace HiEasyX
{
	class Canvas;
	class SysControlBase;

	////////////****** 类型定义 ******////////////

	/**
	 * @brief 窗口
	*/
	struct EasyWindow
	{
		bool isAlive;								///< 窗口是否存在

		HWND hWnd;									///< 窗口句柄
		HWND hParent;								///< 父窗口句柄

		IMAGE* pImg;								///< 窗口图像
		IMAGE* pBufferImg;							///< 图像缓冲区
		Canvas* pBufferImgCanvas;					///< 图像缓冲区绑定的画布指针

		WNDPROC funcWndProc;						///< 窗口消息处理函数

		std::vector<ExMessage> vecMessage;			///< 模拟 EasyX 窗口消息队列

		bool isUseTray;								///< 是否使用托盘
		NOTIFYICONDATA nid;							///< 托盘信息
		bool isUseTrayMenu;							///< 是否使用托盘菜单
		HMENU hTrayMenu;							///< 托盘菜单

		/**
		 * @brief 托盘菜单消息处理函数指针
		 * @note
		 *		给出此函数是为了方便响应托盘的菜单消息 <p>
		 *		如需响应完整的托盘消息，请自定义窗口过程函数并处理 WM_TRAY 消息 <p>
		*/
		void(*funcTrayMenuProc)(UINT);

		bool isNewSize;								///< 窗口大小是否改变
		bool isBusyProcessing;						///< 是否正忙于处理内部消息（指不允许用户启动任务的情况）

		int nSkipPixels;							///< 绘制时跳过的像素点数量（降质性速绘）

		std::vector<SysControlBase*> vecSysCtrl;	///< 记录创建的系统控件
		bool bHasCtrl = false;						///< 是否创建过系统控件
	};

	/**
	 * @brief 绘制模式
	*/
	enum DrawMode
	{
		DM_Real,		///< 完全按实际绘制（每次要求重绘都立即执行，可能导致程序卡顿）
		DM_Normal,		///< 正常绘制（默认，发送 WM_PAINT 消息）
		DM_Fast,		///< 快速绘制（发送 WM_USER_REDRAW 消息，可能跳过部分绘制）
		DM_VeryFast,	///< 极速绘制（发送 WM_USER_REDRAW 消息，可能跳过很多绘制）
		DM_Fastest,		///< 最快的绘制方式（发送 WM_USER_REDRAW 消息，可能跳过大部分绘制）
	};

	/**
	 * @brief 窗口
	*/
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

		virtual ~Window();

		HWND InitWindow(
			int w = 640,
			int h = 480,
			int flag = EW_NORMAL,
			LPCTSTR lpszWndTitle = L"",
			WNDPROC WindowProcess = nullptr,
			HWND hParent = nullptr
		);

		/**
		 * @brief 等价于 InitWindow
		*/
		HWND Create(
			int w = 640,
			int h = 480,
			int flag = EW_NORMAL,
			LPCTSTR lpszWndTitle = L"",
			WNDPROC WindowProcess = nullptr,
			HWND hParent = nullptr
		);

		void CloseWindow();

		/**
		 * @brief 等价于 CloseWindow
		*/
		void Destroy();

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

		/**
		 * @brief 输出绘图缓冲（并非重绘窗口）
		*/
		void FlushDrawing();

		/**
		 * @brief 重绘窗口
		*/
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

		/**
		 * @brief 获取整个窗口的大小
		*/
		SIZE GetWindowSize();

		/**
		 * @brief 获取整个窗口的宽度
		*/
		int GetWindowWidth();

		/**
		 * @brief 获取整个窗口的高度
		*/
		int GetWindowHeight();

		/**
		 * @brief 获取客户区宽度
		*/
		int GetClientWidth();

		/**
		 * @brief 获取客户区高度
		*/
		int GetClientHeight();

		void Move(int x, int y);

		void MoveRel(int dx, int dy);

		void Resize(int w, int h);

		/**
		 * @brief 判断此窗口是否为用户正在使用的窗口
		*/
		bool isForegroundWindow();

		ExMessage Get_Message(BYTE filter = -1);

		void Get_Message(ExMessage* msg, BYTE filter = -1);

		bool Peek_Message(ExMessage* msg, BYTE filter = -1, bool removemsg = true);

		void Flush_Message(BYTE filter = -1);
	};

	////////////****** 窗体相关函数 ******////////////

	/**
	 * @brief 创建 Win32 绘图窗口
	 * @param [in] w					窗口宽
	 * @param [in] h					窗口高
	 * @param [in] flag					窗口样式（EW_ 系列宏，默认为 EW_NORMAL）
	 * @param [in] lpszWndTitle			窗口标题
	 * @param [in] WindowProcess		窗口过程函数
	 * @param [in] hParent				父窗口句柄
	 * @return 创建的窗口句柄
	 *
	 * @note
	 *		调用此函数将跳过 EasyX 原生窗口创建的步骤
	 *		窗口默认支持双击消息
	 *
	 * @bug
	 *		不建议大批量创建绘图窗口，如果必要，请适当添加延时，否则可能导致未知问题。
	 *
	 * @par 窗口过程函数规范
	 *
	 *		函数签名：
	 *			LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	 *
	 *		注意事项：
	 *			若要以默认方式处理消息，则返回 HIWINDOW_DEFAULT_PROC 即可（不要使用 DefWindowProc 函数）
	 *
	 *		示例函数：
	 * @code
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
	 * @endcode
	*/
	HWND initgraph_win32(
		int w = 640,
		int h = 480,
		int flag = EW_NORMAL,
		LPCTSTR lpszWndTitle = L"",
		WNDPROC WindowProcess = nullptr,
		HWND hParent = nullptr
	);

	/**
	 * @brief 关闭某一绘图窗口
	 * @param [in] hWnd 窗口句柄（为空代表所有绘图窗口）
	*/
	void closegraph_win32(HWND hWnd = nullptr);

	/**
	 * @brief 设置某窗口的过程函数
	 * @param [in] hWnd 窗口句柄（为空标识当前活动窗口）
	 * @param [in] WindowProcess 新的过程函数
	*/
	void SetWndProcFunc(HWND hWnd, WNDPROC WindowProcess);

	/**
	 * @brief 得到当前活动绘图窗口的句柄
	*/
	HWND GetHWnd_win32();

	/**
	 * @brief 初始化窗口结束后，可以用此函数阻塞等待目标窗口被关闭，然后函数返回
	 * @param [in] hWnd 目标窗口（为空代表所有窗口）
	*/
	void init_end(HWND hWnd = nullptr);

	/**
	 * @brief 设置：当窗口都被销毁时，自动退出程序
	*/
	void AutoExit();

	/**
	 * @brief 是否还存在未销毁的绘图窗口
	*/
	bool isAnyWindow();

	/**
	 * @brief 判断一窗口是否还存在（未被关闭）
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	 * @return 是否存在
	*/
	bool isAliveWindow(HWND hWnd = nullptr);

	/**
	 * @brief 获取某窗口的图像指针
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	 * @return 缓冲区图像指针
	*/
	IMAGE* GetWindowImage(HWND hWnd = nullptr);

	/**
	 * @brief 获取窗口画布指针
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	 * @return 画布指针，若未绑定画布则返回空
	*/
	Canvas* GetWindowCanvas(HWND hWnd = nullptr);

	/**
	 * @brief 绑定窗口画布指针
	 * @note 绑定后，使用画布绘图时将自动开启任务，无需用户开启，但不会自动刷新屏幕
	 * @attention 绑定画布后，如在外部操作画布，则必须先启动窗口任务
	 * @param [in] pCanvas 画布指针
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	*/
	void BindWindowCanvas(Canvas* pCanvas, HWND hWnd = nullptr);

	/**
	 * @brief 将绘制在图像中的内容显示到目标窗口上（任意窗口，不局限于 HiWindow 窗口）
	 * @param [in] pImg	图像
	 * @param [in] hWnd	目标窗口
	*/
	void FlushDrawingToWnd(IMAGE* pImg, HWND hWnd);

	/**
	 * @brief 得到当前绘图窗口的详细信息
	*/
	EasyWindow GetWorkingWindow();

	/**
	 * @brief 等待当前任务完成并设置活动窗口
	 * @param [in] hWnd 新的活动窗口句柄
	 * @return 是否设置成功
	*/
	bool SetWorkingWindow(HWND hWnd);

	/**
	 * @brief 设置加速绘制跳过多少像素点
	 * @warning 此加速效果是有损的，加速效果与跳过的像素点数正相关
	 * @param [in] nSkipPixels 跳过的像素点数目
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	*/
	void QuickDraw(UINT nSkipPixels, HWND hWnd = nullptr);

	/**
	 * @brief 获取全局绘制模式
	*/
	DrawMode GetDrawMode();

	/**
	 * @brief 设置全局绘制模式
	 * @param [in] mode 全局绘制模式
	*/
	void SetDrawMode(DrawMode mode);

	/**
	 * @brief 重绘绘图窗口（在 WM_PAINT 消息内绘图不需要使用此函数）
	 * @param [in] hWnd 要重绘的窗口
	*/
	void EnforceRedraw(HWND hWnd = nullptr);

	/**
	 * @brief 为当前活动窗口启动任务
	 * @note 调用 EasyX 函数进行绘图或获取消息时，都应当启动任务。
	 * @return 是否启动成功（若已在任务中也返回 true）
	*/
	bool BeginTask();

	/**
	 * @brief 终止当前任务
	 * @param [in] flush 是否输出绘图缓冲（但不会自动刷新窗口）
	*/
	void EndTask(bool flush = true);

	/**
	 * @brief 判断某窗口是否在任务中
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	 * @return 是否在任务中
	 * @note 窗口任务是队列式的，只有活动窗口可能处在任务中。故若传入窗口不是活动窗口，将直接返回 false
	*/
	bool isInTask(HWND hWnd = nullptr);

	/**
	 * @brief 阻塞等待某窗口任务完成
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	*/
	void WaitForTask(HWND hWnd = nullptr);

	/**
	 * @brief 判断某窗口的大小是否改变
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	 * @return 窗口的大小是否改变
	*/
	bool isWindowSizeChanged(HWND hWnd = nullptr);

	/**
	 * @brief 为窗口创建一个托盘
	 * @param [in] lpszTrayName 托盘提示文本
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	 * @attention 每个窗口仅能稳定占有一个托盘
	*/
	void CreateTray(LPCTSTR lpszTrayName, HWND hWnd = nullptr);

	/**
	 * @brief 删除某窗口的托盘
	 * @param [in] hWnd 窗口句柄（为空表示当前活动窗口）
	*/
	void DeleteTray(HWND hWnd = nullptr);

	/**
	 * @brief 设置托盘菜单（允许在任何时候设置）
	 * @param [in] hMenu	菜单
	 * @param [in] hWnd	窗口句柄（为空表示当前活动窗口）
	*/
	void SetTrayMenu(HMENU hMenu, HWND hWnd = nullptr);

	/**
	 * @brief 设置托盘菜单消息处理函数
	 * @param [in] pFunc	消息处理函数
	 * @param [in] hWnd	窗口句柄（为空表示当前活动窗口）
	*/
	void SetTrayMenuProcFunc(void(*pFunc)(UINT), HWND hWnd = nullptr);

	/**
	 * @brief 获取自定义程序图标的启用状态
	*/
	bool GetCustomIconState();

	/**
	 * @brief 使用自定义图标资源作为程序图标
	 * @note 必须在第一次创建窗口前就调用该函数才能生效
	 * @param [in] lpszIcon		大图标资源
	 * @param [in] lpszIconSm	小图标资源
	 * @see 使用 MAKEINTRESOURCE 将资源 ID 转为字符串
	*/
	void SetCustomIcon(LPCTSTR lpszIcon, LPCTSTR lpszIconSm);

	/**
	 * @brief 在创建窗口前设置窗口样式，仅对此操作后首个新窗口生效
	 * @attention 新窗口的所有样式都将被当前样式覆盖
	 * @param [in] lStyle 新样式
	*/
	void PreSetWindowStyle(long lStyle);

	/**
	 * @brief 在创建窗口前设置窗口位置，仅对此操作后首个新窗口生效
	 * @param [in] x	位置
	 * @param [in] y	位置
	*/
	void PreSetWindowPos(int x, int y);

	/**
	 * @brief 设置某窗口样式
	 * @param [in] lNewStyle 新样式
	 * @param [in] hWnd		窗口句柄（为空代表当前活动窗口）
	 * @return 返回上一次设置的窗口样式，失败返回 0
	*/
	int SetWindowStyle(long lNewStyle, HWND hWnd = nullptr);

	/**
	 * @brief 设置某窗口扩展样式
	 * @param [in] lNewExStyle	新样式
	 * @param [in] hWnd 			窗口句柄（为空代表当前活动窗口）
	 * @return 返回上一次设置的窗口样式，失败返回 0
	*/
	int SetWindowExStyle(long lNewExStyle, HWND hWnd = nullptr);

	/**
	 * @brief 获取窗口位置
	 * @param [in] hWnd 窗口句柄（为空代表当前活动窗口）
	 * @return 窗口位置
	*/
	POINT GetWindowPos(HWND hWnd = nullptr);

	/**
	 * @brief 获取窗口大小
	 * @param [in] hWnd 窗口句柄（为空代表当前活动窗口）
	 * @return 窗口大小
	*/
	SIZE GetWindowSize(HWND hWnd = nullptr);

	/**
	 * @brief 移动窗口
	 * @param [in] x		位置
	 * @param [in] y		位置
	 * @param [in] hWnd	窗口句柄（为空代表当前活动窗口）
	*/
	void MoveWindow(int x, int y, HWND hWnd = nullptr);

	/**
	 * @brief 相对移动窗口
	 * @param [in] dx	相对位移
	 * @param [in] dy	相对位移
	 * @param [in] hWnd	窗口句柄（为空代表当前活动窗口）
	*/
	void MoveWindowRel(int dx, int dy, HWND hWnd = nullptr);

	/**
	 * @brief 重设窗口大小
	 * @param [in] w		窗口宽
	 * @param [in] h		窗口高
	 * @param [in] hWnd	窗口句柄（为空代表当前活动窗口）
	*/
	void ResizeWindow(int w, int h, HWND hWnd = nullptr);


	////////////****** 消息相关函数 ******////////////

	//// ExMessage 式函数

	/**
	 * @brief 阻塞等待，直到获取到一个新消息
	 * @param [in] filter	消息筛选方式
	 * @param [in] hWnd		窗口句柄（为空代表当前活动窗口）
	 * @return 获取到的消息
	*/
	ExMessage getmessage_win32(BYTE filter = -1, HWND hWnd = nullptr);

	/**
	 * @brief 阻塞等待，直到获取到一个新消息
	 * @param [out] msg	返回获取到的消息
	 * @param [in] filter	消息筛选方式
	 * @param [in] hWnd		窗口句柄（为空代表当前活动窗口）
	*/
	void getmessage_win32(ExMessage* msg, BYTE filter = -1, HWND hWnd = nullptr);

	/**
	 * @brief 获取一个消息，立即返回是否获取成功
	 * @param [out] msg	返回获取到的消息
	 * @param [in] filter	消息筛选方式
	 * @param [in] removemsg	获取消息后是否将其移除
	 * @param [in] hWnd		窗口句柄（为空代表当前活动窗口）
	 * @return 是否获取到消息
	*/
	bool peekmessage_win32(ExMessage* msg, BYTE filter = -1, bool removemsg = true, HWND hWnd = nullptr);

	/**
	 * @brief 清除所有消息记录
	 * @param [in] filter	消息筛选方式
	 * @param [in] hWnd		窗口句柄（为空代表当前活动窗口）
	*/
	void flushmessage_win32(BYTE filter = -1, HWND hWnd = nullptr);

	//// MOUSEMSG 式函数（兼容）

	/**
	 * @brief 检查是否存在鼠标消息
	 * @param [in] hWnd 窗口句柄（为空代表当前活动窗口）
	 * @return 是否存在鼠标消息
	*/
	bool MouseHit_win32(HWND hWnd = nullptr);

	/**
	 * @brief 阻塞等待，直到获取到一个新的鼠标消息
	 * @param [in] hWnd 窗口句柄（为空代表当前活动窗口）
	 * @return 鼠标消息
	*/
	MOUSEMSG GetMouseMsg_win32(HWND hWnd = nullptr);

	/**
	 * @brief 获取一个新的鼠标消息，立即返回是否获取成功
	 * @param [out] pMsg		返回获取到的消息
	 * @param [in] bRemoveMsg	获取消息后是否将其移除
	 * @param [in] hWnd			窗口句柄（为空代表当前活动窗口）
	 * @return 是否获取到消息
	*/
	bool PeekMouseMsg_win32(MOUSEMSG* pMsg, bool bRemoveMsg = true, HWND hWnd = nullptr);

	/**
	 * @brief 清空鼠标消息
	 * @param [in] hWnd 窗口句柄（为空代表当前活动窗口）
	*/
	void FlushMouseMsgBuffer_win32(HWND hWnd = nullptr);

	//// 转换

	/**
	 * @brief MOUSEMSG 转 ExMessage
	 * @param [in] msg MOUSEMSG 消息
	 * @return ExMessage 消息
	*/
	ExMessage To_ExMessage(MOUSEMSG msg);

	/**
	 * @brief ExMessage 转 MOUSEMSG
	 * @param [in] msgEx ExMessage 消息
	 * @return MOUSEMSG 消息
	*/
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

// 是否允许某窗口改变大小
#define EnableResizing(hwnd, state)		(state ? HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) | WS_SIZEBOX | WS_MAXIMIZEBOX) :\
										HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX))

#define DisableResizing(hwnd, state)	EnableResizing(hwnd, !state)

// 是否启用某窗口的系统菜单
#define EnableSystemMenu(hwnd, state)	(state ? HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) | WS_SYSMENU) :\
										HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) & ~WS_SYSMENU))

#define DisableSystemMenu(hwnd, state)	EnableSystemMenu(hwnd, !state)

// 是否启用当前窗口的工具栏样式
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
