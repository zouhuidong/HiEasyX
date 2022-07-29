#include "Static.h"

namespace HiEasyX
{
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
}
