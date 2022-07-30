#include "Container.h"

namespace HiEasyX
{
	Container::Container()
	{
	}

	Container::~Container()
	{
	}

	void Container::UpdateRect()
	{
		if (m_rct.bottom < m_rct.top) m_rct.bottom = m_rct.top;
		if (m_rct.right < m_rct.left) m_rct.right = m_rct.left;
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
}
