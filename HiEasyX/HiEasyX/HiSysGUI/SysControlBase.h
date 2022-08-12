////////////////////////////////////
//
//	SysControlBase.h
//	HiSysGUI 控件分支：控件基础
//

#pragma once

#include "../HiContainer.h"

#include <windowsx.h>
#include <string>

namespace HiEasyX
{
	// 支持的系统控件类型
	enum SysControlType
	{
		SCT_Unknown,
		SCT_Group,
		SCT_GroupBox,
		SCT_Static,
		SCT_Button,
		SCT_CheckBox,
		SCT_RadioButton,
		SCT_Edit,
		SCT_ComboBox,
	};

	class SysControlBase : public Container
	{
	private:

		HFONT m_hFont = nullptr;

	protected:

		HWND m_hWnd = nullptr;
		HWND m_hParent = nullptr;
		int m_nID = 0;
		SysControlType m_type = SCT_Unknown;

		// 创建控件
		HWND CreateControl(HWND hParent, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle);

		// 实际调用的创建控件函数
		virtual void RealCreate(HWND hParent) = 0;

	public:

		SysControlBase();

		virtual ~SysControlBase();

		void UpdateRect(RECT rctOld) override;

		// 更新消息，此函数无需用户调用
		// bRet 传出，标记是否返回值
		virtual LRESULT UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet);

		// 注：控件只能创建一次
		HWND Create(HWND hParent, RECT rct, std::wstring strText = L"");

		// 注：控件只能创建一次
		HWND Create(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		HWND GetHandle() const { return m_hWnd; }

		// 获取控件类型
		SysControlType GetControlType() const { return m_type; }

		bool isEnable();

		void Enable(bool enable);

		bool isVisible();

		void Show(bool show);

		bool isFocused();

		void SetFocus(bool focused);

		int GetTextLength();

		std::wstring GetText();

		void SetText(std::wstring wstr);

		HFONT GetFont();

		void SetFont(int h, int w = 0, std::wstring typeface = L"");

		int GetID();

	};

	// 自动分配控件 ID
	int AllocID();
}
