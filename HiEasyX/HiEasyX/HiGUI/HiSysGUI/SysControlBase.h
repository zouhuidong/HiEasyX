////////////////////////////////////
//
//	SysControlBase.h
//	HiSysGUI 控件分支：控件基础
//

#pragma once

#include "../Container.h"

#include <string>

namespace HiEasyX
{
	enum SysControlType
	{
		SCT_Unknown,
		SCT_Button,
		SCT_EditBox,
	};

	class SysControlBase : public Container
	{
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

		SysControlBase(HWND hParent, RECT rct, std::wstring strText = L"");

		SysControlBase(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		virtual ~SysControlBase();

		void UpdateRect(RECT rctOld) override;

		// 更新消息，此函数无需用户调用
		virtual void UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

		// 注：控件只能创建一次
		HWND Create(HWND hParent, RECT rct, std::wstring strText = L"");

		// 注：控件只能创建一次
		HWND Create(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		HWND GetHandle() const { return m_hWnd; }

		SysControlType GetControlType() const { return m_type; }

		bool isEnable();

		void Enable(bool enable);

		bool isVisible();

		void Show(bool show);

		bool isFocused();

		void SetFocus(bool focused);

		std::wstring GetText();

		void SetText(std::wstring wstr);
		
		int GetID();
		
	};

	// 自动分配控件 ID
	int AllocID();
}
