////////////////////////////////////
//
//	SysRadioButton.h
//	HiSysGUI 控件分支：单选框
//

#pragma once

#include "SysControlBase.h"

namespace HiEasyX
{
	class SysRadioButton : public SysControlBase
	{
	private:
		bool m_bChecked = false;
		void (*m_pFunc)(bool checked) = nullptr;

	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysRadioButton();

		SysRadioButton(HWND hParent, RECT rct, std::wstring strText = L"");

		SysRadioButton(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		LRESULT UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet) override;

		// 注册点击消息
		void RegisterMessage(void (*pFunc)(bool checked));

		// 获取选中状态
		bool isChecked() const { return m_bChecked; }

		void Check(bool check);
	};
}
