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

> 由于在线文档中的有些页面会出现 404，所以建议下载仓库后在本地浏览文档。

**原有项目使用 HiEasyX**

HiEasyX 的高兼容性支持您轻松地将原先的 EasyX 项目配置上 HiEasyX。 [详细教程](#在原有-easyx-项目上使用-hieasyx)

## 窗口篇：HiWindow 窗口模块

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
还有一些可选参数，此处不赘述，请查阅 [文档](https://zouhuidong.github.io/HiEasyX/doxygen/html/class_hi_easy_x_1_1_window.html) 或头文件。

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

> **想使用原生 EasyX ？**
>
> 在 `HiDef.h` 中定义 `_NATIVE_EASYX_` 宏后，`initgraph` 函数将创建原生的 EasyX 窗口。但是，这也意味着不再支持 HiEasyX 的许多扩展功能。

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

通过 `hiex::SetWorkingWindow()` 设置当前活动窗口，同时，当前工作绘图对象（WorkingImage）也会被设置到活动窗口的 IMAGE 对象。

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

> **注意：**
>
> 由于支持窗口拉伸，缓冲区 `IMAGE` 对象在窗口拉伸时会自动调整大小。如果您保存了缓冲区 `IMAGE` 对象的显存指针，则一定要检测窗口是否被拉伸（使用 `hiex::isWindowSizeChanged()` 或 `hiex::Window::isSizeChanged()`），然后更新显存指针。

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

运行此示例代码，将会在托盘中创建您的程序图标。只要在托盘菜单里面点击某一项，程序就会作出反应。

### 自定义程序图标

HiEasyX 默认为程序加载 HiEasyX 图标。在 EasyX 中，只要在 Visual Studio 项目中加入图标资源，程序就会自动加载您的图标。

在 HiEasyX 中，您加入图标资源后，还需要在第一次创建窗口前调用一次 `hiex::SetCustomIcon()`，即可。

下面列举两种在 Visual Studio 中添加图标资源的方法：

**方法一：** 在资源视图中右键项目 -> 【添加】 -> 【资源】，选择图标资源。

**方法二：** 在项目中创建 resource.h 和 【项目名】.rc。

在 resource.h 中：

```cpp
#define IDI_ICON1	101
```

在 【项目名】.rc 中：

```cpp
#include "resource.h"
IDI_ICON1	ICON	"icon.ico" /* 修改为你的图标文件路径 */
```

然后可以使用如下示例代码测试：

```cpp
#include "resource.h"
#include "HiEasyX.h"

int main()
{
	// 在创建窗口前设置图标
	hiex::SetCustomIcon(MAKEINTRESOURCE(IDI_ICON1), MAKEINTRESOURCE(IDI_ICON1));

	hiex::Window wnd;
	wnd.Create();

	hiex::init_end();
	return 0;
}
```

即可自定义图标。

### 自定义窗口样式

如果您想改变窗口样式，例如取消最大化按钮，禁止用户拉伸窗口，您可以使用 Win32 API `SetWindowLong` 函数，这需要一些 Win32 知识。

还有更简便的方式，直接使用 `hiex::SetWindowStyle()` 或 `hiex::Window::SetStyle()`。

HiEasyX 定义了如下宏用于快速设置窗口样式

`EnableResizing` 设置是否允许窗口拉伸

`EnableSystemMenu` 设置是否启用系统标题栏按钮

`EnableToolWindowStyle` 设置是否启用窗口的工具栏样式

等等。更多请参见 [文档](https://zouhuidong.github.io) 和 `HiWindow.h`。

## 绘图篇：HiCanvas 绘图模块

### 概念

**画布**（`hiex::Canvas`）是对 EasyX 绘图函数的封装和扩展。它的使用方法和 `IMAGE` 对象一样，不同的是，使用画布绘制时不需要 `SetWorkingImage`，可以直接调用对象方法进行绘制，而且它支持透明通道。

**图像块**（`hiex::ImageBlock`）是 `hiex::Canvas` 的扩展，它保存了画布的位置，透明通道信息，可以更方便地存储在图层中。 

**图层**（`hiex::Layer`）中存储有若干个图像块，图层的透明度可以叠加到所有图像块上。

**场景**（`hiex::Scene`）中存储有若干个图层，以及一些特殊图层。渲染整个场景时，可以使图层按次序渲染。

### 使用 Canvas 绘制

您可以创建一个画布对象，然后直接调用它的成员方法进行绘制。它们和 EasyX 原生绘图函数名称很像，区别仅在于它们使用驼峰命名法。

此外，Canvas 还提供一些更方便的绘制方式。例如：调用 Canvas 的绘图函数时可以选择直接设置绘制颜色，直接操作显存绘制或获取像素，支持透明通道的图片加载、缩放、旋转，直接设置字体名称（而不必设置字体大小）、字符（串）的绘制角度，格式化输出文本，等等。 

示例代码：

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

### 使用 Canvas 绑定窗口或 IMAGE 对象

Canvas 还可以和 HiWindow 更好地融合，可以直接将窗口和画布绑定，这样，在绘制时甚至不需要启动窗口任务，直接调用画布的绘制方法即可。例如：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(640, 480);			// 创建窗口
	hiex::Canvas canvas;				// 创建画布对象

	wnd.BindCanvas(&canvas);			// 将窗口和画布绑定

	canvas.Circle(130, 130, 30);		// 绘制画布对象
	wnd.Redraw();						// 重绘窗口

	hiex::init_end();					// 阻塞等待窗口关闭
	return 0;
}
```

也可以将一个 Canvas 对象绑定到已有的 IMAGE 对象，让 Canvas 为其绘制，只需要：

```cpp
canvas.BindToImage(_Your_Image_Pointer_);
```

> **注意：**
>
> 一旦画布绑定窗口，或者绑定到其他 `IMAGE` 对象，请不要再使用 `&canvas` 的方式获取画布指针，请使用 `hiex::Canvas::GetImagePointer()`，这很重要。

### 应用场景、图层、Alpha 通道

它们都很易于使用，您可以看下面的一个例子：

<div align=center>
<img src="./screenshot/balls1.png"><br>
<b>小球示例（1）</b>
</div><br>

<div align=center>
<img src="./screenshot/balls2.png"><br>
<b>小球示例（2）</b>
</div><br>

为了缩短篇幅，请您在此查看 [源代码](./Samples/Recommend/Balls.cpp)

## 控件篇：使用更完善的 Win32 UI 库

HiEasyX 封装了常用 Win32 控件，这个控件模块被称为 HiSysGUI。

目前支持的控件类型（此文档可能更新不及时）：

* 分组框
* 静态文本（图像）
* 按钮
* 复选框
* 单选框
* 编辑框
* 组合框

一般情况下，这些控件已经足够。而且，您也可以自定义窗口过程函数，直接调用其它 Win32 控件。

### 体验 HiSysGUI 的极速构建

请看这个例子：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);

	hiex::SysButton btn(wnd.GetHandle(), 100, 85, 100, 30, L"Button");

	hiex::init_end();
	return 0;
}
```

<div align=center>
<img src="./screenshot/fast_btn.png"><br>
<b>创建按钮</b>
</div><br>

很好！使用按钮就是这么容易。

如果要响应按钮消息，可以使用 `RegisterMessage` 方法，或者使用 `GetClickCount` 函数获取按钮点击次数。

例如，使用 `GetClickCount` 函数：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);

	hiex::SysButton btn(wnd.GetHandle(), 100, 85, 100, 30, L"Button");
	
	while (wnd.isAlive())
	{
		if (btn.GetClickCount())
		{
			// 处理点击消息
		}

		Sleep(50);
	}

	return 0;
}
```

或者注册点击消息：

```cpp
#include "HiEasyX.h"

void OnBtn()
{
	// 在此处理点击消息
}

int main()
{
	hiex::Window wnd(300, 200);

	hiex::SysButton btn(wnd.GetHandle(), 100, 85, 100, 30, L"Button");
	
	btn.RegisterMessage(OnBtn);	// 注册点击消息

	hiex::init_end();
	return 0;
}
```

其余控件的使用方式大同小异，可以看看相应的头文件介绍。此处再举一例，创建编辑框。

像这样：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);
	hiex::SysEdit edit;	// 编辑框

	// 预设样式为支持多行输入，因为有的控件样式必须在创建之前就指定
	edit.PreSetStyle(true, false, true, true, true, true);
	edit.Create(wnd.GetHandle(), 10, 10, 280, 180, L"Multiline Edit Box\r\n\r\nEdit here");

	// 设置编辑框字体
	edit.SetFont(24, 0, L"微软雅黑");

	hiex::init_end();
	return 0;
}
```

<div align=center>
<img src="./screenshot/fast_edit.png"><br>
<b>创建编辑框</b>
</div><br>

加上按钮，获取文本：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);

	// 编辑框
	hiex::SysEdit edit;
	edit.PreSetStyle(true, false, true, true);
	edit.Create(wnd.GetHandle(), 10, 10, 280, 140, L"Type here~");
	edit.SetFont(24, 0, L"微软雅黑");

	// 按钮
	hiex::SysButton btn;
	btn.Create(wnd.GetHandle(), 190, 160, 100, 30,L"Submit");

	while (wnd.isAlive())
	{
		// 按下按钮时，弹窗显示输入的文本
		if (btn.isClicked())
			MessageBox(wnd.GetHandle(), edit.GetText().c_str(), L"Submit", MB_OK);
		Sleep(50);
	}

	return 0;
}
```

<div align=center>
<img src="./screenshot/fast_edit_2.png"><br>
<b>获取编辑框文本</b>
</div><br>

还可以设置文字颜色、背景颜色、密码框、左中右对齐方式、仅数字输入、禁用控件，等等，不一一列举。这个教程不可能面面俱到，也有可能更新延迟，如果您想具体了解每个控件，可以看看它们的声明，此处不再展开。参阅 [文档](https://zouhuidong.github.io/HiEasyX/doxygen/html/class_hi_easy_x_1_1_sys_control_base.html)。

下面这个示例用到的控件比较全面，可以帮您更深入地了解 HiSysGUI：

<div align=center>
<img src="./screenshot/overview.png"><br>
<b>控件示例</b>
</div><br>

在此查看此示例的 [源代码](./Samples/Recommend/OverviewSample.cpp)

##

## 迁移篇：在原有 EasyX 项目上使用 HiEasyX



### 代码中的常见问题整合