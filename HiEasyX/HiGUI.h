#pragma once

#include "HiMacro.h"
#include "HiFunc.h"
#include "HiCanvas.h"
#include "HiMouseDrag.h"

#include <ctime>
#include <string>
#include <vector>
#include <list>

namespace HiEasyX
{
	// 基础容器
	class Container
	{
	protected:

		RECT m_rct = {};						// 容器区域

	public:

		Container();

		virtual ~Container();

		virtual void UpdateRect() = 0;			// 更新区域消息响应

		RECT GetRect() const { return m_rct; }

		void SetRect(int x, int y, int w, int h);

		void SetRect(RECT rct);

		POINT GetPos() const { return { m_rct.left,m_rct.top }; }

		int GetX() const { return m_rct.left; }

		int GetY() const { return m_rct.top; }

		void SetPos(int x, int y);

		void SetPos(POINT pt);

		void MoveRel(int dx, int dy);

		int GetWidth() const { return m_rct.right - m_rct.left; }

		void SetWidth(int w);

		int GetHeight() const { return m_rct.bottom - m_rct.top; };

		void SetHeight(int h);

		void Resize(int w, int h);
	};

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

		virtual void UpdateRect();

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

		// 重绘画布
		virtual void Redraw(bool draw_child = true);

		// 渲染控件
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

	// 静态（文本、图像）控件
	class Static : public ControlBase
	{
	public:

		struct Char
		{
			TCHAR ch;
			COLORREF cText;
			COLORREF cBk;
		};

	protected:

		std::vector<Char> m_vecText;
		ImageBlock* m_pImgBlock = nullptr;

	public:

		Static();

		Static(int x, int y, int w, int h, std::wstring wstrText = L"");

		virtual std::vector<Char> Convert(std::wstring wstrText);

		virtual std::wstring Convert(std::vector<Char> vecText);

		virtual std::vector<Char>& GetTextVector() { return m_vecText; }

		virtual void ClearText();

		virtual void AddText(
			std::wstring wstr,
			bool isSetTextColor = false,
			COLORREF cText = 0,
			bool isSetBkColor = false,
			COLORREF cBk = 0
		);

		void SetText(std::wstring wstrText) override;

		void SetText(std::vector<Char> vecText);

		// 获取缓存的图像
		virtual ImageBlock* GetImage() { return m_pImgBlock; }

		// 设置显示图像
		virtual void SetImage(ImageBlock* pImgBlockmg);

		void Draw_Text(int nTextOffsetX = 0, int nTextOffsetY = 0) override;

		void Redraw(bool draw_child = true) override;
	};

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

		COLORREF m_cBorder_Disabled = GRAY;
		COLORREF m_cBackground_Disabled = GRAY;

		COLORREF m_cClassicNormalBorder3D = GRAY;			// 未按下时的 3D 边框颜色（经典样式）
		COLORREF m_cClassicPressedBorder3D = LIGHTGRAY;		// 按下时的 3D 边框颜色（经典样式）

		Button();

		Button(int x, int y, int w, int h, std::wstring wstrText = L"");

		void SetEnable(bool enable) override;

		virtual void EnableClassicStyle(bool enable);

		void UpdateMessage(ExMessage msg) override;

		void Redraw(bool draw_child = true) override;
	};

	// 进度条控件
	class ProgressCtrl : public ControlBase
	{
	protected:

		bool m_bEnableAnimation = true;					// 是否启用动画效果

		int m_nLightPos = 0;							// 光源位置
		clock_t m_tClock = 0;							// 保留上一次动画的绘制时间
		int m_nSpeed = 30;								// 动画速度（像素每秒）

		float m_fLenRatio = 0.8f;						// 动画光亮效果长度占比

		float m_fBarColorLightness = 0.35f;				// 进度条颜色亮度
		float m_fBarLightLightness = 0.41f;				// 进度条光源亮度

		COLORREF m_cBar;								// 进度条颜色（不用于亮度采样）

		float m_fH = 0;									// 色相信息
		float m_fS = 0;									// 饱和度信息

		int m_nPos = 0;									// 进度条进度
		int m_nLen = 100;								// 进度总长度

	public:

		ProgressCtrl();

		ProgressCtrl(int x, int y, int w, int h, int len);

		// 获取总长度
		virtual int GetLen() const { return m_nLen; }

		virtual void SetLen(int len);					// 设置总长度

		// 获取进度
		virtual int GetProcess() const { return m_nPos; }

		virtual void SetProcess(int pos);				// 设置进度

		virtual void Step();							// 进度加一

		// 获取进度条颜色
		virtual COLORREF GetBarColor() const { return m_cBar; }

		virtual void SetBarColor(COLORREF cBar);		// 设置进度条颜色

		// 获取动画启用状态
		virtual bool GetAnimationState() const { return m_bEnableAnimation; }

		// 设置是否启用动画
		virtual void EnableAnimation(bool enable);

		void Redraw(bool draw_child = true) override;
	};

	// 滚动条控件
	class ScrollBar : public ControlBase
	{
	public:

		// 滚动条信息
		struct ScrollBarInfo
		{
			int btnW, btnH;			// 普通按钮尺寸
			int slider_free_len;	// 滑块可以占据的像素长度
			int slider_move_len;	// 滑块顶点可以移动的像素长度
			int slider_len;			// 滑块像素长度
		};

	protected:

		int m_nBtnHeight = 20;				// 按钮高度（竖直放置时有效）
		int m_nBtnWidth = 20;				// 按钮宽度（水平放置时有效）

		bool m_bHorizontal = false;			// 是否水平放置

		int m_nDrawInterval = 7;			// 绘制间隙

		// 按钮
		Button m_btnTop;
		Button m_btnUp;
		Button m_btnDown;
		Button m_btnBottom;
		Button m_btnDrag;
		MouseDrag m_MouseDrag;
		bool m_bDragging = false;			// 是否正在拖动
		int m_nSliderSpeed = 20;			// 按住按钮时滑块每秒运行的内容长度
		clock_t m_tPressed = 0;				// 按钮按住计时

		float m_fPos = 0;					// 滑块内容位置
		int m_nLen = 0;						// 内容总长度
		float m_fPosRatio = 0;				// 滑块内容位置比例

		ScrollBarInfo m_info = {};			// 滚动条信息

		int m_nViewLen = 10;				// 视野内容长度
		float m_fViewRatio = 1;				// 视野范围占总长度的比

		bool m_bSliderPosChanged = false;	// 标记滑块位置改变

		RECT m_rctOnWheel = { 0 };			// 响应滚轮消息的区域（客户区坐标）
		bool m_bSetOnWheelRct = false;		// 是否设置了滚轮消息的响应区域

		// 初始化
		virtual void Init();

		// 响应按钮消息
		static void OnButtonMsg(void* pThis, ControlBase* pCtrl, int msgid, ExMessage msg);

		// 更新滑块位置区域
		virtual void UpdateSliderRect();

		// 更新位置比例
		void UpdatePosRatio();

		// 更新视野比例
		void UpdateViewRatio();

		// 更新滚动条信息
		void UpdateScrollBarInfo();

	public:

		ScrollBar();

		ScrollBar(int x, int y, int w, int h, int len, int pos, bool bHorizontal = false);

		int GetButtonHeight() const { return m_nBtnHeight; }

		// 设置按钮高度（竖直放置时生效）
		void SetButtonHeight(int h);

		int GetButtonWidth() const { return m_nBtnWidth; }

		// 设置按钮宽度（水平放置时生效）
		void SetButtonWidth(int w);

		// 是否正在拖动
		bool isDragging() const { return m_bDragging; }

		// 获取滑块内容位置
		int GetSliderContentPos() const { return (int)m_fPos; }

		// 设置滑块内容位置
		void SetSliderContentPos(float pos);

		// 移动滑块的内容位置
		void MoveSlider(float d);

		// 获取滚动条内容长度
		int GetContentLength() const { return m_nLen; }

		// 设置滚动条内容长度
		void SetContentLength(int len);

		int GetViewLength() const { return m_nViewLen; }

		// 设置视野内容长度
		void SetViewLength(int len);

		int GetSliderSpeed() const { return m_nSliderSpeed; }

		// 设置按下按钮时滑块的运行速度（每秒经过的内容长度）
		void SetSliderSpeed(int speed);

		bool isHorizontal() const { return m_bHorizontal; }

		// 启用水平放置
		void EnableHorizontal(bool enable);

		// 滑块位置是否改变
		bool isSliderPosChanged();

		// 获取响应滚轮消息的区域（未自定义时返回空区域）
		RECT GetOnWheelRect() const { return m_rctOnWheel; }

		// 设置响应滚轮消息的区域（客户区坐标）
		void SetOnWheelRect(RECT rct);

		void UpdateRect() override;

		void UpdateMessage(ExMessage msg) override;

		void Redraw(bool draw_child = true) override;

	};

	class test : public ScrollBar
	{
	protected:
		static void OnButtonMsg(void* pThis, ControlBase* pCtrl, int msgid, ExMessage msg);
	};

	//class EditBox
	//{
	//public:
	//
	//	RECT m_rct;
	//
	//	COLORREF m_cLine;
	//	COLORREF m_cBackground;
	//
	//
	//	void Create(RECT rct)
	//	{
	//		m_rct = rct;
	//	}
	//
	//	void Show()
	//	{
	//		setlinecolor(m_cLine);
	//		setfillcolor(m_cBackground);
	//		fillrectangle(m_rct.left, m_rct.top, m_rct.right, m_rct.bottom);
	//
	//	}
	//
	//};

};
