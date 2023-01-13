> [回到教程目录](./index.md)

# Canvas 画布绘图基础

## 概念

在 HiEasyX 中，你可以在窗口任务中使用原生 EasyX 函数绘图，但是使用画布绘图是更方便好用的选择。

**画布**（ `hiex::Canvas` ）是对 EasyX 绘图函数的封装和扩展。它的使用方法和 `IMAGE` 对象一样，不同的是，使用画布绘制时不需要 `SetWorkingImage`，可以直接调用对象方法进行绘制，而且它封装了 GDI+，支持透明通道和抗锯齿。

你可以创建一个画布对象，然后直接调用它的成员方法进行绘制。它们和 EasyX 原生绘图函数名称很像，但它们使用驼峰命名法，如果想了解更多，可以看看 `hiex::Canvas` 的声明。

Canvas 的优势：
1. 面向对象绘图，更直观
2. 除了 EasyX 绘图函数，还封装了 GDI+ 绘图函数
3. 支持透明和抗锯齿绘制
4. 支持透明通道的图片绘制、缩放、旋转
5. 可以绑定 HiEasyX 创建的窗口，这样做可以带来很大的便利，详见 [使用 Canvas 绑定窗口或 IMAGE 对象](./draw_bind.md)。

小便利：
1. 可以直接设置字体名称（而不必设置字体大小）以及字符（串）的绘制角度
2. 可以格式化输出文本 
3. 调用绘图函数时可以直接设置绘制颜色（可选）

还有很多，不能备述。

## 使用 Canvas 绘制

例如，用 Canvas 绘制基础图形：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(640, 480);			// 创建窗口

	hiex::Canvas canvas(60, 60);		// 创建画布对象

	canvas.Circle(30, 30, 30);			// 绘制画布

	if (wnd.BeginTask())				// 启动窗口任务
	{
		putimage(100, 100, &canvas);	// 将画布内容输出到窗口

		wnd.EndTask();					// 结束窗口任务
		wnd.Redraw();					// 重绘窗口
	}

	hiex::init_end();					// 阻塞等待窗口关闭
	return 0;
}
```

> [回到教程目录](./index.md)

