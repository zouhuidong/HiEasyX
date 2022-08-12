////////////////////////////////////
//
//	HiWindow.h
//	HiEasyX ��Ĵ���ģ��
//

#pragma once

#include "HiDef.h"
#include "HiFunc.h"
#include <graphics.h>
#include <windowsx.h>
#include <vector>
#include <thread>


#define __HIWINDOW_H__

// �����ͼ���ڳ�ʼ������
// ��ͨ����
#define EW_NORMAL							0

// ������Ϣ
#define WM_TRAY								(WM_USER + 9337)

// �޴���ʱ������
#define NO_WINDOW_INDEX						-1

// ���ڹ��̺���Ĭ�Ϸ���ֵ
#define HIWINDOW_DEFAULT_PROC				(LRESULT)(-10086)

// ϵͳ�ؼ�������Ϣ
// wParam ���� SysControlBase*
// lParam ���� CREATESTRUCT*
#define WM_SYSCTRL_CREATE					(WM_USER + 9338)

// ϵͳ�ؼ�������Ϣ
// wParam ���� SysControlBase*
#define WM_SYSCTRL_DELETE					(WM_USER + 9339)


namespace HiEasyX
{
	class Canvas;
	class SysControlBase;

	////////////****** ���Ͷ��� ******////////////

	//	Easy Window ���ڹ��̺���
	//
	//	���ڹ��̺����淶
	// 
	//	����ǩ����
	//		LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 
	//	ע�����
	//		��Ҫ��Ĭ�Ϸ�ʽ������Ϣ���򷵻� HIWINDOW_DEFAULT_PROC ���ɣ���Ҫʹ�� DefWindowProc ������
	/*
		// ʾ������
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
				// TODO: �ڴ˴��ͷ�������ڴ�
				PostQuitMessage(0);
				break;

			default:
				return HIWINDOW_DEFAULT_PROC;	// ��ʶʹ��Ĭ����Ϣ����������������

				// ��Ҫ��Ĭ�Ϸ�ʽ����������ʹ�ô����
				//return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			return 0;
		}
	*/

	// ����
	struct EasyWindow
	{
		bool isAlive;								// �����Ƿ����

		HWND hWnd;									// ���ھ��
		HWND hParent;								// �����ھ��

		IMAGE* pImg;								// ����ͼ��
		IMAGE* pBufferImg;							// ͼ�񻺳���
		Canvas* pBufferImgCanvas;					// ͼ�񻺳����󶨵Ļ���ָ��

		WNDPROC funcWndProc;						// ������Ϣ��������

		std::vector<ExMessage> vecMessage;			// ģ�� EasyX ������Ϣ����

		bool isUseTray;								// �Ƿ�ʹ������
		NOTIFYICONDATA nid;							// ������Ϣ
		bool isUseTrayMenu;							// �Ƿ�ʹ�����̲˵�
		HMENU hTrayMenu;							// ���̲˵�

		void(*funcTrayMenuProc)(UINT);				// ���̲˵���Ϣ��������
													// �����˺�����Ϊ�˷�����Ӧ���̵Ĳ˵���Ϣ
													// ������Ӧ������������Ϣ�����Զ��崰�ڹ��̺��������� WM_TRAY ��Ϣ

		bool isNewSize;								// ���ڴ�С�Ƿ�ı�
		bool isBusyProcessing;						// �Ƿ���æ�ڴ����ڲ���Ϣ��ָ�������û���������������

		int nSkipPixels;							// ����ʱ���������ص��������������ٻ棩

		std::vector<SysControlBase*> vecSysCtrl;	// ��¼������ϵͳ�ؼ�
	};

	// ������
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

		// �ȼ��� InitWindow
		HWND Create(
			int w = 640,
			int h = 480,
			int flag = EW_NORMAL,
			LPCTSTR lpszWndTitle = L"",
			WNDPROC WindowProcess = nullptr,
			HWND hParent = nullptr
		);

		void CloseWindow();

		// �ȼ��� CloseWindow
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

		// �����ͼ���壨�����ػ洰�ڣ�
		void FlushDrawing();

		// �ػ洰��
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

		SIZE GetWindowSize();				// ��ȡ�������ڵĴ�С

		int GetWindowWidth();				// ��ȡ�������ڵĿ���

		int GetWindowHeight();				// ��ȡ�������ڵĸ߶�

		int GetClientWidth();				// ��ȡ�ͻ�������

		int GetClientHeight();				// ��ȡ�ͻ����߶�

		void Move(int x, int y);

		void MoveRel(int dx, int dy);

		void Resize(int w, int h);

		// �Ƿ�Ϊ�û�����ʹ�õĴ���
		bool isForegroundWindow();

		ExMessage Get_Message(BYTE filter = -1);

		void Get_Message(ExMessage* msg, BYTE filter = -1);

		bool Peek_Message(ExMessage* msg, BYTE filter = -1, bool removemsg = true);

		void Flush_Message(BYTE filter = -1);
	};

	////////////****** ������غ��� ******////////////

	// ���� Win32 ��ͼ���ڣ�Ĭ��֧�ִ���˫����Ϣ���������� EasyX ԭ�����ڴ����Ĳ��裩
	// w, h				���ڴ�С
	// flag				������ʽ��ʶ
	// lpszWndTitle		���ڱ���
	// WindowProcess	������Ϣ����������ָ�룬Ϊ�ձ�ʾʹ��Ĭ����Ϣ�������������ͷ�ļ��С�������Ϣ���������淶����
	// hParent			�����ھ����Ϊ�����ʾ�ô��ڶ������ڣ�����д���ô��ڽ���Ϊģ̬���ڣ�
	//
	// ע��������������ͼ���ڣ�������������ʱ������ᵼ��һЩ���⡣
	HWND initgraph_win32(
		int w = 640,
		int h = 480,
		int flag = EW_NORMAL,
		LPCTSTR lpszWndTitle = L"",
		WNDPROC WindowProcess = nullptr,
		HWND hParent = nullptr
	);

	// �ر�ĳһ��ͼ���ڣ������Ϊ nullptr ��ر����л�ͼ����
	void closegraph_win32(HWND hWnd = nullptr);

	// ����ĳ���ڵĹ��̺���������վ�����Ա�ʶ��ǰ����ڣ�
	void SetWndProcFunc(HWND hWnd, WNDPROC WindowProcess);

	// �õ���ǰ��ͼ���ڵľ��
	HWND GetHWnd_win32();

	// ������ʼ������
	// ������ֱ��Ŀ�괰�ڱ��ر�
	// �������� nullptr����ȴ����д��ڹر�
	void init_end(HWND hWnd = nullptr);

	// ���ã������ڶ�������ʱ���Զ��˳�����
	void AutoExit();

	// �Ƿ񻹴���δ���ٵĻ�ͼ����
	bool isAnyWindow();

	// �ж�һ�����Ƿ񻹴��ڣ������ѱ��رյĴ��ڣ�
	// ����վ�����Ա�ʶ��ǰ�����
	bool isAliveWindow(HWND hWnd = nullptr);

	// ��ȡĳ���ڵ�ͼ��ָ��
	IMAGE* GetWindowImage(HWND hWnd = nullptr);

	// ��ȡ���ڻ���ָ��
	Canvas* GetWindowCanvas(HWND hWnd = nullptr);

	// �󶨴��ڻ���ָ��
	// �󶨺�ʹ�û�����ͼʱ���Զ��������������û��������������Զ�ˢ����Ļ
	// �󶨻����������ⲿ�����������������������������
	void BindWindowCanvas(Canvas* pCanvas, HWND hWnd = nullptr);

	// �������� EasyX �е�������ʾ��Ŀ�괰���ϣ����ⴰ�ڣ�
	void FlushDrawingToWnd(IMAGE* pImg, HWND hWnd);

	// �����ȴ���ͼ�������
	// �������������ֻ�иþ�������ǻ����ʱ�ŵȴ�
	void WaitForTask(HWND hWnd = nullptr);

	// �õ���ǰ��ͼ���ڵ���ϸ��Ϣ
	EasyWindow GetWorkingWindow();

	// �ȴ���ǰ������ɲ����û���ڣ������Ƿ����óɹ�
	bool SetWorkingWindow(HWND hWnd);

	// ���ü��ٻ��������������ص�
	// �˼���Ч��������ģ�����Ч�������������ص�������ء�
	void QuickDraw(UINT nSkipPixels, HWND hWnd = nullptr);

	// ǿ���ػ��ͼ���ڣ��� WM_PAINT ��Ϣ�ڻ�ͼ����Ҫʹ�ô˺�����
	void EnforceRedraw(HWND hWnd = nullptr);

	// Ϊ��ǰ������������񣬷����Ƿ������ɹ���������������Ҳ���� true��
	// ���� EasyX �������л�ͼ���ȡ��Ϣʱ����Ӧ�������������ٽ��е��á�
	bool BeginTask();

	// ��ֹ��ǰ���񣬣���ѡ�������ͼ����
	void EndTask(bool flush = true);

	// �жϵ�ǰ�Ƿ��������ڽ���
	// ����������������ж����Ƿ�Ϊ�����
	bool isInTask(HWND hWnd = nullptr);

	// �ж�ĳ���ڵĴ�С�Ƿ�ı�
	// ����վ�����Ա�ʶ��ǰ�����
	bool isWindowSizeChanged(HWND hWnd = nullptr);

	// Ϊ���ڴ���һ������
	// ����������ʾ�ı�
	// ע�⣺ÿ�����ڽ����ȶ�ռ��һ������
	void CreateTray(LPCTSTR lpszTrayName, HWND hWnd = nullptr);

	// ɾ��ĳ���ڵ����̣������ָ����Ա�ʶ��ǰ�����
	void DeleteTray(HWND hWnd = nullptr);

	// �������̲˵����������κ�ʱ�����ã�
	void SetTrayMenu(HMENU hMenu, HWND hWnd = nullptr);

	// �������̲˵���Ϣ��������
	void SetTrayMenuProcFunc(void(*pFunc)(UINT), HWND hWnd = nullptr);

	// �ж��Զ������ͼ�������״̬
	bool GetCustomIconState();

	// ʹ��ͼ����Դ��Ϊ����ͼ��
	// ��������ͼ����Դ ID����ͼ���Сͼ�꣩
	// ע�������ڵ�һ�δ�������ǰ�͵��øú���������Ч��Ĭ������£������Ի� EasyX ����ͼ��
	void SetCustomIcon(int nIcon, int nIconSm);

	// �ڴ�������ǰ���ô�����ʽ�����Դ˲������׸��´�����Ч
	// ע�⣺�´��ڵ�������ʽ��������ǰ��ʽ����
	void PreSetWindowStyle(long lStyle);

	// �ڴ�������ǰ���ô���λ�ã����Դ˲������׸��´�����Ч
	void PreSetWindowPos(int x, int y);

	// ���õ�ǰ������ʽ
	int SetWindowStyle(long lNewStyle, HWND hWnd = nullptr);

	// ���õ�ǰ������չ��ʽ
	int SetWindowExStyle(long lNewExStyle, HWND hWnd = nullptr);

	// ��ȡ����λ��
	POINT GetWindowPos(HWND hWnd = nullptr);

	// ��ȡ���ڴ�С
	SIZE GetWindowSize(HWND hWnd = nullptr);

	// �ƶ�����
	void MoveWindow(int x, int y, HWND hWnd = nullptr);

	// ����ƶ�����
	void MoveWindowRel(int dx, int dy, HWND hWnd = nullptr);

	// ���贰�ڴ�С
	void ResizeWindow(int w, int h, HWND hWnd = nullptr);


	////////////****** ��Ϣ��غ��� ******////////////

	//// ExMessage ʽ����

	// �����ȴ���ֱ����ȡ��һ������Ϣ
	ExMessage getmessage_win32(BYTE filter = -1, HWND hWnd = nullptr);

	// �����ȴ���ֱ����ȡ��һ������Ϣ
	void getmessage_win32(ExMessage* msg, BYTE filter = -1, HWND hWnd = nullptr);

	// ��ȡһ����Ϣ�����������Ƿ��ȡ�ɹ�
	bool peekmessage_win32(ExMessage* msg, BYTE filter = -1, bool removemsg = true, HWND hWnd = nullptr);

	// ���������Ϣ��¼
	void flushmessage_win32(BYTE filter = -1, HWND hWnd = nullptr);

	//// MOUSEMSG ʽ���������ݣ�

	// ����Ƿ���������Ϣ
	bool MouseHit_win32(HWND hWnd = nullptr);

	// �����ȴ���ֱ����ȡ��һ���µ������Ϣ
	MOUSEMSG GetMouseMsg_win32(HWND hWnd = nullptr);

	// ��ȡһ���µ������Ϣ�����������Ƿ��ȡ�ɹ�
	bool PeekMouseMsg_win32(MOUSEMSG* pMsg, bool bRemoveMsg = true, HWND hWnd = nullptr);

	// ��������Ϣ
	void FlushMouseMsgBuffer_win32(HWND hWnd = nullptr);

	//// ת��

	// MOUSEMSG ת ExMessage
	ExMessage To_ExMessage(MOUSEMSG msg);

	// ExMessage ת MOUSEMSG
	MOUSEMSG To_MouseMsg(ExMessage msgEx);
}

////////////****** ����ָ��궨�� ******////////////

// ����һ�Σ���ͼ�����񣨻��Ƶ���ǰ��ͼ���ڣ�
#define BEGIN_TASK()\
	if (HiEasyX::SetWorkingWindow(nullptr))\
	{\
		if (HiEasyX::BeginTask())\
		{(0)	/* �˴�ǿ��Ҫ��ӷֺ� */

// ����һ�Σ���ͼ������ָ��Ŀ���ͼ���ڣ�
#define BEGIN_TASK_WND(hWnd)\
	/* ���ù�������ʱ���Զ��ȴ����� */\
	if (HiEasyX::SetWorkingWindow(hWnd))\
	{\
		if (HiEasyX::BeginTask())\
		{(0)

// ����һ�Σ���ͼ�����񣬲������ͼ���棨���� BEGIN_TASK ���ã�
#define END_TASK()\
			HiEasyX::EndTask();\
		}\
	}(0)

// Ҫ�󴰿��ػ�
#define FLUSH_DRAW()			HiEasyX::EnforceRedraw()

////////////****** ������ʽ�궨�� ******////////////

// �Ƿ�����ĳ���ڸı��С
#define EnableResizing(hwnd, state)		(state ? HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) | WS_SIZEBOX | WS_MAXIMIZEBOX) :\
										HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX))

#define DisableResizing(hwnd, state)	EnableResizing(hwnd, !state)

// �Ƿ�����ĳ���ڵ�ϵͳ�˵�
#define EnableSystemMenu(hwnd, state)	(state ? HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) | WS_SYSMENU) :\
										HiEasyX::SetWindowStyle(GetWindowStyle(hwnd) & ~WS_SYSMENU))

#define DisableSystemMenu(hwnd, state)	EnableSystemMenu(hwnd, !state)

// �Ƿ����õ�ǰ���ڵĹ�������ʽ
#define EnableToolWindowStyle(state)	(state ? HiEasyX::SetWindowExStyle(GetWindowExStyle(HiEasyX::GetHWnd_win32()) | WS_EX_TOOLWINDOW) :\
										HiEasyX::SetWindowExStyle(GetWindowExStyle(HiEasyX::GetHWnd_win32()) & ~WS_EX_TOOLWINDOW))

////////////****** ������Ϣ�궨�� ******////////////

// �ж�ȫ�ְ���״̬
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

// �жϵ�ǰ������Ƿ���ܵ�ĳ������Ϣ
#define KEY_DOWN_WND(VK_NONAME) (GetForegroundWindow() == HiEasyX::GetHWnd_win32() && KEY_DOWN(VK_NONAME))

////////////****** EasyX ԭ�������ĺ��滻 ******////////////

// ��ʹ�� EasyX ԭ�������������ڣ���رմ���ʱ�Զ��˳�����
#define initgraph(...)			HiEasyX::initgraph_win32(__VA_ARGS__);\
								HiEasyX::AutoExit()

#define closegraph				HiEasyX::closegraph_win32

// Ĭ��ʹ��˫���壬�� BeginBatchDraw ������
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