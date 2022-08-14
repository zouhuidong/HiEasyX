/**
 * @file	SysRadioButton.h
 * @brief	HiSysGUI 控件分支：单选框
 * @author	huidong
*/

#pragma once

#include "SysControlBase.h"

namespace HiEasyX
{
	/**
	 * @brief 系统单选框控件
	*/
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

		/**
		 * @brief 注册点击消息
		 * @param [in] pFunc 消息响应函数
		*/
		void RegisterMessage(void (*pFunc)(bool checked));

		/**
		 * @brief 获取选中状态
		*/
		bool isChecked() const { return m_bChecked; }

		void Check(bool check);
	};
}
