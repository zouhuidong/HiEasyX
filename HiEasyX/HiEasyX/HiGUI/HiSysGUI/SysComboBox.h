////////////////////////////////////
//
//	SysComboBox.h
//	HiSysGUI 控件分支：组合框
//

#pragma once

#include "SysControlBase.h"

namespace HiEasyX
{
	class SysComboBox : public SysControlBase
	{
	private:

		long m_lBasicStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWN | CBS_HASSTRINGS | CBS_AUTOHSCROLL;
		int m_nSel = -1;
		bool m_bSelChanged = false;
		bool m_bEdited = false;
		void (*m_pFuncSel)(int sel, std::wstring wstrSelText) = nullptr;
		void (*m_pFuncEdit)(std::wstring wstrText) = nullptr;

		// 用户设置了 CBS_SIMPLE 同时禁用输入
		// 此情况需要特殊实现
		bool m_bSimple_No_Edit = false;

	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysComboBox();

		SysComboBox(HWND hParent, RECT rct, std::wstring strText = L"");

		SysComboBox(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		// 在创建控件前预设样式
		void PreSetEtyle(
			bool always_show_list,	// 是否总是显示列表
			bool editable,			// 是否可编辑
			bool sort				// 是否自动排序
		);

		LRESULT UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet) override;

		// 注册选择消息
		void RegisterSelMessage(void (*pFunc)(int sel, std::wstring wstrSelText));

		// 注册编辑消息
		void RegisterEditMessage(void (*pFunc)(std::wstring wstrText));

		// 获取选中的索引
		int GetSel() const { return m_nSel; }

		// 设置选中的索引
		void SetSel(int sel);

		// 选择具有指定文本的一项
		// 返回是否选择成功
		bool SelectString(std::wstring wstrText);

		// 增加项
		void AddString(std::wstring wstrText);

		// 插入项
		void InsertString(int index, std::wstring wstrText);

		// 删除项
		void DeleteString(int index);

		// 获取列表内容数量
		int GetCount();
		
		// 清空列表
		void Clear();

		// 显示列表
		void ShowDropdown(bool enable);

		// 选择项是否变化
		bool isSelChanged();

		// 是否被编辑
		bool isEdited();
	};
}
