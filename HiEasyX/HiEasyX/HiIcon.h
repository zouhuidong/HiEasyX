//////////////////////////
//
//	HiIcon.h
//	HiEasyX 库的图标模块
//	
//	用于存储 HiEasyX 图标
//

#pragma once

#include <graphics.h>

// 图标大小
#define HIICON_WIDTH	64
#define HIICON_HEIGHT	64

namespace HiEasyX
{
	// 获取图标显存
	DWORD* GetIconImageBuffer();

	// 获取图标对象（显存的拷贝）
	IMAGE* GetIconImage();
};
