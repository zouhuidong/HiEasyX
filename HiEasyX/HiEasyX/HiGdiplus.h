/**
 * @file	HiGdiPlus.h
 * @brief	HiEasyX 库的 Gdi+ 绘图模块
 * @author	huidong
*/

#pragma once

#include <Windows.h>
#include <gdiplus.h>

namespace HiEasyX
{
	/**
	 * @brief 启动 GDI+
	*/
	void Gdiplus_Starup();

	/**
	 * @brief 关闭 GDI+
	*/
	void Gdiplus_Shutdown();

	/**
	 * @brief 转换 COLORREF 到 Gdiplus::Color
	 * @param[in] color				原颜色
	 * @param[in] reserve_alpha		是否保留 COLORREF 中的 alpha 值
	 * @return 转换后的 Gdiplus::Color 色值
	*/
	Gdiplus::Color ConvertToGdiplusColor(COLORREF color, bool reserve_alpha = false);
	
	/**
	 * @brief 画线
	*/
	void Gdiplus_Line(
		HDC hdc,
		float x1,
		float y1,
		float x2,
		float y2,
		Gdiplus::Color color,
		float width,
		Gdiplus::SmoothingMode smoothing_mode
	);

	void Gdiplus_Polygon(
		HDC hdc, 
		int points_num, 
		Gdiplus::PointF* points,
		Gdiplus::Color color, 
		float width, 
		Gdiplus::SmoothingMode smoothing_mode
	);

	void Gdiplus_Rectangle(HDC hdc, float x, float y, float w, float h, Gdiplus::Color color, float width, Gdiplus::SmoothingMode smoothing_mode);


};
