> [回到教程目录](./index.md)

# 使用封装的 GDI+ 接口绘图

HiEasyX 对 GDI+ 的封装在 `HiGdiplus.h` 中，这些绘图函数都支持透明通道和抗锯齿。可以这样调用它们：

```cpp
// HiEasyX Ver0.3.2
#include "HiEasyX.h"

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.SetFont(32);
	canvas.CenterText(L"Wonderful");

	// 由于 canvas 绑定了窗口，所以由外部向 canvas 上绘图前，要先启动窗口任务，
	// 此处使用 BeginBatchDrawing 是因为它会在内部启动窗口任务。
	canvas.BeginBatchDrawing();

	hiex::EasyX_Gdiplus_Line(
		20, 20, 600, 400,		// 画线坐标
		SET_ALPHA(RED, 170),	// 使用透明度为 170 的红色
		25,						// 设置线宽
		true,					// 启用透明通道
		Gdiplus::SmoothingModeAntiAlias,	// 启用抗锯齿
		canvas.Pt()				// 设置绘制到 canvas 上
	);

	// 对应 BeginBatchDrawing
	canvas.EndBatchDrawing();

	REDRAW_WINDOW();
	getmessage(EM_CHAR);
	closegraph();
	return 0;
}
```

其实也可以直接用 Canvas 调用 GDI+ 相关函数，它们作为 Canvas 的成员函数，都有 `GP_` 前缀，详细可以参阅 `hiex::Canvas` 的声明。

使用方法很简单，见代码：

```cpp
// HiEasyX Ver0.3.2
#include "HiEasyX.h"

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.SetFont(32);
	canvas.CenterText(L"Wonderful");

	canvas.GP_EnableAlpha(true);	// 启用 GDI+ 透明通道
	canvas.GP_SetLineWidth(25);		// 设置线宽
	canvas.GP_Line(
		20, 20, 600, 400,			// 画线坐标
		true, SET_ALPHA(RED, 170)	// 使用透明度为 170 的红色
	);

	REDRAW_WINDOW();
	getmessage(EM_CHAR);
	closegraph();
	return 0;
}
```

其它示例：
```cpp
// HiEasyX Ver0.3.2
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd;
	wnd.Create();

	hiex::Canvas canvas;
	wnd.BindCanvas(&canvas);

	// 开启透明通道
	canvas.GP_EnableAlpha(true);
	
	// 设置抗锯齿模式为高质量（抗锯齿的所有模式定义在枚举类型 Gdiplus::SmoothingMode 中）
	canvas.GP_SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	// 绘制圆角矩形
	canvas.GP_RoundRect(100, 120, 100, 100, 20, 20, true, SET_ALPHA(PURPLE, 100));
	canvas.GP_SolidRoundRect(180, 120, 100, 100, 20, 20, true, SET_ALPHA(GREEN, 100));
	canvas.GP_FillRoundRect(260, 120, 100, 100, 20, 20, true, SET_ALPHA(BLACK, 100), SET_ALPHA(RED, 100));

	wnd.Redraw();

	hiex::init_end();
	return 0;
}
```

> 查看了解更多封装的 Gdiplus 函数可以转到 [此处](https://hiex.alan-crl.top/function/graphical/gdiplus/)


> [回到教程目录](./index.md)
