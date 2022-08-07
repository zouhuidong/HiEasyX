////////////////////////////////////
//
//	Page.h
//	HiGUI 控件分支：页控件
//

#pragma once

#include "ControlBase.h"

namespace HiEasyX
{
	// 页控件
	class Page : public ControlBase
	{
	protected:

		Canvas* m_pCanvas = nullptr;

		virtual void Init(COLORREF cBk = WHITE);

	public:

		Page();

		Page(int w, int h, COLORREF cBk = WHITE);

		Page(Canvas* pCanvas);

		// 绑定到画布
		virtual void BindToCanvas(Canvas* pCanvas);

		// 加入控件
		virtual void push(ControlBase* pCtrl, int offset_x = 0, int offset_y = 0);

		virtual void push(const std::list<ControlBase*> list);

		// 移除控件
		virtual void remove(ControlBase* pCtrl);

		// 渲染（若已绑定画布，则默认输出到绑定画布）
		void Render(Canvas* dst = nullptr, RECT* pRct = nullptr, int* pCount = 0) override;

		// 更新画布到...（若已绑定画布，则默认输出到绑定画布）
		void UpdateImage(Canvas* pCanvas = nullptr);
	};
}

