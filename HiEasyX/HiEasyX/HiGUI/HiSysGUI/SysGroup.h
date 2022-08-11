////////////////////////////////////
//
//	SysGroup.h
//	HiSysGUI 控件分支：分组器
//

#pragma once

#include "SysControlBase.h"

namespace HiEasyX
{
	class SysGroup : public SysControlBase
	{
	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysGroup();

		SysGroup(HWND hParent);

		void Create(HWND hParent);
	};
}
