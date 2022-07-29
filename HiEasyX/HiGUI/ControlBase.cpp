#include "ControlBase.h"

namespace HiEasyX
{
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
		if (m_bVisible && m_bEnabled)
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
}
