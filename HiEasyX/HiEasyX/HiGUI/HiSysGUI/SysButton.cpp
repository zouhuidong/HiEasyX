#include "SysButton.h"

namespace HiEasyX
{
	void SysButton::RealCreate(HWND hParent)
	{
		m_type = SCT_Button;
		m_hWnd = CreateControl(
			hParent,
			L"Button",
			L"",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | /*BS_AUTOCHECKBOX*/ BS_AUTORADIOBUTTON | WS_GROUP
		);
		//SetSysColors(GetID(),)
	}

	SysButton::SysButton()
		: SysControlBase()
	{
	}

	SysButton::SysButton(HWND hParent, RECT rct, std::wstring strText)
		: SysControlBase(hParent, rct, strText)
	{
	}

	SysButton::SysButton(HWND hParent, int x, int y, int w, int h, std::wstring strText)
		:SysControlBase(hParent, x, y, w, h, strText)
	{
	}

	LRESULT SysButton::UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet)
	{
		if (msg == WM_COMMAND)
		{
			if (LOWORD(wParam) == GetID())
			{
				m_nClickCount++;
				if (m_pFunc)
					m_pFunc();
				//SendMessage(GetHandle(),BM_SETCHECK,)
			}
		}

		bRet = false;
		return 0;
	}

	void SysButton::RegisterMessage(void(*pFunc)())
	{
		m_pFunc = pFunc;
	}

	int SysButton::GetClickCount()
	{
		int c = m_nClickCount;
		m_nClickCount = 0;
		return c;
	}
}
