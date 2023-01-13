> [回到教程目录](./index.md)

# 使用 Canvas 绑定窗口或 IMAGE 对象

Canvas 可以和 HiWindow 更好地融合，即直接将窗口和画布绑定，这样，在绘制时甚至不需要启动窗口任务，直接调用画布的绘制方法即可。例如：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(640, 480);			// 创建窗口
	hiex::Canvas canvas;				// 创建画布对象

	wnd.BindCanvas(&canvas);			// 将窗口和画布绑定

	canvas.Circle(130, 130, 30);		// 绘制画布对象（此处直接省去了开启窗口任务的步骤）
	wnd.Redraw();						// 重绘窗口

	hiex::init_end();					// 阻塞等待窗口关闭
	return 0;
}
```

注意，使用 Canvas 绑定到窗口后，每次使用 Canvas 绘制图形都会标记需要刷新窗口双缓冲。如果想要关闭这一功能，请使用 `hiex::Canvas::EnableAutoMarkFlushWindow` 函数。

也可以将一个 Canvas 对象绑定到已有的 IMAGE 对象，让 Canvas 为其绘制，只需要：

```cpp
canvas.BindToImage(【你的 IMAGE 指针】);
```

> **非常重要的提醒：**
>
> 一旦画布绑定窗口，或者绑定到其他 `IMAGE` 对象，请不要再使用 `&canvas` 的方式获取画布指针，请使用 `hiex::Canvas::GetImagePointer()` 或者 `hiex::Canvas::Pt()`。
>
> 这是因为 `&canvas` 只能对画布本身取址，但是不能取址到画布所绑定的对象。
>
> 如果可以的话，建议在任何时候都使用 `hiex::Canvas::GetImagePointer()` 或者 `hiex::Canvas::Pt()`，这会安全很多。
>
> 此外，由于绑定了窗口的画布绘图时会先启动窗口任务，所以不可以在其他窗口的窗口任务内绘图，否则会导致绘图时等待其他窗口任务结束阻塞。

> [回到教程目录](./index.md)

