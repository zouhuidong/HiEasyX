////////////////////////////////////
//
//	ScrollBar.h
//	HiGUI 控件分支：滚动条控件
//

#pragma once

#include "Button.h"

#include "../HiMouseDrag.h"

#include <time.h>

namespace HiEasyX
{
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

		int m_nDrawInterval = 6;			// 绘制间隙

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

		// 获取滑块像素长度
		int GetSliderLength() const { return m_info.slider_len; }

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

		void Draw(bool draw_child = true) override;

	};
}

