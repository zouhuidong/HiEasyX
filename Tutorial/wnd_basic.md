> [回到教程目录](./index.md)

# 绘图窗口基础

我们先从一个简单示例开始：

```cpp
#include "HiEasyX.h"		// 包含 HiEasyX 头文件

int main()
{
	initgraph();			// 初始化窗口

	BEGIN_TASK();			// （不同于 EasyX）启动任务，标识开始绘制
	
	circle(320, 240, 100);	// 画圆

	END_TASK();				// （不同于 EasyX）完成绘制，结束任务

	REDRAW_WINDOW();		// （不同于 EasyX）刷新屏幕，使刚才绘制的内容显示出来

	getmessage(EM_KEY);		// 任意键退出

	closegraph();			// 关闭窗口
	return 0;
}
```

接下来我们来讲解。

由于 HiEasyX 完全重写了 EasyX 的绘图窗口实现，所以可以支持创建多窗口、拉伸窗口，也支持自定义窗口过程函数。

在 HiEasyX 中，创建、管理窗口的模块名为 `HiWindow`。

创建窗口的正确方式：

```cpp
// 方法 1：直接使用 initgraph，它实际上被宏定义为 HiEasyX 的窗口创建函数
initgraph(640, 480);

// 方法 2：调用 HiEasyX 的窗口创建函数
hiex::initgraph_win32(640, 480);

// 方法 3：使用 HiEasyX 的窗口类创建窗口
hiex::Window wnd(640, 480);

// 也可以这样使用窗口类创建窗口
hiex::Window wnd;
wnd.Create(640, 480);
```

创建窗口时还有一些可选参数，例如窗口名称、窗口属性、过程函数、父窗口句柄，等等。详情请查阅 [文档](https://zouhuidong.github.io/HiEasyX/doxygen/html/class_hi_easy_x_1_1_window.html) 或头文件。

如果想要创建多个窗口，再次调用创建窗口函数即可。

> **想使用原生 EasyX ？**
>
> 在 `HiDef.h` 中定义 `_NATIVE_EASYX_` 宏后，`initgraph` 函数将创建原生的 EasyX 窗口。不过，这样会导致部分 HiEasyX 的功能不可用。

## 确定窗口是否还存在

EasyX 的用户可能早已习惯不判断绘图窗口是否还存在，因为在 EasyX 中，窗口一旦被关闭，将自动退出程序。

但是 HiEasyX 给您更多的选择。

**方式一：使用 `initgraph` 创建窗口**

所有窗口被关闭后，程序将自动退出。

**方式二：使用 `hiex::initgraph_win32` 或 `hiex::Window` 创建窗口**

所有窗口被关闭后，不会自动退出程序（若创建窗口后调用 `hiex::AutoExit()`，则会自动退出程序）。

您可以随时使用 `hiex::IsAnyWindow()` 检测是否还存在任何窗口。

也可以使用 `hiex::IsAliveWindow()` 或 `hiex::Window::IsAlive()` 检测某一窗口是否存在。

> **特别注意！** 
> 
> 以此方式创建窗口，如果不判断窗口是否关闭，则窗口被关闭后程序会一直在后台运行。

## IMAGE* 的空指针

在原生 EasyX 中，`(IMAGE*)(nullptr)` 代表着绘图窗口的图像指针。但是 `HiWindow` 创建的不是原生 EasyX 窗口，所以不支持 `(IMAGE*)(nullptr)`。

诸如以下函数都默认会传入 `IMAGE* pImg = NULL`：

* `GetImageBuffer`
* `SetWorkingImage`
* `GetImageHDC`

等等。

使用这些函数时，都应该传入具体的图像指针。您可以使用 `GetWorkingImage()` 或 `GetWindowImage()` 获取窗口的图像指针。

> **注意：**
>
> 由于窗口支持拉伸，缓冲区 IMAGE 对象在窗口拉伸时会自动调整大小，这会导致 IMAGE 对象的缓冲区内存重新分配。所以如果您保存了缓冲区 IMAGE 对象的显存指针，则一定要检测窗口是否被拉伸（使用 `hiex::IsWindowSizeChanged()` 或 `hiex::Window::IsSizeChanged()`），然后重新获取最新的显存指针。否则，很可能发生内存越界问题。

## 活动窗口的概念

由于 `HiWindow` 支持多窗口，所以操作窗口时要指定目标操作窗口。而 HiEasyX 操作多窗口的逻辑和 EasyX 中的 `SetWorkingImage()` 类似，也就是在操作某个窗口前，将这个窗口设置为活动窗口，然后再对其进行操作。

可以通过 `hiex::SetWorkingWindow()` 设置当前活动窗口，同时，当前工作绘图对象（WorkingImage）也会被设置到活动窗口的 IMAGE 对象。

## 窗口任务

由于 `HiWindow` 在多线程中处理多窗口和窗口拉伸事件，所以会导致绘制冲突问题。为了协调冲突，需要在调用 EasyX 绘图函数前，需要标记开启一个窗口任务。

`BEGIN_TASK()` 为当前活动窗口开启窗口任务

`BEGIN_TASK_WND()` 设置某个窗口为活动窗口，再为它开启窗口任务

`END_TASK()` 结束窗口任务，它必须和上述的两个宏之一配套，因为宏内含有不成对的大括号，将宏配套使用才能使大括号匹配。

例如：
```cpp
BEGIN_TASK();		// （不同于 EasyX）启动任务，标识开始绘制
circle(320, 240, 100);	// 画圆
END_TASK();		// （不同于 EasyX）完成绘制，结束任务
```

或者：
```cpp
BEGIN_TASK_WND(【要启动任务的窗口句柄】);	// 为某个窗口启动任务，标识开始绘制
circle(320, 240, 100);			// 画圆
END_TASK();				// 完成绘制，结束任务
```

上面的宏可以这样展开：
```cpp
if (hiex::SetWorkingWindow(【要启动任务的窗口句柄】))
{
	if (hiex::BeginTask())
	{
		// 窗口任务内的代码
		circle(320, 240, 100);	// 画圆

		hiex::EndTask();
	}
}
```

由于宏展开后内部含有大括号，所以窗口任务内的代码其实是位于代码块内部的，所以更推荐下面这种写法（不强求）：
```cpp
BEGIN_TASK();
{
	circle(320, 240, 100);
}
END_TASK();
```

此外，调用 `getmessage()` 一系列消息获取函数时，无需启动窗口任务，因为它们可以直接指定获取哪个窗口的消息。

> **注意：**
> 
> 尽量将不必要在窗口任务中执行的代码移出窗口任务代码块，因为它们可能会导致窗口任务耗时过长，窗口消息就不能及时被处理，进而引发窗口卡顿或假死。（例如实现延时绘图效果时，`Sleep` 语句不应当放在窗口任务内）
>
> 此外，在两个窗口任务之间插入适当的间隙也很有必要，例如在无限循环的绘制中插入 `Sleep` 语句，这样同时也能降低 CPU 占用率。

## 双缓冲机制

HiEasyX 强制使用双缓冲，所以无需再调用 EasyX 的 `BeginBatchDraw()` 系列函数。

EasyX 原生的 `FlushBatchDraw()` 和 `EndBatchDraw()` 函数都被宏定义为刷新窗口函数（ `hiex::RedrawWindow()` ）

HiEasyX 刷新双缓冲的机制是：每次窗口任务结束时，也就是调用 `hiex::EndTask();` 或者 `END_TASK();` 时，都会标记需要刷新双缓冲，然后在窗口接受到重绘消息时，再刷新双缓冲。

备注：调用 `hiex::EndTask();` 或者 `END_TASK();` 时，也可以传入参数指定是否标记需要刷新双缓冲。

这种刷新双缓冲的机制叫做自动刷新双缓冲，因为每次窗口任务结束都自动标记了需要刷新双缓冲，不需要手动去刷新。当然，也可以通过 `hiex::EnableAutoFlush` 函数关闭自动刷新双缓冲。

如果关闭了自动刷新双缓冲，那么就需要你调用 `hiex::FlushDrawing` 函数来手动刷新双缓冲（这个函数必须在窗口任务内调用），这个函数也可以只刷新局部的双缓冲。

## 窗口刷新机制

刷新了双缓冲并不意味着刷新了窗口。刷新双缓冲后，窗口不一定刷新了，这个时候就可能看不到绘制的效果。所以，绘制完成后，还需要通知窗口刷新。

你可以调用 `REDRAW_WINDOW();` 宏来刷新窗口，他等价于 `hiex::RedrawWindow();`。

但是，如果你自定义了窗口过程函数，并且在窗口过程函数的 `WM_PAINT` 或 `WM_USER_REDRAW` 消息中绘图，就不需要通知窗口刷新。

## 消息事件

由于 HiEasyX 的窗口是在 HiWindow 中重新实现的，所以 ExMessage 的消息队列也是重新实现的，它现在已经完美兼容 EasyX 的 ExMessage 消息队列了。

所以在 HiEasyX 中，获取鼠标、键盘、窗口等等消息和在 EasyX 中是一样的，你可以使用：

* `getmessage` 系列原生 EasyX 函数（其实是被宏定义为了 HiEasyX 内部的消息获取函数）
* `hiex::getmessage_win32` 系列函数
* `hiex::Window::Get_Message` 系列函数

以及旧版 EasyX 的 `GetMouseMsg` 系列函数，都支持。

> [回到教程目录](./index.md)
