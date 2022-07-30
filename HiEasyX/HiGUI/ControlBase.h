////////////////////////////////////
//
//	ControlBase.h
//	HiGUI 控件分支：控件基础
//

#pragma once

#include "Container.h"

#include "../HiMacro.h"
#include "../HiFunc.h"
#include "../HiCanvas.h"

namespace HiEasyX
{
	// 控件消息
	enum CtrlMessage
	{
		CM_OTHER,						// 未特殊标识的其它消息
		CM_HOVER,						// 悬停
		CM_HOVER_OVER,					// 悬停结束
		CM_PRESS,						// 按下
		CM_PRESS_OVER,					// 按下结束
		CM_CLICK,						// 单击
		CM_DOUBLE_CLICK,				// 双击
		CM_FOCUS,						// 获取焦点
		CM_FOCUS_OVER,					// 丢失焦点

	};

	class ControlBase;

	// 控件消息处理函数
	// _Ctrl	传入控件指针
	// _MsgId	传入消息标识代码
	// _ExMsg	传入详细消息（坐标已变换到控件）
	typedef void (*MESSAGE_PROC_FUNC)(ControlBase* _Ctrl, int _MsgId, ExMessage _ExMsg);

	// 支持静态类函数作为控件消息处理函数
	typedef void (*MESSAGE_PROC_FUNC_CLASS)(void* _This, ControlBase* _Ctrl, int _MsgId, ExMessage _ExMsg);

	// 控件基础
	class ControlBase : public Container
	{
	protected:

		bool m_bEnabled = true;										// 是否可用
		bool m_bVisible = true;										// 是否可见

		std::wstring m_wstrText;									// 控件文本

		Canvas m_canvas;											// 画布
		BYTE m_alpha = 255;											// 透明度
		bool m_bUseCanvasAlpha = false;								// 是否使用画布自身的透明度信息
		bool m_isAlphaCalculated = false;							// 画布是否已经计算透明混合颜色

		COLORREF m_cBorder = MODEN_BORDER_GRAY;						// 边框颜色
		COLORREF m_cBackground = CLASSICGRAY;						// 背景色
		COLORREF m_cText = BLACK;									// 文本颜色

		bool m_bEnableBorder = true;								// 是否绘制边框
		int m_nBorderThickness = 1;									// 边框粗细

		bool m_bCompleteFirstSetRect = false;						// 是否已经完成第一次设置区域

		ControlBase* m_pParent = nullptr;							// 父控件
		std::list<ControlBase*> m_listChild;						// 子控件

		bool m_bAutoSizeForChild = false;							// 为子控件自动改变大小以容纳控件

		MESSAGE_PROC_FUNC m_funcMessageProc = nullptr;				// 消息处理函数
		MESSAGE_PROC_FUNC_CLASS m_funcMessageProc_Class = nullptr;	// 若绑定的消息处理函数是静态类函数，则记录其地址
		void* m_pCalledClass = nullptr;								// 若绑定的消息处理函数是静态类函数，则记录该类指针

		bool m_bHovered = false;									// 鼠标是否悬停
		bool m_bPressed = false;									// 鼠标是否按下
		bool m_bFocused = false;									// 是否拥有焦点
		
		// 更新区域消息处理
		void UpdateRect() override;
		
		// 重绘子控件
		virtual void RedrawChild();

		// 转换消息
		virtual ExMessage& TransformMessage(ExMessage& msg);

		// 分发消息到用户函数
		virtual void CallUserMsgProcFunc(int msgid, ExMessage msg);

		// 子控件区域更变
		virtual void ChildRectChanged(ControlBase* pChild);

	public:

		ControlBase();

		ControlBase(std::wstring wstrText);

		ControlBase(int x, int y, int w = 0, int h = 0, std::wstring wstrText = L"");

		virtual ~ControlBase();

		ControlBase* GetParent() { return m_pParent; }

		// 设置父控件（内部调用 AddChild）
		virtual void SetParent(ControlBase* p);

		virtual bool isAutoSizeForChild() const { return m_bAutoSizeForChild; }

		// 为子控件自动改变大小以容纳控件（不容纳负坐标部分）
		virtual void EnableAutoSizeForChild(bool enable);

		std::list<ControlBase*>& GetChildList();

		virtual void AddChild(ControlBase* p, int offset_x = 0, int offset_y = 0);

		virtual void RemoveChild(ControlBase* p);

		virtual bool isEnabled() const { return m_bEnabled; }

		virtual void SetEnable(bool enable);

		virtual bool isVisible() const { return m_bVisible; }

		virtual void SetVisible(bool visible);

		virtual Canvas& GetCanavs() { return m_canvas; }

		virtual COLORREF GetBkColor() const { return m_cBackground; }

		virtual void SetBkColor(COLORREF color);

		virtual COLORREF GetTextColor() const { return m_cText; }

		virtual void SetTextColor(COLORREF color);

		virtual void EnableBorder(bool bEnableBorder, COLORREF color = BLACK, int thickness = 1);

		virtual void SetAlpha(BYTE alpha, bool bUseCanvasAlpha, bool isAlphaCalculated);

		virtual std::wstring GetText() const { return m_wstrText; }

		virtual void SetText(std::wstring wstr);

		virtual void Draw_Text(int nTextOffsetX = 0, int nTextOffsetY = 0);

		// 绘制控件（可选绘制子控件）
		virtual void Draw(bool draw_child = true);

		// 渲染控件到外部
		virtual void Render(Canvas* dst);

		// 设置消息响应函数
		virtual void SetMsgProcFunc(MESSAGE_PROC_FUNC func);

		// 设置消息响应函数为静态类函数
		virtual void SetMsgProcFunc(MESSAGE_PROC_FUNC_CLASS static_class_func, void* _this);

		// 更新消息
		virtual void UpdateMessage(ExMessage msg);

		// 是否悬停
		virtual bool isHovered() const { return m_bHovered; }

		// 是否拥有焦点
		virtual bool isFocused() const { return m_bFocused; }

		// 是否按下
		virtual bool isPressed() const { return m_bPressed; }
	};
}

