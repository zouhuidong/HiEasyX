#include "HiGdiplus.h"
#include "HiMacro.h"

#pragma comment(lib, "gdiplus.lib")

bool							g_bStarup = false;
Gdiplus::GdiplusStartupInput	g_Input;
ULONG_PTR						g_Token;

namespace HiEasyX
{
	void Gdiplus_Starup()
	{
		if (!g_bStarup)
		{
			Gdiplus::GdiplusStartup(&g_Token, &g_Input, nullptr);
			g_bStarup = true;
		}
	}

	void Gdiplus_Shutdown()
	{
		if (g_bStarup)
		{
			Gdiplus::GdiplusShutdown(g_Token);
			g_bStarup = false;
		}
	}

	Gdiplus::Color ConvertToGdiplusColor(COLORREF color, bool reserve_alpha)
	{
		return Gdiplus::Color(
			reserve_alpha ? GetAValue(color) : 255,
			GetRValue(color),
			GetGValue(color),
			GetBValue(color)
		);
	}

	void Gdiplus_Line(
		HDC hdc,
		float x1,
		float y1,
		float x2,
		float y2,
		Gdiplus::Color color,
		float width,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(color, width);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawLine(&pen, x1, y1, x2, y2);
	}

	void Gdiplus_Polygon(
		HDC hdc,
		int points_num,
		Gdiplus::PointF* points,
		Gdiplus::Color color,
		float width,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(color, width);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawLines(&pen, points, points_num);
	}

	void Gdiplus_Rectangle(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		Gdiplus::Color color,
		float width,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(color, width);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawRectangle(&pen, x, y, w, h);
	}

	void Gdiplus_Ellipse(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		Gdiplus::Color color,
		float width,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(color, width);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawEllipse(&pen, x, y, w, h);
	}

	void Gdiplus_Pie(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		Gdiplus::Color color,
		float width,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(color, width);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawPie(&pen, x, y, w, h, stangle, endangle);
	}
};

