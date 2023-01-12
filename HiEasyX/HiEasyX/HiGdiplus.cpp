#include "HiGdiplus.h"
#include "HiMacro.h"

#pragma comment(lib, "gdiplus.lib")

bool							g_bStarup = false;
Gdiplus::GdiplusStartupInput	g_Input;
ULONG_PTR						g_Token;

namespace HiEasyX
{
	void Gdiplus_Try_Starup()
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
		Gdiplus::Color linecolor,
		float linewidth,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(linecolor, linewidth);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawLine(&pen, x1, y1, x2, y2);
	}

	void Gdiplus_Polygon(
		HDC hdc,
		int points_num,
		Gdiplus::PointF* points,
		Gdiplus::Color linecolor,
		float linewidth,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(linecolor, linewidth);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawLines(&pen, points, points_num);
	}

	void Gdiplus_SolidPolygon(
		HDC hdc,
		int points_num,
		Gdiplus::PointF* points,
		Gdiplus::Color fillcolor,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush brush(fillcolor);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.FillPolygon(&brush, points, points_num);
	}

	void Gdiplus_Rectangle(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		Gdiplus::Color linecolor,
		float linewidth,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(linecolor, linewidth);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawRectangle(&pen, x, y, w, h);
	}

	void Gdiplus_SolidRectangle(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		Gdiplus::Color fillcolor,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush brush(fillcolor);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.FillRectangle(&brush, x, y, w, h);
	}

	void Gdiplus_Ellipse(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		Gdiplus::Color linecolor,
		float linewidth,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(linecolor, linewidth);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawEllipse(&pen, x, y, w, h);
	}

	void Gdiplus_SolidEllipse(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		Gdiplus::Color fillcolor,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush brush(fillcolor);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.FillEllipse(&brush, x, y, w, h);
	}

	void Gdiplus_Pie(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		Gdiplus::Color linecolor,
		float linewidth,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(linecolor, linewidth);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawPie(&pen, x, y, w, h, stangle, endangle);
	}

	void Gdiplus_SolidPie(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		Gdiplus::Color fillcolor,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush brush(fillcolor);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.FillPie(&brush, x, y, w, h, stangle, endangle);
	}

	void Gdiplus_Arc(
		HDC hdc,
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		Gdiplus::Color linecolor,
		float linewidth,
		Gdiplus::SmoothingMode smoothing_mode
	)
	{
		Gdiplus_Try_Starup();
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Pen pen(linecolor, linewidth);

		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(smoothing_mode);
		graphics.DrawArc(&pen, x, y, w, h, stangle, endangle);
	}

	void EasyX_Gdiplus_Line(
		float x1,
		float y1,
		float x2,
		float y2,
		COLORREF linecolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_Line(
			GetImageHDC(pImg),
			x1, y1, x2, y2,
			ConvertToGdiplusColor(linecolor, enable_alpha),
			linewidth,
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}

	void EasyX_Gdiplus_Polygon(
		int points_num,
		POINT* points,
		COLORREF linecolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		// 转换点的类型
		Gdiplus::PointF* pPts = new Gdiplus::PointF[points_num];
		for (int i = 0; i < points_num; i++)
		{
			pPts[i].X = (float)points[i].x;
			pPts[i].Y = (float)points[i].y;
		}

		Gdiplus_Polygon(
			GetImageHDC(pImg),
			points_num,
			pPts,
			ConvertToGdiplusColor(linecolor, enable_alpha),
			linewidth,
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);

		delete[] pPts;
	}

	void EasyX_Gdiplus_SolidPolygon(
		int points_num,
		POINT* points,
		COLORREF fillcolor,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		// 转换点的类型
		Gdiplus::PointF* pPts = new Gdiplus::PointF[points_num];
		for (int i = 0; i < points_num; i++)
		{
			pPts[i].X = (float)points[i].x;
			pPts[i].Y = (float)points[i].y;
		}

		Gdiplus_SolidPolygon(
			GetImageHDC(pImg),
			points_num,
			pPts,
			ConvertToGdiplusColor(fillcolor, enable_alpha),
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);

		delete[] pPts;
	}

	void EasyX_Gdiplus_FillPolygon(
		int points_num,
		POINT* points,
		COLORREF linecolor,
		COLORREF fillcolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		EasyX_Gdiplus_SolidPolygon(points_num, points, fillcolor, enable_alpha, enable_aa, pImg);
		EasyX_Gdiplus_Polygon(points_num, points, linecolor, linewidth, enable_alpha, enable_aa, pImg);
	}

	void EasyX_Gdiplus_Rectangle(
		float x,
		float y,
		float w,
		float h,
		COLORREF linecolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_Rectangle(
			GetImageHDC(pImg),
			x, y, w, h,
			ConvertToGdiplusColor(linecolor, enable_alpha),
			linewidth,
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}

	void EasyX_Gdiplus_SolidRectangle(
		float x,
		float y,
		float w,
		float h,
		COLORREF fillcolor,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_SolidRectangle(
			GetImageHDC(pImg),
			x, y, w, h,
			ConvertToGdiplusColor(fillcolor, enable_alpha),
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}

	void EasyX_Gdiplus_FillRectangle(
		float x,
		float y,
		float w,
		float h,
		COLORREF linecolor,
		COLORREF fillcolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		EasyX_Gdiplus_SolidRectangle(x, y, w, h, fillcolor, enable_alpha, enable_aa, pImg);
		EasyX_Gdiplus_Rectangle(x, y, w, h, linecolor, linewidth, enable_alpha, enable_aa, pImg);
	}

	void EasyX_Gdiplus_Ellipse(
		float x,
		float y,
		float w,
		float h,
		COLORREF linecolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_Ellipse(
			GetImageHDC(pImg),
			x, y, w, h,
			ConvertToGdiplusColor(linecolor, enable_alpha),
			linewidth,
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}

	void EasyX_Gdiplus_SolidEllipse(
		float x,
		float y,
		float w,
		float h,
		COLORREF fillcolor,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_SolidEllipse(
			GetImageHDC(pImg),
			x, y, w, h,
			ConvertToGdiplusColor(fillcolor, enable_alpha),
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}

	void EasyX_Gdiplus_FillEllipse(
		float x,
		float y,
		float w,
		float h,
		COLORREF linecolor,
		COLORREF fillcolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		EasyX_Gdiplus_SolidEllipse(x, y, w, h, fillcolor, enable_alpha, enable_aa, pImg);
		EasyX_Gdiplus_Ellipse(x, y, w, h, linecolor, linewidth, enable_alpha, enable_aa, pImg);
	}

	void EasyX_Gdiplus_Pie(
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		COLORREF linecolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_Pie(
			GetImageHDC(pImg),
			x, y, w, h, stangle, -endangle,
			ConvertToGdiplusColor(linecolor, enable_alpha),
			linewidth,
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}

	void EasyX_Gdiplus_SolidPie(
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		COLORREF fillcolor,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_SolidPie(
			GetImageHDC(pImg),
			x, y, w, h, stangle, -endangle,
			ConvertToGdiplusColor(fillcolor, enable_alpha),
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}

	void EasyX_Gdiplus_FillPie(
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		COLORREF linecolor,
		COLORREF fillcolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		EasyX_Gdiplus_SolidPie(x, y, w, h, stangle, endangle, fillcolor, enable_alpha, enable_aa, pImg);
		EasyX_Gdiplus_Pie(x, y, w, h, stangle, endangle, linecolor, linewidth, enable_alpha, enable_aa, pImg);
	}

	void EasyX_Gdiplus_Arc(
		float x,
		float y,
		float w,
		float h,
		float stangle,
		float endangle,
		COLORREF linecolor,
		float linewidth,
		bool enable_alpha,
		bool enable_aa,
		IMAGE* pImg
	)
	{
		Gdiplus_Arc(
			GetImageHDC(pImg),
			x, y, w, h, stangle, endangle,
			ConvertToGdiplusColor(linecolor, enable_alpha),
			linewidth,
			enable_aa ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeDefault
		);
	}
};

