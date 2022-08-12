////////////////////////////////////
//
//	SysGroupBox.h
//	HiSysGUI 控件分支：分组框
//

#pragma once

#include "SysControlBase.h"

namespace HiEasyX
{
	// 注意：分组框背景绘制有 Bug，请斟酌使用
	// 创建控件时不要将此控件作为父控件，否则无法创建控件
	class SysGroupBox : public SysControlBase
	{
	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysGroupBox();

		SysGroupBox(HWND hParent, RECT rct, std::wstring strText = L"");

		SysGroupBox(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");
	};
}
