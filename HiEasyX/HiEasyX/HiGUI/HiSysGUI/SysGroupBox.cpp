#include "SysGroupBox.h"

namespace HiEasyX
{
	void SysGroupBox::RealCreate(HWND hParent)
	{
		m_type = SCT_GroupBox;
		m_hWnd = CreateControl(
			hParent,
			L"Button",
			L"",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX
		);

		HDC hdc = GetDC(m_hWnd);
		SetBkColor(hdc, RGB(255, 0, 0));
		ReleaseDC(m_hWnd, hdc);
	}

	SysGroupBox::SysGroupBox()
	{
	}

	SysGroupBox::SysGroupBox(HWND hParent, RECT rct, std::wstring strText)
	{
		Create(hParent, rct, strText);
	}

	SysGroupBox::SysGroupBox(HWND hParent, int x, int y, int w, int h, std::wstring strText)
	{
		Create(hParent, x, y, w, h, strText);
	}
}
