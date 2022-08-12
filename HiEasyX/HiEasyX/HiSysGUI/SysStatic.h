////////////////////////////////////
//
//	SysStatic.h
//	HiSysGUI 控件分支：静态控件
//

#pragma once

#include "SysControlBase.h"

#include <graphics.h>

namespace HiEasyX
{
	class SysStatic : public SysControlBase
	{
	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysStatic();

		SysStatic(HWND hParent, RECT rct, std::wstring strText = L"");

		SysStatic(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		// 设置文本居中
		void Center(bool center);

		// 设置图片
		void Image(bool enable, IMAGE* img = nullptr);
	};
}
