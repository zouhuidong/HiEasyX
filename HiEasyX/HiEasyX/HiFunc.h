////////////////////////////
//
//	HiFunc.h
//	HiEasyX 库的常用函数模块
//

#pragma once

#include <graphics.h>

// 获取图像尺寸
// 可以方便地处理 IMAGE 指针为空，即指向主绘图窗口的情况
void GetImageSize(IMAGE* pImg, int& width, int& height);

// 得到 IMAGE 对象的 HBITMAP
HBITMAP Image2Bitmap(IMAGE* img);

// HBITMAP 转 HICON
HICON Bitmap2Icon(HBITMAP hBmp);

// 精确延时函数(可以精确到 1ms，精度 ±1ms)
// by yangw80<yw80@qq.com>, 2011-5-4
void HpSleep(int ms);

// 点是否位于矩形内
bool isInRect(int x, int y, RECT rct);

// 获取 ExMessage 的 EM 消息类型
UINT GetExMessageType(ExMessage msg);
