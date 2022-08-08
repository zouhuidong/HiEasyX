////////////////////////////////////
//
//	SysControlBase.h
//	HiSysGUI 控件分支：控件基础
//

#pragma once

#include "SysControlBase.h"

namespace HiEasyX
{
	class SysButton : public SysControlBase
	{
	private:
		int m_nClickCount = 0;
		void (*m_pFunc)() = nullptr;

	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysButton();

		SysButton(HWND hParent, RECT rct, std::wstring strText = L"");

		SysButton(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		LRESULT UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet) override;

		// 注册点击消息
		void RegisterMessage(void (*pFunc)());

		// 获取按下次数
		int GetClickCount();
	};
}
