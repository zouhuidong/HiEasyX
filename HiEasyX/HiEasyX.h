////////////////////////////////////////////////////
//
//	HiEasyX.h
//	HiEasyX 库头文件
//	该库基于 EasyX 实现了在窗口、绘图、UI 等诸多方面的全面升级
//
//	版　　本：Ver 0.2 (alpha)
//	作　　者：huidong <mailhuid@163.com>
//	编译环境：VisualStudio 2022 | EasyX_20220610 | Windows 10 
//	项目地址：https://github.com/zouhuidong/HiEasyX
//	创建日期：2022.07.20
//	最后修改：2022.08.11
//

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

#include "HiEasyX/HiCanvas.h"
#include "HiEasyX/HiGUI/HiGUI.h"


// HiEasyX 命名空间缩写
namespace hiex = HiEasyX;

// 兼容旧版 EasyWin32 命名空间，请尽量使用 HiEasyX 命名空间
namespace EasyWin32 = HiEasyX;

