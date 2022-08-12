#include "SysGroup.h"

namespace HiEasyX
{
	void SysGroup::RealCreate(HWND hParent)
	{
		m_type = SCT_Group;
		m_hWnd = CreateControl(
			hParent,
			L"Button",
			L"",
			WS_CHILD | WS_GROUP
		);
	}

	SysGroup::SysGroup()
	{
	}

	SysGroup::SysGroup(HWND hParent)
	{
		Create(hParent);
	}

	void SysGroup::Create(HWND hParent)
	{
		RealCreate(hParent);
	}
}
