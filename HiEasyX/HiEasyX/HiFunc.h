////////////////////////////
//
//	HiFunc.h
//	HiEasyX 库的常用杂项函数
//

#pragma once

#include <Windows.h>
#include <WinUser.h>
#include <graphics.h>

// 存储整个屏幕的大小信息（多显示器）
struct ScreenSize
{
	int left;	// 多显示器的左上角 x 坐标
	int top;	// 多显示器的左上角 y 坐标
	int w;		// 多显示器的总和宽度
	int h;		// 多显示器的总和高度
};

// 获取多显示器大小信息
ScreenSize GetScreenSize();

// 获取图像尺寸
// 可以方便地处理 IMAGE 指针为空，即指向主绘图窗口的情况
void GetImageSize(IMAGE* pImg, int& width, int& height);

// 反转图像 Alpha 值
// 将 alpha 值不为 0 的一切像素的 alpha 设为 0，
// 同时将 alpha 值为 0 的一切像素的 alpha 设为 255
DWORD* ReverseAlpha(DWORD* pBuf, int size);

// 得到 IMAGE 对象的 HBITMAP
// enable_alpha	是否启用透明度
// 注意，若图像 alpha 值全为 0，则表示不启用透明混合
HBITMAP Image2Bitmap(IMAGE* img, bool enable_alpha);

// HBITMAP 转 HICON
HICON Bitmap2Icon(HBITMAP hBmp);

// 精确延时函数(可以精确到 1ms，精度 ±1ms)
// by yangw80<yw80@qq.com>, 2011-5-4
void HpSleep(int ms);

// 点是否位于矩形内
bool isInRect(int x, int y, RECT rct);

// 获取 ExMessage 的 EM 消息类型
UINT GetExMessageType(ExMessage msg);
