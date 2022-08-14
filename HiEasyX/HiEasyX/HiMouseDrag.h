/**
 * @file	HiMouseDrag.h
 * @brief	HiEasyX 库的鼠标拖动处理模块
 * @author	huidong
*/

#pragma once

#include <graphics.h>

namespace HiEasyX
{
	/**
	 * @brief 鼠标拖动
	 * @note
	 *		调用方法
	 *		需要在鼠标消息循环中每次都调用 UpdateMessage 更新鼠标消息
	 *		调用 isLeftDrag，isMiddleDrag，isRightDrag 函数判断正在拖动的鼠标按键
	 *		调用 GetDragX，GetDragY 获取鼠标拖动时鼠标坐标的变化量
	*/
	class MouseDrag
	{
	private:
		ExMessage old = { 0 }, msg = { 0 };
		int dx = 0, dy = 0;
		bool lbtn = false, mbtn = false, rbtn = false;
		bool ldrag = false, mdrag = false, rdrag = false;
		bool newmsg = false;

		bool UpdateDragInfo(bool& btn, int msgid_down, int msgid_up);
		
	public:

		void UpdateMessage(ExMessage m);	///< 更新鼠标消息

		bool isLeftDrag();					///< 鼠标左键是否拖动
		bool isMiddleDrag();				///< 鼠标中键是否拖动
		bool isRightDrag();					///< 鼠标右键是否拖动

		int GetDragX();						///< 获取拖动的 x 坐标偏移量
		int GetDragY();						///< 获取拖动的 y 坐标偏移量
	};

};
