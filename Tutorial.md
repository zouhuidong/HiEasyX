# HiEasyX 快速攻略

## Why HiEasyX

EasyX 从设计之初，它就仅仅是一个图形库，不涉及其它方面的功能。

当我们用 EasyX 制作软件或游戏时经常会遇到下列问题：

* 创建多个绘图窗口
* 完整的控件库
* 播放 gif 动画
* 声音
* 画布
* 透明通道
* 图层

等等……

您是否曾经为它们苦恼？HiEasyX 可能是一个更完美的解决方案。

您想用 EasyX 更高效地制作软件或游戏吗？HiEasyX 或许是适合您的选择。

HiEasyX 不是独立的一个库，它依赖 EasyX，是 EasyX 充分的扩展库，它使用 C++，或许不适合 EasyX 的初学者。

## 准备 HiEasyX

**配置库**

请确保您已经在 Visual Studio 项目中配置好 HiEasyX，如果还没有，请看 [README.md](./README.md#配置此库)

推荐使用新版 Visual Studio 编译项目，详见编译环境 [README.md](./README.md#编译环境)

**库函数文档**

见 `./doc/index.html`，或在此浏览 [在线版](https://zouhuidong.github.io/HiEasyX)

**原有项目使用 HiEasyX**

HiEasyX 的高兼容性支持您轻松地将原先的 EasyX 项目配置上 HiEasyX。 [详细教程](#在原有-easyx-项目上使用-hieasyx)

## 使用 HiEasyX

我们先从一个简单示例开始：

```cpp
#include "HiEasyX.h"			// 包含 HiEasyX 头文件

int main()
{
	initgraph();			// 初始化窗口

	BEGIN_TASK();			// （不同于 EasyX）启动任务，标识开始绘制

	circle(320, 240, 100);		// 画圆

	END_TASK();			// （不同于 EasyX）完成绘制，结束任务

	FLUSH_DRAW();			// （不同于 EasyX）将绘制内容刷新到屏幕

	getmessage(EM_KEY);		// 任意键退出

	closegraph();			// 关闭窗口
	return 0;
}
```

接下来我为您解释如何使用 HiEasyX。

### 包含此库

只需要
```cpp
#include "HiEasyX.h"
```

### 命名空间

HiEasyX 在代码中使用 HiEasyX 命名空间，缩写 `hiex`，兼容旧版命名空间 `EasyWin32`。

### 创建绘图窗口

HiEasyX 支持创建多个绘图窗口，调用 Win32 控件。HiEasyX 的窗口模块叫 `HiWindow`。

像这样创建一个窗口：

```cpp
hiex::initgraph_win32(640, 480);
```
还有一些可选参数，此处不赘述，请查阅文档或头文件。

当然也支持 EasyX 原生的创建窗口函数，它被宏定义了，所以照样能用：
```cpp
initgraph(640, 480);
```

更推荐您使用 `hiex::Window` 类：
```cpp
hiex::Window wnd(640, 480);
```
或者
```cpp
hiex::Window wnd;
wnd.Create(640, 480);
```

如果想要在项目中使用原生的 EasyX 窗口，请在 `HiDef.h` 中定义 `_NATIVE_EASYX_` 宏。

### 确定窗口是否还存在

EasyX 的用户可能早已习惯不判断绘图窗口是否还存在，因为在 EasyX 中，窗口一旦被关闭，将自动退出程序。

但是 HiEasyX 给您更多的选择。

如果使用 `initgraph` 创建窗口，则所有窗口被关闭后，程序将自动退出。

如果使用 `hiex::initgraph_win32` 或 `hiex::Window` 创建窗口，则不会自动退出程序。

对于第二种情况，您可以随时使用 `hiex::isAnyWindow()` 检测是否还存在任何窗口，然后再选择是否退出程序。

如果想检测某一窗口是否存在，可以使用 `hiex::isAliveWindow()` 或 `hiex::Window::isAlive()`。

如果想设置所有窗口关闭时，程序自动退出，可以调用一次 `AutoExit()`，即可。

### IMAGE* 的空指针

在原生 EasyX 中，`(IMAGE*)(nullptr)` 代表着绘图窗口的图像指针。但是 `HiWindow` 创建的不是原生 EasyX 窗口，所以不支持 `(IMAGE*)(nullptr)`。

诸如以下函数都默认会传入 `IMAGE* pImg = NULL`：

* `GetImageBuffer`
* `SetWorkingImage`
* `GetImageHDC`

等等。

使用这些函数时，都应该传入具体的图像指针。您可以使用 `GetWoringImage()` 或 `GetWindowImage()` 获取窗口的图像指针。

### 活动窗口的概念

`HiWindow` 支持多窗口。操作多窗口的逻辑和 EasyX 中的 `SetWorkingImage()` 类似，也就是在操作某个窗口前，将这个窗口设置为活动窗口。

通过 `hiex::SetWorkingWindow()` 设置当前活动窗口。

### 窗口任务

由于 `HiWindow` 支持多窗口和窗口拉伸，所以会导致绘制冲突问题。为了协调冲突，需要在调用 EasyX 绘图函数前，需要标记开启一个窗口任务。

`BEGIN_TASK()` 为当前活动窗口开启窗口任务

`BEGIN_TASK_WND()` 设置某个窗口为活动窗口，再为它开启窗口任务

`END_TASK()` 结束窗口任务，它必须和上述的两个宏之一配套，因为宏内含有不成对的大括号。

上面的宏可以这样展开：
```cpp
if (hiex::SetWorkingWindow(_YourWindowHandle_))
{
	if (hiex::BeginTask())
	{
		// 窗口任务内的代码

		hiex::EndTask();
	}
}
```

调用 `getmessage()` 一系列消息获取函数时，无需启动窗口任务，因为它们可以指定获取哪个窗口的消息，不会产生冲突。

### 绘图缓冲

HiEasyX 强制双缓冲，所以无需再调用 EasyX 的 `BeginBatchDraw()` 系列函数

EasyX 原生的 `FlushBatchDraw()` 和 `EndBatchDraw()` 函数都被宏定义为输出绘图缓冲（`hiex::EnforceRedraw()`）

由于绘图代码在窗口任务中执行，故每次窗口任务结束时将默认输出绘图缓冲。

但是，如果不是在窗口过程函数的 `WM_PAINT` 消息中绘图，就还需要在结束窗口任务后调用 `FLUSH_DRAW()` 宏发送窗口重绘消息（它也等同于 `hiex::EnforceRedraw()`），这样才能将绘制内容刷新到屏幕上。

### 消息事件

由于 HiEasyX 的窗口是在 HiWindow 中重新实现的，所以 ExMessage 的消息队列也是重新实现的，它现在已经完美兼容 EasyX 的 ExMessage 消息队列了。

所以在 HiEasyX 中，获取鼠标、键盘消息和在 EasyX 中是一样的。由于 HiEasyX 支持多窗口，所以在 HiEasyX 中可能还需要额外指定需要获取哪个窗口的消息。

### 自定义窗口过程函数

为了维护 HiWindow 的正常运行，自定义窗口函数并不是使用 Win32 API 设置窗口的过程函数，您应该使用 `hiex::SetWndProcFunc()` 或 `hiex::Window::SetProcFunc()`。

自定义的过程函数的签名和普通的 Win32 过程函数相同，唯一的区别就是，返回 `DefWindowProc()` 时，改为返回 `HIWINDOW_DEFAULT_PROC` 宏标志。

下面是一段自定义过程函数的示例代码：

```cpp
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		BEGIN_TASK_WND(hWnd);
		circle(100, 100, 70);
		END_TASK();
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// TODO: 在此处释放申请的内存
		PostQuitMessage(0);
		break;

	default:
		return HIWINDOW_DEFAULT_PROC;	// 标识使用默认消息处理函数继续处理

		// 若要以默认方式处理，请勿使用此语句
		//return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}
```

### 创建托盘

这个无需多说，看一个很简单的示例代码即可。

```cpp
#include "HiEasyX.h"

#define IDC_A	101
#define IDC_B	102

void OnTray(UINT id)
{
	BEGIN_TASK();

	switch (id)
	{
	case IDC_A:
		outtextxy(100, 100, L"A");
		break;

	case IDC_B:
		outtextxy(100, 100, L"B");
		break;
	}

	END_TASK();
	FLUSH_DRAW();
}

int main()
{
	hiex::Window wnd;
	wnd.Create();

	wnd.CreateTray(L"Tray Name");

	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, IDC_A, L"选项 A");
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);			// 分隔符
	AppendMenu(hMenu, MF_STRING, IDC_B, L"选项 B");

	wnd.SetTrayMenu(hMenu);					// 设置菜单
	wnd.SetTrayMenuProcFunc(OnTray);		// 设置菜单响应函数

	hiex::init_end();						// 阻塞等待所有窗口关闭
	DestroyMenu(hMenu);						// 销毁菜单，释放内存
	return 0;
}

```

只要在托盘菜单里面点击某一项，程序就会作出反应。

### 添加程序图标

### 在原有 EasyX 项目上使用 HiEasyX

### 自定义窗口样式

### 代码中的常见问题整合
