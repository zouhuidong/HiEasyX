> [回到教程目录](./index.md)

# 使用封装的 GDI+ 接口绘图

HiEasyX 对 GDI+ 的封装在 `HiGdiplus.h` 中，这些绘图函数都支持透明通道和抗锯齿。可以这样调用它们：

```cpp
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
		true,					// 启用抗锯齿
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


> [回到教程目录](./index.md)
