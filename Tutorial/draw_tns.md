> [回到教程目录](./index.md)

# 使用基础 EasyX 函数绘制透明图形

前面已经说过，可以使用 HiEasyX 内封装的 GDI+ 接口绘制透明和抗锯齿图形，详见 [这里](./draw_gdiplus.md)

此外，也可以使用基础 EasyX 函数实现绘制透明图形。

这里说一句，我所指的“基础 EasyX 函数”包括封装在 Canvas 内的 EasyX 原生函数，以及原生 EasyX 函数。

如下代码所示：

```cpp
#include "HiEasyX.h"

int main()
{
	initgraph(640, 480);

	hiex::Canvas canvas;
	hiex::BindWindowCanvas(&canvas);

	canvas.CenterText(L"Wonderful");

	// 绘制透明图形
	DRAW_TNS_INIT_GRAPHICS(201, 201);
	{
		graphics.FillCircle(100, 100, 100, true, RED, SKYBLUE);
	}
	DRAW_TNS_RENDER_TO(120, 120, canvas.Pt(), 100);

	REDRAW_WINDOW();
	getmessage(EM_CHAR);
	closegraph();
	return 0;
}
```

分析上述代码：

1. 首先准备绘制透明图形，指定透明图形的宽高（201 * 201）

```cpp
DRAW_TNS_INIT_GRAPHICS(201, 201);
```

2. 然后在一个代码块中使用 `graphics` 变量绘图

```cpp
{
	graphics.FillCircle(100, 100, 100, true, RED, SKYBLUE);
}
```

注意，不打花括弧也可以编译，但是最好还是打上，因为宏定义展开 `DRAW_TNS_INIT_GRAPHICS` 和 `DRAW_TNS_RENDER_TO` 他俩之间就是有花括弧的，这里也打上，以免不知道这中间是代码块。

此外，关于变量 `graphics`，这是在 `DRAW_TNS_INIT_GRAPHICS` 宏里面定义的，就是用来给你绘制透明图形的。你也无需担心这个变量和代码块外的变量产生命名冲突，因为代码块内外的变量是可以重名的。

还有，在这个代码块内也可以不使用 `graphics` 变量进行绘制，你可以直接就用原生 EasyX 函数画图就行了，像这样：

```cpp
DRAW_TNS_INIT_GRAPHICS(201, 201);
{
	setfillcolor(GREEN);
	fillcircle(100, 100, 100);
}
DRAW_TNS_RENDER_TO(120, 120, canvas.Pt(), 100);
```

其实就相当于在这个代码块内，WorkingImage 被临时设置到了变量 `graphics` 上，一出代码块就设置回来了。所以注意在代码块内的坐标系是 `graphics` 的坐标系。

3. 最后设定你所绘制的透明图形要输出到什么地方，以及透明度是多少

```cpp
DRAW_TNS_RENDER_TO(
	120,			// 输出坐标 X
	120,			// 输出坐标 Y
	canvas.Pt(),	// 输出到哪个 IMAGE*
	100				// 透明度（0~255）
);
```

> [回到教程目录](./index.md)
