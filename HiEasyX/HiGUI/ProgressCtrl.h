////////////////////////////////////
//
//	ProgressCtrl.h
//	HiGUI 控件分支：进度条控件
//

#pragma once

#include "ControlBase.h"

#include <time.h>

namespace HiEasyX
{
	// 进度条控件
	class ProgressCtrl : public ControlBase
	{
	protected:

		bool m_bEnableAnimation = true;					// 是否启用动画效果

		int m_nLightPos = 0;							// 光源位置
		clock_t m_tClock = 0;							// 保留上一次动画的绘制时间
		float m_fSpeedRatio = 0.4;						// 动画速度（每秒经过总长度的多少）

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
}

