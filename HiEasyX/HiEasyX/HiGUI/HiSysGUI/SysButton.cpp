#include "SysButton.h"

namespace HiEasyX
{
	void SysButton::RealCreate(HWND hParent)
	{
		m_hWnd = CreateControl(
			hParent,
			L"Button",
			L"",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		);
		m_type = SCT_Button;
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

}
