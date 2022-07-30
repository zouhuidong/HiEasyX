////////////////////////////////////
//
//	Button.h
//	HiGUI 控件分支：按钮控件
//

#pragma once

#include "ControlBase.h"

namespace HiEasyX
{
	// 按钮控件
	class Button : public ControlBase
	{
	protected:

		bool m_bEnableClassicStyle = false;			// 是否使用经典样式

		void InitColor()
		{
			m_cBorder = m_cBorder_Normal;
			m_cBackground = m_cBackground_Normal;
		}

	public:

		// 按钮颜色设置（现代样式）
		COLORREF m_cBorder_Normal = MODEN_BORDER_GRAY;
		COLORREF m_cBackground_Normal = MODEN_FILL_GRAY;
		COLORREF m_cBorder_Hovered = MODEN_BORDER_BLUE;
		COLORREF m_cBackground_Hovered = MODEN_FILL_BLUE;
		COLORREF m_cBorder_Pressed = MODEN_BORDER_PRESSED_BLUE;
		COLORREF m_cBackground_Pressed = MODEN_FILL_PRESSED_BLUE;

		COLORREF m_cText_Disabled = LIGHTGRAY;
		COLORREF m_cBorder_Disabled = GRAY;
		COLORREF m_cBackground_Disabled = GRAY;

		COLORREF m_cClassicNormalBorder3D = GRAY;			// 未按下时的 3D 边框颜色（经典样式）
		COLORREF m_cClassicPressedBorder3D = LIGHTGRAY;		// 按下时的 3D 边框颜色（经典样式）

		Button();

		Button(int x, int y, int w, int h, std::wstring wstrText = L"");

		void SetEnable(bool enable) override;

		virtual void EnableClassicStyle(bool enable);

		void UpdateMessage(ExMessage msg) override;

		void Draw(bool draw_child = true) override;
	};
}

