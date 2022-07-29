////////////////////////////
//
//	HiDrawingProperty.h
//	HiEasyX 库的绘图属性存储模块
//

#pragma once

#include <graphics.h>

namespace HiEasyX
{

	// 绘图属性
	// setorigin 和 setcliprgn 是对于绘图设备来说的，故不保存
	class DrawingProperty
	{
	private:
		bool m_isSaved = false;

	public:
		IMAGE* m_pImg;
		float m_xasp, m_yasp;
		COLORREF m_cBk;
		int m_nBkMode;
		COLORREF m_cFill;
		FILLSTYLE m_fillstyle;
		COLORREF m_cLine;
		LINESTYLE m_linestyle;
		int m_nPolyFillMode;
		int m_nRop2Mode;
		COLORREF m_cText;
		LOGFONT m_font;

		void SaveProperty();				// 保存当前所有的绘图属性
		void SaveWorkingImageOnly();		// 只保存当前绘图对象
		void ApplyProperty();				// 应用保存的所有绘图属性
		void ApplyWorkingImageOnly();		// 只恢复绘图对象
		bool isSaved();						// 是否保存了绘图属性
		void Reset();						// 重置保存状态

	};

};
