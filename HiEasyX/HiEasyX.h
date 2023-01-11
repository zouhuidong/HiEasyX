/**
 * @file	HiEasyX.h
 * @brief	HiEasyX 库头文件
 * @note	该库基于 EasyX 实现了在窗口、绘图、UI 等诸多方面的全面升级
 * 
 * @version	Ver 0.2.2
 * @envir	VisualStudio 2022 | EasyX_20220901 | Windows 10
 * @site	https://github.com/zouhuidong/HiEasyX
 * 
 * @author	huidong
 * @email	mailhuid@163.com
 * @create	2022.07.20
 * @date	2022.12.04
*/

#pragma once

#define __HIEASYX_H__

#include "HiEasyX/HiDef.h"
#include "HiEasyX/HiMacro.h"
#include "HiEasyX/HiFunc.h"
#include "HiEasyX/HiFPS.h"
#include "HiEasyX/HiDrawingProperty.h"
#include "HiEasyX/HiMouseDrag.h"
#include "HiEasyX/HiMusicMCI.h"
#include "HiEasyX/HiGif.h"

// 未选择使用原生 EasyX
#ifndef _NATIVE_EASYX_
#include "HiEasyX/HiWindow.h"
#include "HiEasyX/HiSysGUI/HiSysGUI.h"
#endif /* _NATIVE_EASYX_ */

#include "HiEasyX/HiGdiplus.h"
#include "HiEasyX/HiCanvas.h"
#include "HiEasyX/HiGUI/HiGUI.h"


// HiEasyX 命名空间缩写
namespace hiex = HiEasyX;

// 兼容旧版 EasyWin32 命名空间，请尽量使用 HiEasyX 命名空间
namespace EasyWin32 = HiEasyX;

