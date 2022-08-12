////////////////////////////////////
//
//	SysEdit.h
//	HiSysGUI 控件分支：输入框
//

#pragma once

#include "SysControlBase.h"

#include <graphics.h>

namespace HiEasyX
{
	class SysEdit : public SysControlBase
	{
	private:

		// 编辑框属性
		struct Property
		{
			bool left_align = true;			// 文字左对齐
			bool right_align = false;		// 文字右对齐
			bool uppercase = false;			// 输入内容转大写
			bool lowercase = false;			// 输入内容转小写
			bool password = false;			// 密码框
			bool read_only = false;			// 只读
			bool number_only = false;		// 仅数字输入
		} m_property;

		long m_lBasicStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
		void (*m_pFunc)(std::wstring wstrText) = nullptr;
		bool m_bEdited = false;

		COLORREF m_cBk = WHITE;
		COLORREF m_cTextBk = WHITE;
		COLORREF m_cText = BLACK;
		HBRUSH hBk = nullptr;

		// 应用设置的属性
		void ApplyProperty();

	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysEdit();

		SysEdit(HWND hParent, RECT rct, std::wstring strText = L"");

		SysEdit(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");

		~SysEdit() override;

		// 在创建控件前预设样式
		void PreSetStyle(
			bool multiline,					// 多行文本
			bool center_text = false,		// 文本居中
			bool vscroll = false,			// 拥有垂直滚动条
			bool auto_vscroll = false,		// 自动垂直滚动
			bool hscroll = false,			// 拥有水平滚动条
			bool auto_hscroll = false		// 自动水平滚动
		);

		LRESULT UpdateMessage(UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet) override;

		// 注册输入消息
		void RegisterMessage(void (*pFunc)(std::wstring wstrText));

		// 设置文本右对齐
		void RightAlign(bool enable);

		// 自动转大写
		void Uppercase(bool enable);

		// 自动转小写
		void Lowercase(bool enable);

		// 密码框
		void Password(bool enable);

		// 只读
		void ReadOnly(bool enable);

		// 仅数字
		void NumberOnly(bool enable);

		// 获取最大输入长度
		int GetMaxTextLength();

		// 限制最大输入长度
		void SetMaxTextLength(int len);

		// 设置控件背景色
		void SetBkColor(COLORREF color);

		// 设置文本背景色
		void SetTextBkColor(COLORREF color);

		// 设置文本颜色
		void SetTextColor(COLORREF color);

		// 获取光标选择区域
		void GetSel(int* begin, int* end);

		// 设置关闭选择区域
		// end 为 -1 可表示结尾
		void SetSel(int begin, int end);

		// 复制选中的文本
		void Copy();

		// 剪切选中的文本
		void Cut();

		// 从剪切板黏贴文本到当前位置
		void Paste();

		// 删除选中的文本
		void Delete();

		// 替换选中的文本
		void Replace(std::wstring wstrText);

		// 是否被编辑
		bool isEdited();
	};
}
