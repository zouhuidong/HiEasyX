/**
 * @file	SysControlBase.h
 * @brief	HiSysGUI 控件分支：控件基础
 * @author	huidong
*/

#pragma once

#include "../HiContainer.h"

#include <windowsx.h>
#include <string>

namespace HiEasyX
{
	/**
	 * @brief 支持的系统控件类型
	*/
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

	/**
	 * @brief 系统控件基础
	*/
	class SysControlBase : public Container
	{
	private:

		HFONT m_hFont = nullptr;

	protected:

		HWND m_hWnd = nullptr;
		HWND m_hParent = nullptr;
		int m_nID = 0;
		SysControlType m_type = SCT_Unknown;

		/**
		 * @brief 创建控件
		 * @param [in] hParent			父控件 
		 * @param [in] lpszClassName		窗口类名
		 * @param [in] lpszWindowName	窗口名
		 * @param [in] dwStyle			窗口样式
		 * @return 窗口句柄
		*/
		HWND CreateControl(HWND hParent, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle);

		/**
		 * @brief 实际调用的创建控件函数（各种控件实现不同，但内部都调用 CreateControl 创建控件）
		 * @param [in] hParent 父控件句柄
		*/
		virtual void RealCreate(HWND hParent) = 0;

	public:

		SysControlBase();

		virtual ~SysControlBase();

		void UpdateRect(RECT rctOld) override;

		/**
		 * @brief 更新消息，此函数无需用户调用
		 * @param [in] msg		新消息
		 * @param [in] wParam	参数
		 * @param [in] lParam	参数
		 * @param [out] bRet 标记是否返回值
		 * @return 不定返回值
		*/
		virtual LRESULT UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet);

		/**
		 * @brief 创建控件
		 * @attention 控件只能创建一次
		 * @param [in] hParent	父窗口句柄
		 * @param [in] rct		控件区域
		 * @param [in] strText	控件文本
		 * @return 控件窗口句柄
		*/
		HWND Create(HWND hParent, RECT rct, std::wstring strText = L"");

		HWND Create(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		HWND GetHandle() const { return m_hWnd; }

		/**
		 * @brief 获取此控件类型
		*/
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

	/**
	 * @brief 自动分配控件 ID
	*/
	int AllocID();
}
