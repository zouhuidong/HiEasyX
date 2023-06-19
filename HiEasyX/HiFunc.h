/**
 * @file	HiFunc.h
 * @brief	HiEasyX 库的常用杂项函数
 * @author	huidong
*/

#pragma once

#include <Windows.h>
#include <WinUser.h>
#include <graphics.h>

/**
 * @brief 存储整个屏幕的大小信息（多显示器）
*/
struct ScreenSize
{
	int left;	///< 多显示器的左上角 x 坐标
	int top;	///< 多显示器的左上角 y 坐标
	int w;		///< 多显示器的总和宽度
	int h;		///< 多显示器的总和高度
};

/**
 * @brief 获取多显示器大小信息
*/
ScreenSize GetScreenSize();

/**
 * @brief <pre>
 *		获取图像尺寸
 *
 *	备注：
 *		可以方便地处理 IMAGE 指针为空，即指向主绘图窗口的情况
 * </pre>
 *
 * @param[in] pImg			目标图像
 * @param[out] width		返回图像宽
 * @param[out] height		返回图像高
*/
void GetImageSize(IMAGE* pImg, int* width, int* height);

/**
 * @brief <pre>
 *		反转图像 Alpha 值
 *
 *	备注：
 *		将 alpha 值不为 0 的一切像素的 alpha 设为 0，
 *		同时将 alpha 值为 0 的一切像素的 alpha 设为 255。
 * </pre>
 *
 * @param[in, out] pBuf		显存指针
 * @param[in] size			显存大小
 * @return 显存指针（和原来一样）
*/
DWORD* ReverseAlpha(DWORD* pBuf, int size);

/**
 * @brief <pre>
 *		创建指定尺寸及颜色的图像
 *
 *	备注：
 *		color 可以带有透明度。
 *		enable_alpha 若为 false 则会将返回图像透明度设为 255。
 * </pre>
 *
 * @param[in] w					返回图像宽度
 * @param[in] h					返回图像高度
 * @param[in] color				返回图像填充颜色
 * @param[in] enable_alpha		是否允许图像的 alpha 信息
 * @return 显存指针（和原来一样）
*/
IMAGE CreateImageColor(int w, int h, COLORREF color, bool enable_alpha);

/**
 * @brief <pre>
 *		设置图像的背景颜色
 *
 *	备注：
 *		color 可以带有透明度。
 *		enable_alpha 若为 false 则会将图像透明度设为 255。
 * </pre>
 *
 * @param[in] img				设置的图像
 * @param[in] color				图像填充颜色
 * @param[in] enable_alpha		是否允许颜色的 alpha 信息
 * @return 显存指针（和原来一样）
*/
void SetImageColor(IMAGE& img, COLORREF color, bool enable_alpha);

/**
 * @brief 得到 IMAGE 对象的 HBITMAP
 * @param[in] img			目标图像
 *
 * @param[in] enable_alpha <pre>
 *		是否允许图像的 alpha 信息
 *
 *	注意：
 *		若图像 alpha 值全为 0，则表示不启用透明混合
 * </pre>
 *
 * @return 转换得到的位图句柄
*/
HBITMAP Image2Bitmap(IMAGE* img, bool enable_alpha);

/**
 * @brief 得到 HBITMAP 对象的 IMAGE
 * @param[in] img			目标图像
 *
 * @param[in] enable_alpha <pre>
 *		是否允许图像的 alpha 信息
 *
 *	注意：
 *		若图像 alpha 值全为 0，则表示不启用透明混合
 * </pre>
 *
 * @return 转换得到的位图句柄
*/
IMAGE Bitmap2Image(HBITMAP* hBitmap, bool enable_alpha);

/**
 * @brief HBITMAP 转 HICON
 * @param[in] hBmp 位图句柄
 * @return 图标句柄
*/
HICON Bitmap2Icon(HBITMAP hBmp);

/**
 * @brief 精确延时函数（可以精确到 1ms，精度 ±1ms）
 * @author yangw80 <yw80@qq.com>
 * @date 2011-5-4
 * @param[in] ms 延时长度（单位：毫秒）
*/
void HpSleep(int ms);

/**
 * @brief 点是否位于矩形内
 * @param[in] x		位置
 * @param[in] y		位置
 * @param[in] rct		矩形
 * @return 点是否位于矩形内
*/
bool IsInRect(int x, int y, RECT rct);

/**
 * @brief 获取 ExMessage 的消息类型
 * @param[in] msg 消息
 * @return EM_ 消息类型中的一种，若失败返回 0
*/
UINT GetExMessageType(ExMessage msg);

namespace HiEasyX
{
	/**
	 * @brief 绘制图像（可包含透明通道）
	 * @param[in] Dstimg 指向目标位图的指针
	 * @param[in] DstimgX 目标位图上绘制的左上角横坐标
	 * @param[in] DstimgY 目标位图上绘制的左上角纵坐标
	 * @param[in] Srcimg 指向源位图的指针
	 * @param[in] transparency 叠加透明度
	 *
	 *	注意：
	 *		绘制出来的位图大小将与 Srcimg 中的图像一致
	 * </pre>
	*/
	void TransparentImage(IMAGE* Dstimg, int DstimgX, int DstimgY, IMAGE* Srcimg, int transparency = 255);

	/**
	 * @brief 绘制图像（可包含透明通道）
	 * @param[in] Dstimg 指向目标位图的指针
	 * @param[in] DstimgX 目标位图上绘制的左上角横坐标
	 * @param[in] DstimgY 目标位图上绘制的左上角纵坐标
	 * @param[in] DstimgWidth 目标位图上绘制的宽度
	 * @param[in] DstimgHeight 目标位图上绘制的高度
	 * @param[in] Srcimg 指向源位图的指针
	 * @param[in] SrcimgX 源位图中要绘制的区域的左上角横坐标
	 * @param[in] SrcimgY 源位图中要绘制的区域的左上角纵坐标
	 * @param[in] SourceWidth 源位图中要绘制的区域的宽度
	 * @param[in] SourceHeight 源位图中要绘制的区域的高度
	 * @param[in] transparency 叠加透明度
	 *
	 *	注意：
	 *		若 目标位图上绘制的宽高 不等于 源位图中要绘制的区域的宽高，那么源位图中的区域将会被拉伸或压缩以适应目标位图上的绘制区域
	 * </pre>
	*/
	void TransparentImage(IMAGE* Dstimg, int DstimgX, int DstimgY, int DstimgWidth, int DstimgHeight, IMAGE* Srcimg, int SrcimgX, int SrcimgY, int SourceWidth, int SourceHeight, int transparency = 255);
}