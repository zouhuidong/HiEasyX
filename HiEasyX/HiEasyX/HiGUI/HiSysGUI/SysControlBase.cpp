#include "SysControlBase.h"

#include "../../HiWindow.h"

namespace HiEasyX
{
	HWND SysControlBase::CreateControl(HWND hParent, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle)
	{
		m_hParent = hParent;
		m_nID = AllocID();

		CREATESTRUCT c;
		c.lpCreateParams = 0;
		c.hInstance = 0;
		c.hMenu = (HMENU)(long long)GetID();
		c.hwndParent = hParent;
		c.cy = GetHeight();
		c.cx = GetWidth();
		c.y = GetY();
		c.x = GetX();
		c.style = dwStyle;
		c.lpszName = lpszWindowName;
		c.lpszClass = lpszClassName;
		c.dwExStyle = 0;

		return (HWND)SendMessage(hParent, WM_SYSCTRL_CREATE, (WPARAM)this, (LPARAM)&c);
	}

	SysControlBase::SysControlBase()
	{
	}

	SysControlBase::~SysControlBase()
	{
		if (m_hFont)
		{
			DeleteFont(m_hFont);
			m_hFont = nullptr;
		}
		SendMessage(m_hParent, WM_SYSCTRL_DELETE, (WPARAM)this, 0);
	}

	void SysControlBase::UpdateRect(RECT rctOld)
	{
		SetWindowPos(GetHandle(), 0, GetX(), GetY(), GetWidth(), GetHeight(), 0);
	}

	LRESULT SysControlBase::UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet)
	{
		return LRESULT();
	}

	HWND SysControlBase::Create(HWND hParent, RECT rct, std::wstring strText)
	{
		if (!GetHandle())
		{
			SetRect(rct);
			RealCreate(hParent);
			SetText(strText);
		}
		return GetHandle();
	}

	HWND SysControlBase::Create(HWND hParent, int x, int y, int w, int h, std::wstring strText)
	{
		if (!GetHandle())
		{
			SetRect(x, y, w, h);
			RealCreate(hParent);
			SetText(strText);
		}
		return GetHandle();
	}

	bool SysControlBase::isEnable()
	{
		return IsWindowEnabled(GetHandle());
	}

	void SysControlBase::Enable(bool enable)
	{
		EnableWindow(GetHandle(), enable);
	}

	bool SysControlBase::isVisible()
	{
		return IsWindowVisible(GetHandle());
	}

	void SysControlBase::Show(bool show)
	{
		ShowWindow(GetHandle(), show ? SW_SHOW : SW_HIDE);
	}

	bool SysControlBase::isFocused()
	{
		DWORD SelfThreadId = GetCurrentThreadId();						// 获取自身线程 ID
		DWORD ForeThreadId = GetWindowThreadProcessId(m_hParent, NULL);	// 根据窗口句柄获取线程 ID
		AttachThreadInput(ForeThreadId, SelfThreadId, true);			// 附加到线程
		HWND hWnd = GetFocus();											// 获取具有输入焦点的窗口句柄
		AttachThreadInput(ForeThreadId, SelfThreadId, false);			// 取消附加到线程
		return hWnd == GetHandle();
	}

	void SysControlBase::SetFocus(bool focused)
	{
		SendMessage(GetHandle(), focused ? WM_SETFOCUS : WM_KILLFOCUS, 0, 0);
	}

	int SysControlBase::GetTextLength()
	{
		return GetWindowTextLength(GetHandle());;
	}

	std::wstring SysControlBase::GetText()
	{
		int len = GetTextLength();
		WCHAR* buf = new WCHAR[len + 1];
		ZeroMemory(buf, sizeof WCHAR * (len + 1));
		GetWindowText(GetHandle(), buf, len + 1);
		std::wstring str = buf;
		delete[] buf;
		return str;
	}

	void SysControlBase::SetText(std::wstring wstr)
	{
		SetWindowText(GetHandle(), wstr.c_str());
	}

	HFONT SysControlBase::GetFont()
	{
		return (HFONT)SendMessage(GetHandle(), WM_GETFONT, 0, 0);
	}

	void SysControlBase::SetFont(int h, int w, LPCTSTR lpszTypeface)
	{
		if (m_hFont)
		{
			DeleteFont(m_hFont);
			m_hFont = nullptr;
		}
		m_hFont = CreateFont(
			h, w,
			0, 0, 0, 0, 0, 0,
			GB2312_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_MODERN,
			lpszTypeface
		);
		SendMessage(GetHandle(), WM_SETFONT, (WPARAM)m_hFont, 0);
	}

	int SysControlBase::GetID()
	{
		return m_nID;
	}

	int AllocID()
	{
		static int id = 10086;
		return id++;
	}
}
