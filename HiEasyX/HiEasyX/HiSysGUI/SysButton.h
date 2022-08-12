////////////////////////////////////
//
//	SysButton.h
//	HiSysGUI 控件分支：按钮
//

#pragma once

#include "SysControlBase.h"

#include <graphics.h>

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

		// 设置图片
		// reserve_text	保留按钮中的文字
		void Image(bool enable, IMAGE* img = nullptr, bool reserve_text = false);

		// 获取按下次数
		int GetClickCount();

		// 判断是否按下按键
		// 建议使用 GetClickCount，此函数可能丢失按下次数信息
		bool isClicked();
	};
}
