//////////////////////////
//
//	HiFPS.h
//	HiEasyX 库的帧率模块
//

#pragma once

namespace HiEasyX
{
	// 根据目标帧率延时
	// wait_long	是否长等待（降低占用）
	void DelayFPS(int fps, bool wait_long = false);
};
