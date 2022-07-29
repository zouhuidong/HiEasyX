#include "HiGUI.h"

#include "HiCanvas.h"

namespace HiEasyX
{
	Container::Container()
	{
	}

	Container::~Container()
	{
	}

	void Container::SetRect(RECT rct)
	{
		m_rct = rct;
		UpdateRect();
	}

	void Container::SetRect(int x, int y, int w, int h)
	{
		m_rct = { x,y,x + w,y + h };
		UpdateRect();
	}

	void Container::SetPos(int x, int y)
	{
		m_rct = { x,y,x + GetWidth(),y + GetHeight() };
		UpdateRect();
	}

	void Container::SetPos(POINT pt)
	{
		m_rct = { pt.x,pt.y,pt.x + GetWidth(),pt.y + GetHeight() };
		UpdateRect();
	}

	void Container::SetWidth(int w)
	{
		m_rct.right = m_rct.left + w;
		UpdateRect();
	}

	void Container::SetHeight(int h)
	{
		m_rct.bottom = m_rct.top + h;
		UpdateRect();
	}

	void Container::Resize(int w, int h)
	{
		m_rct.right = m_rct.left + w;
		m_rct.bottom = m_rct.top + h;
		UpdateRect();
	}

	void Container::MoveRel(int dx, int dy)
	{
		SetPos({ m_rct.left + dx,m_rct.top + dy });
	}

	ControlBase::ControlBase()
	{
	}

	ControlBase::ControlBase(std::wstring wstrText)
	{
		SetText(wstrText);
	}

	ControlBase::ControlBase(int x, int y, int w, int h, std::wstring wstrText)
	{
		SetRect(x, y, w, h);
		SetText(wstrText);
	}

	ControlBase::~ControlBase()
	{
	}

	void ControlBase::UpdateRect()
	{
		if (m_rct.bottom < m_rct.top) m_rct.bottom = m_rct.top;
		if (m_rct.right < m_rct.left) m_rct.right = m_rct.left;

		m_canvas.Resize(GetWidth(), GetHeight());

		if (!m_bCompleteFirstSetRect)
		{
			// TODO：等 EasyX 修复后加上这句
			//m_canvas.SetTypeface(L"Arial");
			m_bCompleteFirstSetRect = true;
		}

		if (m_pParent)
		{
			m_pParent->ChildRectChanged(this);
		}
	}

	void ControlBase::SetEnable(bool enable)
	{
		m_bEnabled = enable;
	}

	void ControlBase::SetParent(ControlBase* p)
	{
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		if (p)
		{
			m_pParent = p;
			p->AddChild(this);
		}
	}

	void ControlBase::EnableAutoSizeForChild(bool enable)
	{
		m_bAutoSizeForChild = enable;
	}

	std::list<ControlBase*>& ControlBase::GetChildList()
	{
		return m_listChild;
	}

	void ControlBase::AddChild(ControlBase* p, int offset_x, int offset_y)
	{
		for (auto& child : m_listChild)
			if (child == p)
				return;
		p->MoveRel(offset_x, offset_y);
		p->m_pParent = this;
		m_listChild.push_back(p);
		ChildRectChanged(p);
	}

	void ControlBase::RemoveChild(ControlBase* p)
	{
		m_listChild.remove(p);
	}

	void ControlBase::SetVisible(bool bVisible)
	{
		m_bVisible = bVisible;
	}

	void ControlBase::SetBkColor(COLORREF color)
	{
		m_cBackground = color;
	}

	void ControlBase::SetTextColor(COLORREF color)
	{
		m_cText = color;
	}

	void ControlBase::EnableBorder(bool bEnableBorder, COLORREF color, int thickness)
	{
		m_bEnableBorder = bEnableBorder;
		if (bEnableBorder)
		{
			m_cBorder = color;
			m_nBorderThickness = thickness;
		}
	}

	void ControlBase::SetAlpha(BYTE alpha, bool bUseCanvasAlpha, bool isAlphaCalculated)
	{
		m_alpha = alpha;
		m_bUseCanvasAlpha = bUseCanvasAlpha;
		m_isAlphaCalculated = isAlphaCalculated;
	}

	void ControlBase::SetText(std::wstring wstr)
	{
		m_wstrText = wstr;
	}

	void ControlBase::Draw_Text(int nTextOffsetX, int nTextOffsetY)
	{
		m_canvas.SetBkColor(m_cBackground);
		m_canvas.SetTextColor(m_cText);
		int w = m_canvas.TextWidth(m_wstrText.c_str());
		int h = m_canvas.TextHeight(m_wstrText.c_str());
		m_canvas.OutTextXY(
			(GetWidth() - w) / 2 + nTextOffsetX,
			(GetHeight() - h) / 2 + nTextOffsetY,
			m_wstrText.c_str()
		);
	}

	void ControlBase::Redraw(bool draw_child)
	{
		m_canvas.SetLineThickness(m_nBorderThickness);
		m_canvas.FillRectangle(
			0, 0, GetWidth() - 1, GetHeight() - 1,
			true, m_bEnableBorder ? m_cBorder : m_cBackground, m_cBackground
		);

		if (draw_child)
		{
			RedrawChild();
		}
	}

	void ControlBase::RedrawChild()
	{
		for (auto& child : m_listChild)
		{
			child->Redraw();
		}
	}

	void ControlBase::Render(Canvas* dst)
	{
		if (m_bVisible)
		{
			for (auto& child : m_listChild)
			{
				child->Render(&m_canvas);
			}

			dst->PutImage_Alpha(
				m_rct.left, m_rct.top,
				&m_canvas,
				m_alpha, m_bUseCanvasAlpha, m_isAlphaCalculated
			);
		}
	}

	void ControlBase::SetMsgProcFunc(MESSAGE_PROC_FUNC func)
	{
		m_funcMessageProc = func;
		m_funcMessageProc_Class = nullptr;
		m_pCalledClass = nullptr;
	}

	void ControlBase::SetMsgProcFunc(MESSAGE_PROC_FUNC_CLASS static_class_func, void* _this)
	{
		m_funcMessageProc = nullptr;
		m_funcMessageProc_Class = static_class_func;
		m_pCalledClass = _this;
	}

	ExMessage& ControlBase::TransformMessage(ExMessage& msg)
	{
		switch (GetExMessageType(msg))
		{
		case EM_MOUSE:
			msg.x -= GetX();
			msg.y -= GetY();
			break;
		default:
			break;
		}
		return msg;
	}

	void ControlBase::CallUserMsgProcFunc(int msgid, ExMessage msg)
	{
		if (m_funcMessageProc)
		{
			m_funcMessageProc(this, msgid, msg);
		}
		else if (m_funcMessageProc_Class)
		{
			m_funcMessageProc_Class(m_pCalledClass, this, msgid, msg);
		}
	}

	void ControlBase::ChildRectChanged(ControlBase* pChild)
	{
		if (m_bAutoSizeForChild)
		{
			int _w = GetWidth(), _h = GetHeight();
			if (pChild->m_rct.right > _w)		_w += pChild->m_rct.right - _w;
			if (pChild->m_rct.bottom > _h)		_h += pChild->m_rct.bottom - _h;
			Resize(_w, _h);
		}
	}

	void ControlBase::UpdateMessage(ExMessage msg)
	{
		if (m_bEnabled)
		{
			TransformMessage(msg);

			if (isInRect(msg.x, msg.y, { 0,0,GetWidth(),GetHeight() }))
			{
				if (!m_bHovered)
				{
					m_bHovered = true;
					CallUserMsgProcFunc(CM_HOVER, msg);
				}

				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					m_bPressed = true;
					CallUserMsgProcFunc(CM_PRESS, msg);
					if (!m_bFocused)
					{
						m_bFocused = true;
						CallUserMsgProcFunc(CM_FOCUS, msg);
					}
					break;
				case WM_LBUTTONUP:
					m_bPressed = false;
					if (m_bFocused)
					{
						CallUserMsgProcFunc(CM_PRESS_OVER, msg);
						CallUserMsgProcFunc(CM_CLICK, msg);
					}
					break;
				case WM_LBUTTONDBLCLK:
					CallUserMsgProcFunc(CM_DOUBLE_CLICK, msg);
				default:
					CallUserMsgProcFunc(CM_OTHER, msg);
					break;
				}
			}
			else
			{
				if (m_bHovered)
				{
					m_bHovered = false;
					CallUserMsgProcFunc(CM_HOVER_OVER, msg);
				}

				if (m_bPressed)
				{
					m_bPressed = false;
					CallUserMsgProcFunc(CM_PRESS_OVER, msg);
				}

				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					if (m_bFocused)
					{
						m_bFocused = false;
						CallUserMsgProcFunc(CM_FOCUS_OVER, msg);
					}
					break;
				default:
					CallUserMsgProcFunc(CM_OTHER, msg);
					break;
				}
			}

			for (auto& child : m_listChild)
			{
				child->UpdateMessage(msg);
			}
		}
	}

	Button::Button()
	{
		InitColor();
	}

	Button::Button(int x, int y, int w, int h, std::wstring wstrText)
	{
		SetRect(x, y, w, h);
		SetText(wstrText);
		InitColor();
	}

	void Button::SetEnable(bool enable)
	{
		ControlBase::SetEnable(enable);
		if (!enable)
		{
			m_cBorder = m_cBorder_Disabled;
			m_cBackground = m_cBackground_Disabled;
		}
		else
		{
			InitColor();
		}
	}

	void Button::UpdateMessage(ExMessage msg)
	{
		if (m_bEnabled)
		{
			ControlBase::UpdateMessage(msg);

			if (!m_bEnableClassicStyle)
			{
				if (m_bPressed)
				{
					m_cBorder = m_cBorder_Pressed;
					m_cBackground = m_cBackground_Pressed;
				}
				else if (m_bHovered)
				{
					m_cBorder = m_cBorder_Hovered;
					m_cBackground = m_cBackground_Hovered;
				}
				else
				{
					InitColor();
				}
			}
		}
	}

	void Button::EnableClassicStyle(bool enable)
	{
		m_bEnableClassicStyle = enable;
		if (enable)
		{
			m_cBackground = CLASSICGRAY;
		}
	}

	void Button::Redraw(bool draw_child)
	{
		ControlBase::Redraw(false);

		if (m_bEnableClassicStyle)
		{
			if (m_bPressed)
			{
				m_canvas.SetLineColor(m_cClassicPressedBorder3D);
				m_canvas.Line(1, 1, 1, GetHeight() - 2);
				m_canvas.Line(1, 1, GetWidth() - 2, 1);
				Draw_Text(1, 1);
			}
			else
			{
				m_canvas.SetLineColor(m_cClassicNormalBorder3D);
				m_canvas.Line(GetWidth() - 2, 1, GetWidth() - 2, GetHeight() - 2);
				m_canvas.Line(1, GetHeight() - 2, GetWidth() - 2, GetHeight() - 2);
				Draw_Text();
			}
		}
		else
		{
			Draw_Text();
		}

		if (draw_child)
		{
			RedrawChild();
		}
	}

	Static::Static()
	{
		m_bEnableBorder = false;
	}

	Static::Static(int x, int y, int w, int h, std::wstring wstrText)
	{
		*this = Static();
		SetRect(x, y, w, h);
		SetText(wstrText);
	}

	std::vector<Static::Char> Static::Convert(std::wstring wstrText)
	{
		std::vector<Static::Char> vec;
		for (auto& ch : wstrText)
		{
			vec.push_back({ ch,m_cText,m_cBackground });
		}

		return vec;
	}

	std::wstring Static::Convert(std::vector<Char> vecText)
	{
		std::wstring wstr;
		for (auto& ch : vecText)
		{
			wstr.push_back(ch.ch);
		}

		return wstr;
	}

	void Static::ClearText()
	{
		m_wstrText.clear();
		m_vecText.clear();
	}

	void Static::AddText(std::wstring wstr, bool isSetTextColor, COLORREF cText, bool isSetBkColor, COLORREF cBk)
	{
		m_wstrText += wstr;
		if (!isSetTextColor)	cText = m_cText;
		if (!isSetBkColor)		cBk = m_cBackground;
		for (auto& ch : wstr)
		{
			m_vecText.push_back({ ch,cText,cBk });
		}
	}

	void Static::SetText(std::wstring wstrText)
	{
		m_wstrText = wstrText;
		m_vecText = Convert(wstrText);
	}

	void Static::SetText(std::vector<Char> vecText)
	{
		m_wstrText = Convert(vecText);
		m_vecText = vecText;
	}

	void Static::Draw_Text(int nTextOffsetX, int nTextOffsetY)
	{
		int w = m_canvas.TextWidth(m_wstrText.c_str());
		int h = m_canvas.TextHeight(m_wstrText.c_str());

		m_canvas.MoveTo(
			(GetWidth() - w) / 2 + nTextOffsetX,
			(GetHeight() - h) / 2 + nTextOffsetY
		);

		for (auto& ch : m_vecText)
		{
			m_canvas.SetBkColor(ch.cBk);
			m_canvas.OutText(ch.ch, true, ch.cText);
		}

		m_canvas.SetBkColor(m_cBackground);
	}

	void Static::Redraw(bool draw_child)
	{
		ControlBase::Redraw(false);
		if (m_pImgBlock)
		{
			m_canvas.PutImage_Alpha(
				m_pImgBlock->x, m_pImgBlock->y,
				m_pImgBlock->GetCanvas(),
				m_pImgBlock->alpha, m_pImgBlock->bUseSrcAlpha, m_pImgBlock->isAlphaCalculated
			);
		}
		Draw_Text();

		if (draw_child)
		{
			RedrawChild();
		}
	}

	void Static::SetImage(ImageBlock* pImgBlock)
	{
		m_pImgBlock = pImgBlock;
	}

	ProgressCtrl::ProgressCtrl()
	{
		SetBarColor(GREEN);
	}

	ProgressCtrl::ProgressCtrl(int x, int y, int w, int h, int len)
	{
		m_nLen = len;
		SetRect(x, y, w, h);
		SetBarColor(GREEN);
	}

	void ProgressCtrl::SetLen(int len)
	{
		m_nLen = len;
	}

	void ProgressCtrl::SetProcess(int pos)
	{
		m_nPos = pos;
	}

	void ProgressCtrl::Step()
	{
		if (m_nPos + 1 <= m_nLen)
		{
			m_nPos++;
		}
	}

	void ProgressCtrl::SetBarColor(COLORREF cBar)
	{
		float h, s, l;
		RGBtoHSL(cBar, &h, &s, &l);
		m_fH = h;
		m_fS = s;
		m_cBar = HSLtoRGB(h, s, m_fBarColorLightness);
	}

	void ProgressCtrl::EnableAnimation(bool enable)
	{
		m_bEnableAnimation = enable;
	}

	void ProgressCtrl::Redraw(bool draw_child)
	{
		ControlBase::Redraw(false);

		// 绘制结束位置
		int nEnd = (int)((m_nPos / (float)m_nLen) * (GetWidth() - 2));

		m_canvas.SolidRectangle(
			{ 1,1,nEnd,GetHeight() - 2 },
			true, m_cBar
		);

		// 绘制动画效果
		if (m_bEnableAnimation)
		{
			// 动画进度
			if (m_tClock == 0)	m_tClock = clock();
			float fProcess = (float)((clock() - m_tClock) / (nEnd / (float)m_nSpeed * 1000));
			if (fProcess >= 1)
			{
				fProcess = 0;
				m_tClock = clock();
			}

			int nAnimationLen = (int)(m_nPos / (float)m_nLen * GetWidth() * m_fLenRatio);		// 动画效果长度
			int nAnimationLenHalf = nAnimationLen / 2;										// 动画效果半长
			int draw_pos = (int)((nEnd + nAnimationLen) * fProcess) - nAnimationLenHalf;	// 光源绘制 x 坐标
			for (int i = -nAnimationLenHalf; i < nAnimationLenHalf; i++)
			{
				int pos = i + draw_pos;		// 当前绘制 x 坐标

				// 当前绘制亮度
				float l = (m_fBarColorLightness - m_fBarLightLightness) * (abs(i) / (float)nAnimationLenHalf) + m_fBarLightLightness;
				if (pos >= 1 && pos <= nEnd)
				{
					m_canvas.Line(
						pos, 1, pos, GetHeight() - 2,
						true, HSLtoRGB(m_fH, m_fS, l)
					);
				}
			}
		}

		if (draw_child)
		{
			RedrawChild();
		}
	}

	void ScrollBar::OnButtonMsg(void* pThis, ControlBase* pCtrl, int msgid, ExMessage msg)
	{
		ScrollBar* _this = (ScrollBar*)pThis;

		switch (msgid)
		{
		case CM_CLICK:
			if (pCtrl == &_this->m_btnTop)
			{
				_this->SetSliderContentPos(0);
			}
			else if (pCtrl == &_this->m_btnBottom)
			{
				_this->SetSliderContentPos(_this->GetContentLength());
			}
			break;
		}
	}

	void ScrollBar::Init()
	{
		m_btnTop.SetMsgProcFunc(OnButtonMsg, this);
		m_btnUp.SetMsgProcFunc(OnButtonMsg, this);
		m_btnDown.SetMsgProcFunc(OnButtonMsg, this);
		m_btnBottom.SetMsgProcFunc(OnButtonMsg, this);
		m_btnDrag.SetMsgProcFunc(OnButtonMsg, this);

		AddChild(&m_btnTop);
		AddChild(&m_btnUp);
		AddChild(&m_btnDown);
		AddChild(&m_btnBottom);
		AddChild(&m_btnDrag);

		UpdateScrollBarInfo();
	}

	void ScrollBar::UpdateSliderRect()
	{
		m_btnDrag.SetRect(2, (int)(m_info.btnH * 2 + m_info.slider_move_len * m_fPosRatio), m_info.btnW - 4, m_info.slider_len);
	}

	void ScrollBar::UpdatePosRatio()
	{
		if (m_nLen)
		{
			m_fPosRatio = m_fPos / m_nLen;
		}
		else
		{
			m_fPosRatio = 1;
		}
	}

	void ScrollBar::UpdateViewRatio()
	{
		if (m_nLen)
		{
			m_fViewRatio = m_nViewLen / (float)m_nLen;
		}
		else
		{
			m_fViewRatio = 1;
		}
		UpdateScrollBarInfo();
	}

	void ScrollBar::UpdateScrollBarInfo()
	{
		m_info.btnW = GetWidth();
		m_info.btnH = m_nBtnHeight;

		m_info.slider_free_len = GetHeight() - m_info.btnH * 4;
		m_info.slider_len = (int)(m_info.slider_free_len * m_fViewRatio);
		m_info.slider_move_len = m_info.slider_free_len - m_info.slider_len;
	}

	void ScrollBar::SetButtonHeight(int h)
	{
		m_nBtnHeight = h;
	}

	void ScrollBar::SetButtonWidth(int w)
	{
		m_nBtnWidth = w;
	}

	ScrollBar::ScrollBar()
	{
		Init();
	}

	ScrollBar::ScrollBar(int x, int y, int w, int h, int len, int pos, bool bHorizontal)
	{
		SetRect(x, y, w, h);
		Init();
	}

	void ScrollBar::SetSliderContentPos(float pos)
	{
		if (pos < 0)
		{
			m_fPos = 0;
		}
		else if (pos > m_nLen)
		{
			m_fPos = m_nLen;
		}
		else
		{
			m_fPos = pos;
		}
		UpdatePosRatio();
		UpdateSliderRect();
	}

	void ScrollBar::MoveSlider(float d)
	{
		SetSliderContentPos(m_fPos + d);
	}

	void ScrollBar::SetContentLength(int len)
	{
		m_nLen = len;
		UpdatePosRatio();
		UpdateViewRatio();
		UpdateSliderRect();
	}

	void ScrollBar::SetViewLength(int len)
	{
		m_nViewLen = len;
		UpdateViewRatio();
		UpdateSliderRect();
	}

	void ScrollBar::SetSliderSpeed(int speed)
	{
		m_nSliderSpeed = speed;
	}

	void ScrollBar::UpdateRect()
	{
		ControlBase::UpdateRect();

		UpdateScrollBarInfo();

		m_btnTop.SetRect(0, 0, m_info.btnW, m_info.btnH);
		m_btnUp.SetRect(0, m_info.btnH, m_info.btnW, m_info.btnH);
		m_btnDown.SetRect(0, GetHeight() - m_info.btnH * 2, m_info.btnW, m_info.btnH);
		m_btnBottom.SetRect(0, GetHeight() - m_info.btnH, m_info.btnW, m_info.btnH);

		UpdateSliderRect();
	}

	void ScrollBar::UpdateMessage(ExMessage msg)
	{
		if (m_bEnabled)
		{
			ControlBase::UpdateMessage(msg);
			
			// 此处响应消息无需转换
			//TransformMessage(msg);

			// 检查拖动状态
			m_MouseDrag.UpdateMessage(msg);
			if (m_btnDrag.isPressed())
			{
				m_bDragging = true;
			}
			else if (msg.message == WM_LBUTTONUP)
			{
				m_bDragging = false;
			}

			// 拖动消息
			if (m_bDragging && m_MouseDrag.isLeftDrag())
			{
				if (m_info.slider_move_len)
				{
					MoveSlider(m_MouseDrag.GetDragY() / (float)m_info.slider_move_len * m_nLen);
				}
			}

			// 滚轮消息响应区域
			RECT rctWheel = m_bSetOnWheelRct ? m_rctOnWheel : m_rct;
			if (isInRect(msg.x, msg.y, rctWheel) || m_bDragging)
			{
				if (msg.wheel)
				{
					MoveSlider(-msg.wheel / 120 * 3);
				}
			}
		}
	}

	void ScrollBar::Redraw(bool draw_child)
	{
		ControlBase::Redraw();

		// 按钮
		clock_t tNow = clock();
		float move_len = (float)(tNow - m_tPressed) / CLOCKS_PER_SEC * m_nSliderSpeed;
		if (m_btnUp.isPressed())
		{
			if (m_tPressed != 0)
			{
				MoveSlider(-move_len);
			}
			m_tPressed = tNow;
		}
		else if (m_btnDown.isPressed())
		{
			if (m_tPressed != 0)
			{
				MoveSlider(move_len);
			}
			m_tPressed = tNow;
		}
		else
		{
			m_tPressed = 0;
		}

		Canvas& canvasTop = m_btnTop.GetCanavs();
		Canvas& canvasUp = m_btnUp.GetCanavs();
		Canvas& canvasDown = m_btnDown.GetCanavs();
		Canvas& canvasBottom = m_btnBottom.GetCanavs();

		int bottom_y = m_nBtnHeight - m_nDrawInterval;
		int middle_x = GetWidth() / 2;
		int right_x = GetWidth() - m_nDrawInterval;

		canvasTop.SetLineColor(BLACK);
		canvasTop.MoveTo(m_nDrawInterval, bottom_y);
		canvasTop.LineTo(middle_x, m_nDrawInterval);
		canvasTop.LineTo(right_x, bottom_y);
		canvasTop.Line(m_nDrawInterval, m_nDrawInterval, right_x, m_nDrawInterval);

		canvasUp.SetLineColor(BLACK);
		canvasUp.MoveTo(m_nDrawInterval, bottom_y);
		canvasUp.LineTo(middle_x, m_nDrawInterval);
		canvasUp.LineTo(right_x, bottom_y);

		canvasDown.SetLineColor(BLACK);
		canvasDown.MoveTo(m_nDrawInterval, m_nDrawInterval);
		canvasDown.LineTo(middle_x, bottom_y);
		canvasDown.LineTo(right_x, m_nDrawInterval);

		canvasBottom.SetLineColor(BLACK);
		canvasBottom.MoveTo(m_nDrawInterval, m_nDrawInterval);
		canvasBottom.LineTo(middle_x, bottom_y);
		canvasBottom.LineTo(right_x, m_nDrawInterval);
		canvasBottom.Line(m_nDrawInterval, bottom_y, right_x, bottom_y);

	}

	bool ScrollBar::isSliderPosChanged()
	{
		bool r = m_bSliderPosChanged;
		m_bSliderPosChanged = false;
		return m_bSliderPosChanged;
	}

	void ScrollBar::SetOnWheelRect(RECT rct)
	{
		m_rctOnWheel = rct;
		m_bSetOnWheelRct = true;
	}

	void ScrollBar::EnableHorizontal(bool enable)
	{
		m_bHorizontal = enable;
	}

	void test::OnButtonMsg(void* pThis, ControlBase* pCtrl, int msgid, ExMessage msg)
	{

	}

};
