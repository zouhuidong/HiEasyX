#include "SysCheckBox.h"

namespace HiEasyX
{
	void SysCheckBox::RealCreate(HWND hParent)
	{
		m_type = SCT_CheckBox;
		m_hWnd = CreateControl(
			hParent,
			L"Button",
			L"",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX
		);
	}

	SysCheckBox::SysCheckBox()
	{
	}

	SysCheckBox::SysCheckBox(HWND hParent, RECT rct, std::wstring strText)
	{
		Create(hParent, rct, strText);
	}

	SysCheckBox::SysCheckBox(HWND hParent, int x, int y, int w, int h, std::wstring strText)
	{
		Create(hParent, x, y, w, h, strText);
	}

	LRESULT SysCheckBox::UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet)
	{
		if (msg == WM_COMMAND)
		{
			if (LOWORD(wParam) == GetID())
			{
				m_bChecked = Button_GetCheck(GetHandle());
				if (m_pFunc)
					m_pFunc(m_bChecked);
			}
		}

		bRet = false;
		return 0;
	}

	void SysCheckBox::RegisterMessage(void(*pFunc)(bool checked))
	{
		m_pFunc = pFunc;
	}

	void SysCheckBox::Check(bool check)
	{
		Button_SetCheck(GetHandle(), check);
		m_bChecked = check;
	}
}
