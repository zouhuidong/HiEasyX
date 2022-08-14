## Why HiEasyX

EasyX 在制作软件或游戏时经常会遇到下列问题：

* 多窗口的支持
* 完整的控件库
* 动画
* 声音
* 画布
* 透明通道
* 图层

等等……

您是否曾经为它们苦恼？HiEasyX 可能是一个更完美的解决方案。

您想用 EasyX 更高效地制作软件或游戏吗？HiEasyX 或许是适合您的选择。

HiEasyX 不是独立的一个库，它依赖 EasyX，是 EasyX 充分的扩展库。

## 开始 HiEasyX

**配置库**

请确保您已经在 Visual Studio 项目中配置好 HiEasyX，如果还没有，请看 [README.md](./README.md#配置此库)

推荐使用新版 Visual Studio 编译项目，详见编译环境 [README.md](./README.md#编译环境)

**库函数文档**

见 `./doc/index.html`，或在此浏览 [在线版](https://zouhuidong.github.io/HiEasyX)

**原有项目使用 HiEasyX**

EasyWin32 的高兼容性支持您轻松地将原先的 EasyX 项目配置上 EasyWin32。 [详细教程](#在原有-easyx-项目上使用-easywin32)

---
*接下来将从数个具有代表性的例子开始，讲解 EasyWin32 的使用方法。*

## 例子 1：从 EasyX 到 EasyWin32 的快速过渡

请看这段调用 EasyWin32 的代码：

```cpp
#include "EasyWin32.h"
#include <conio.h>

int main()
{
	// 创建窗口
	EasyWin32::initgraph_win32();

	BEGIN_TASK();

	// 绘图
	settextstyle(32, 0, L"Consolas");
	settextcolor(LIGHTGREEN);
	outtextxy(20, 70, L"Hello EasyWin32");
	settextstyle(26, 0, L"system");
	settextcolor(GREEN);
	outtextxy(20, 110, L"Any key to continue");

	END_TASK();
	FLUSH_DRAW();

	while (true)
	{
		// 按任意键关闭窗口
		if (_kbhit())
		{
			EasyWin32::closegraph_win32();
			break;
		}

		// 若窗口被关闭，退出程序
		if (!EasyWin32::isAnyWindow())
		{
			break;
		}

		Sleep(10);
	}

	return 0;
}
```

很简单吧~其中大部分的函数都是 EasyX 的原生函数。

上面这段代码来自：[源码](./samples/Start/main.cpp)。对于上面代码中出现的 EasyWin32 的新增函数，将在稍后解释。

## 例子 2：创建多个绘图窗口！

下面的代码来自：[源码](./samples/Sample2/main.cpp)

```cpp
#include "EasyWin32.h"
#include <time.h>

int main()
{
	// 设置随机种子
	srand((unsigned)time(NULL));

	// 创建两个绘图窗口
	HWND hWnd1 = EasyWin32::initgraph_win32(640, 480, false, L"字符阵");
	HWND hWnd2 = EasyWin32::initgraph_win32(640, 480, false, L"鼠标操作示例");

	// 设置窗口 1 绘图样式
	EasyWin32::SetWorkingWindow(hWnd1);
	settextstyle(16, 8, _T("Courier"));
	settextcolor(GREEN);

	// 定时绘制
	clock_t tRecord = 0;

	while (true)
	{
		// 若窗口 1 还存在（未被关闭）
		if (EasyWin32::isAliveWindow(hWnd1))
		{
			// 一段时间重绘一次
			if (clock() - tRecord >= 100)
			{
				// 设置窗口 1 为目标绘图窗口，并启动一个绘图任务
				BEGIN_TASK_WND(hWnd1);

				// 绘制内容：EasyX 官方示例“字符阵”（简化）
				cleardevice();
				for (int i = 0; i <= 200; i++)
				{
					// 在随机位置显示三个随机字母
					for (int j = 0; j < 3; j++)
					{
						int x = (rand() % 80) * 8;
						int y = (rand() % 20) * 24;
						char c = (rand() % 26) + 65;
						outtextxy(x, y, c);
					}
				}

				// EasyWin32 默认使用双缓冲绘图，此处输出绘图缓冲
				// 注意：一段绘图任务结束，必须以此宏结尾（即 BEGIN_TASK_WND 和 END_TASK 必须连用）
				END_TASK();

				// 不在窗口过程函数的 WM_PAINT 消息内绘图时，必须强制重绘
				// 由于没有自定义窗口过程函数，所以当然也要调用此宏强制重绘
				FLUSH_DRAW();
			}
		}

		// 窗口 2
		if (EasyWin32::isAliveWindow(hWnd2))
		{
			BEGIN_TASK_WND(hWnd2);

			// 绘制内容：EasyX 官方示例“鼠标操作”（有改动）
			ExMessage m;
			while (peekmessage(&m, EM_MOUSE))
			{
				switch (m.message)
				{
				case WM_MOUSEMOVE:
					// 鼠标移动的时候画红色的小点
					putpixel(m.x, m.y, RED);
					break;

				case WM_LBUTTONDOWN:
					// 如果点左键的同时按下了 Ctrl 键
					if (m.ctrl)
						// 画一个大方块
						rectangle(m.x - 10, m.y - 10, m.x + 10, m.y + 10);
					else
						// 画一个小方块
						rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
					break;
				}
			}

			END_TASK();
			FLUSH_DRAW();
		}

		// 若所有窗口都被关闭，则结束程序
		if (!EasyWin32::isAnyWindow())
		{
			break;
		}

		// 降低 CPU 占用
		Sleep(50);
	}

	return 0;
}

```

这段代码中创建了两个窗口，并且每个窗口都演示了一个 EasyX 的官方示例程序。

### 判断窗口存在

由于创建了两个窗口，那么在主循环的绘图中，您需要分两个 `if` 语句分别判断窗口是否还存在，因为窗口可能被用户关闭。若窗口还存在，才继续绘图。

在程序主循环的末尾，调用了 `EasyWin32::isAnyWindow()` 判断了是否还存在已经被创建的窗口，如果所有窗口都被关闭，则跳出循环并关闭程序。

这条判断很容易被忽略，如果不对是否还存在窗口进行判断的话，所有窗口都被关闭后，`main` 函数仍会继续运行，但是不在 Windows 任务栏中显示，会残留在后台进程中。

事实上您可以在创建窗口完毕之后，调用一次 `EasyWin32::AutoExit();`，这个函数将自动检测是否还有任何窗口未被关闭，如果全部窗口都被关闭，则自动结束程序。

### 任务系统

在 `if` 语句内部，执行绘图任务时，需要调用 `BEGIN_TASK`（或 `BEGIN_TASK_WND`） 宏，任务结束时，使用 `END_TASK()` 宏。

`BEGIN_TASK` 系列宏用以标识一个任务的起始和结束，包括但不限于绘图任务。

每次执行绘图任务（无论是设置绘图属性还是绘图），或者是诸如鼠标消息获取的任务，都应当标识任务的开始和结束，否则，可能导致程序崩溃。

事实上，`BEGIN_TASK_WND` 和 `END_TASK` 是如下代码的宏定义：

```cpp
if (EasyWin32::SetWorkingWindow(hWnd))
{
	if (EasyWin32::BeginTask())
	{
		// TODO: Add your code here.

		EasyWin32::EndTask();
	}
}
```

这也意味着写在 `BEGIN_TASK` 和 `END_TASK` 之间的代码是局部代码。

如果在任务中需要暂时终止任务，以开启另一个任务，例如临时创建一个窗口，则不能使用宏，必须将其写成上面的展开形式。`BEGIN_TASK` 系列宏的定义只是为了能在一些比较简单的情况下写得更加方便。

从上面的代码可以看出，有可能 `EasyWin32::SetWorkingWindow()` 是可能失败的，所以如果任务执行失败对后续的代码会产生影响，那么一定要在后续的代码中根据具体情况判断是否成功执行了任务，或者使用 上面展示的宏展开写法，并加上一个 `else` 语句来处理启动任务失败的情况；否则的话，可能导致程序某些时候突然意外崩溃，这时再排查代码错误是很麻烦的。

调用 `BEGIN_TASK()` 系列宏来表示一个任务的起始和结束，是为了协调多个窗口同时存在的情况，因为多个窗口可能抢占绘图权，所以您需要在执行一个任务时进行标识，使得多个窗口有序绘图。

如果是在顺序代码结构中，很多时候，用户操作导致的关闭窗口，或者是窗口拉伸导致的画布调整，也很有可能直接掐断您的绘图任务，如果您不使用这些宏来标识您正在进行绘图任务的话，就会导致两个任务互相冲突，导致程序崩溃。

### 绘图缓冲

EasyWin32 默认就是双缓冲的，所以无需再调用 EasyX 原生的 `BeginBatchDraw` 系列函数。

每次任务结束时，会刷新绘图缓冲，但是由于目前程序没有在窗口过程函数的 WM_PAINT 消息中绘图，所以即便刷新了绘图缓冲，也不会立即显示到窗口上。这就需要在每次绘图任务结束时，再调用 `FLUSH_DRAW（）` 宏（等价于 `EasyWin32::EnforceRedraw()`）以重绘窗口。

注意，如果在一个任务中完成了绘图，现在需要强制重绘，则 `FLUSH_DRAW()` 应该紧接着 `END_TASK()` 执行；而如果仅仅是需要在程序执行的某个阶段刷新窗口，则需要在任务内部执行 `FLUSH_DRAW()`。

这是因为任务中绘制的内容会在任务结束（`END_TASK()`）时输出缓冲，而如果在任务中进行了绘制，却在任务结束之前调用 `FLUSH_DRAW()`，相当于还没输出绘图缓冲就发送窗口重绘消息，这样就没有把新的绘制内容输出到窗口上。又因为 `FLUSH_DRAW()` 是针对当前活动窗口的，所以该语句需要在任务结束后立即执行，否则如果在多窗口的情况下，当前窗口的任务结束后，活动窗口可能会改变，此时再调用 `FLUSH_DRAW()` 就会操作到错误的窗口。

### 鼠标、键盘消息

消息队列方面，EasyWin32 实现了对 EasyX 的全兼容。

EasyWin32 兼容旧版鼠标消息 `MOUSEMSG`，同时支持 `ExMessage` 系列消息。

按键消息也可以直接使用 `_getch` 一系列函数获取。

注意：使用鼠标消息相关函数时，同样也需要使用 `BEGIN_TASK` 系列宏来标识进行一个任务。

有些人的程序可能在一个主循环中进行，为了响应鼠标操作，每次循环中都用一个 if 语句读取鼠标消息再做处理，这样的做法完全错误！正确的做法就是本例中的写法，使用 while 语句一次性读完全部残留在缓冲区的鼠标消息，至于为什么要这么做，请看 [这篇文章](https://codebus.cn/zhaoh/handle-mouse-messages-correctly) 的解释，此处不再赘述。

## 例子 3：用上 Win32 控件

提示：本例运用了一些 Win32 编程知识。

如果您希望在程序中使用 Win32 控件，则您需要写一个简化版的 Win32 过程函数，就像下面这样：

```cpp
#include "EasyWin32.h"

// 窗口过程函数
bool WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
	switch (msg)
	{
	default: return true; break;	// 使用默认方法处理其余消息
	}
	return false;
}

int main()
{
	EasyWin32::initgraph_win32(640, 480, 0, L"", WndProc);	// 创建窗口，并指定窗口过程函数
	EasyWin32::init_end();					// 在 Win32 消息派发的代码结构下，创建完窗口后必须用此函数阻塞
	return 0;
}

```

窗口过程函数的返回值是布尔类型，标识是否需要使用系统默认方法处理该消息。

需要注意的是：

1. 接受 WM_CREATE 消息时，wParam 和 lParam 是空的，你无法获得 CREATESTRUCT 结构体信息

2. 接受 WM_CLOSE 消息时，返回 true 或 false 表示是否关闭窗口，但如果关闭窗口，您无需编写销毁窗口的代码

对于这段代码，由于 `main` 函数中调用了 `EasyWin32::init_end()` 函数进行阻塞，这个函数会自动判断窗口是否还存在，所以不需要再使用 `EasyWin32::isAnyWindow()` 进行判断。

现在还没有开始创建控件，我们在 `WndProc` 中创建一个按钮吧（可以是其他控件，学习 Win32 编程后可以实现其他控件，这里以按钮举例）：

```cpp
#include "EasyWin32.h"

// 控件 ID
#define IDC_BTN 100

// 存储文本
wchar_t str[128] = { 0 };

// 窗口过程函数
bool WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
	switch (msg)
	{
	case WM_CREATE:

		// 创建按钮
		CreateWindow(L"button", L"Button",
			WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
			200, 100, 100, 60,
			hwnd, (HMENU)IDC_BTN, hInstance, NULL);

		break;

	case WM_PAINT:

		BEGIN_TASK_WND(hwnd);	// 将绘图窗口设为自己，并启动一次绘图任务
		setbkcolor(0xf0f0f0);	// 设置背景色
		settextcolor(BLUE);		// 设置文本色
		cleardevice();			// 清屏
		outtextxy(20, 20, str);	// 输出文字
		END_TASK();				// 结束此次绘图任务

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN:	// 按下按钮

			BEGIN_TASK_WND(hwnd);
			wsprintf(str, L"按下了按钮");
			FLUSH_DRAW();		// 在非 WM_PAINT 消息区域，如需要立即刷新，需要强制重绘
			END_TASK();

			break;
		}

	default: return true; break;	// 使用默认方法处理其余消息
	}
	return false;
}

int main()
{
	EasyWin32::initgraph_win32(640, 480, 0, L"", WndProc);	// 创建窗口，并指定窗口过程函数
	EasyWin32::init_end();									// 在 Win32 消息派发的代码结构下，创建完窗口后必须用此函数阻塞
	return 0;
}
```

最后推荐一个 Win32 学习网站：http://winprog.org/tutorial/zh/start_cn.html 。

## 例子 4：关闭窗口，弹窗确认

在这个例子中，在 `main` 函数绘图的同时，自定义的窗口过程函数响应了 WM_CLOSE （窗口关闭）消息，实现了弹窗确认关闭窗口的功能。

源代码 [Sample3](./samples/Sample3/main.cpp)：

```cpp
// 备注：绘图部分源码来自 https://codebus.cn/dudugang/drawing

#include "EasyWin32.h"
#include <time.h>

int x;								// X 轴坐标
int y;								// y 轴坐标
int g_multiples = 0;				// 倍数：作用是对弧度角进行改变
float g_PI = (float)3.1415;			// 圆周率
float g_radianAngle;				// 起始角的弧度
int w = 830;						// 窗口宽
int h = 580;						// 窗口高

// 窗口过程函数
bool WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
	switch (msg)
	{
	case WM_CLOSE:

		// 关闭窗口时提示是否确认关闭
		if (MessageBox(hwnd, L"确定关闭窗口？", L"提示", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	default:	return true;	break;
	}
}

int main()
{
	// 创建绘图窗口，并指定窗口过程函数
	EasyWin32::initgraph_win32(w, h, false, L"", WndProc);

	// 设置背景色
	setbkcolor(WHITE);

	while (true)
	{
		// 启动一个绘图任务
		// 由于只创建了一个窗口，所以不需要指定目标绘图窗口
		// 否则就需要调用 BEGIN_TASK_WND() 宏，指定目标绘图窗口
		BEGIN_TASK();

		// 进行一些绘制，由于调用的都是 EasyX 绘图函数，不再注释
		cleardevice();
		setcolor(RGB(185, 230, 0));
		setfillcolor(RGB(185, 230, 0));
		solidrectangle(10, 10, 820, 570);
		for (size_t j = 0; j < 11; j++)
		{
			for (size_t i = 0; i < 16; i++)
			{
				x = 15 + 50 * i;
				y = 15 + 50 * j;
				g_radianAngle = 0 + g_multiples * g_PI / 4;
				i < 15 ? g_multiples++ : g_multiples = g_multiples;
				rectangle(x, y, x + 50, y + 50);
				setfillcolor(RGB(250, 250, 250));
				solidcircle(x + 25, y + 25, 20);
				setfillcolor(RGB(80, 80, 80));
				solidpie(x + 5, y + 5, x + 50 - 5, y + 50 - 5, g_radianAngle, g_radianAngle + g_PI);
				setfillcolor(RGB(158, 2, 251));
				solidcircle(x + 25, y + 25, 15);
			}
		}

		// EasyWin32 默认使用双缓冲绘图，此处输出绘图缓冲
		// 注意：一段绘图任务结束，必须以此宏结尾（即 BEGIN_TASK 和 END_TASK 必须连用）
		END_TASK();

		// 不在窗口过程函数的 WM_PAINT 消息内绘图时，必须强制重绘
		// 由于没有自定义窗口过程函数，所以当然也要调用此宏强制重绘
		FLUSH_DRAW();

		while (true)
		{
			// 窗口大小改变时重绘
			if (EasyWin32::isWindowSizeChanged())
			{
				break;
			}

			// 若窗口被关闭，则结束程序
			if (!EasyWin32::isAnyWindow())
			{
				return 0;
			}

			Sleep(10);
		}
	}
}
```

## 例子 5：创建任务栏托盘图标

以下的示例将会创建两个窗口，每个窗口创建了一个 Windows 托盘图标。

提醒：一般一个程序只需要一个托盘图标就足矣。在 EasyWin32 提供的托盘接口中，托盘图标和窗口图标是绑定的。

源代码 [Sample_Tray](./samples/Sample_Tray/main.cpp)：

```cpp
////////////////////////////
//
//	EasyWin32 使用示例
//
//	快速创建托盘图标
//

#include "EasyWin32.h"

// 定义托盘菜单的选项 ID
#define IDC_A	101
#define IDC_B	102
#define IDC_C	103
#define IDC_D	104

// 自定义过程函数
bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, HINSTANCE hInstance)
{
	// 标识是否单击过了托盘图标
	static bool flag = false;

	switch (msg)
	{
		// 处理绘图消息
	case WM_PAINT:
		if (EasyWin32::SetWorkingWindow(hWnd))
		{
			EasyWin32::BeginTask();
			setbkcolor(SKYBLUE);
			cleardevice();
			settextstyle(32, 0, L"system");
			settextcolor(BLACK);
			outtextxy(20, 20, L"Windows Tray Icon Creation Sample");
			if (flag)
			{
				settextstyle(26, 0, L"system");
				settextcolor(PURPLE);
				outtextxy(20, 60, L"You pressed the tray icon.");
			}
			EasyWin32::EndTask();
		}
		break;

		// 处理托盘消息
	case WM_TRAY:
	{
		switch (lParam)
		{
			// 左键按下托盘图标
		case WM_LBUTTONDOWN:
			flag = true;					// 标记按下
			EasyWin32::EnforceRedraw();		// 立即重绘
			break;
		}
	}
	break;
	}

	return true;
}

// 处理托盘菜单事件
void OnMenu(UINT uId)
{
	switch (uId)
	{
	case IDC_A:
		MessageBox(GetHWnd(), L"A", L"Tip", MB_OK);
		break;
	case IDC_C:
		MessageBox(GetHWnd(), L"C", L"Tip", MB_OK);
		break;
	case IDC_D:
		MessageBox(GetHWnd(), L"D", L"Tip", MB_OK);
		break;
	}
}

int main()
{
	// 创建一个自定义过程函数的绘图窗口
	HWND hWnd1 = EasyWin32::initgraph_win32(640, 480, EW_NORMAL, L"EasyWin32 & Tray", WndProc);

	// 制作菜单
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, IDC_A, L"选项 A");
	AppendMenu(hMenu, MF_STRING, IDC_B, L"选项 B");
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);				// 分隔符
	AppendMenu(hMenu, MF_STRING, IDC_C, L"选项 C");
	EnableMenuItem(hMenu, IDC_B, MF_GRAYED);				// 禁用一个选项

	EasyWin32::CreateTray(L"The first");					// 创建托盘
	EasyWin32::SetTrayMenu(hMenu);							// 设置托盘菜单
	EasyWin32::SetTrayMenuProcFunc(OnMenu);					// 设置托盘菜单响应函数

	//
	// 注意：
	//	此示例中为了演示而创建了两个窗口，分别有一个托盘图标。
	//	而在实际应用中几乎不会这样做，一个程序最多一个托盘就够了。
	//	所以托盘图标已固定设置为窗口图标。
	//	EasyWin32 对于托盘功能只是做了简单封装，如果有更多需要，也可以自行实现。
	//

	// 创建第二个窗口
	EasyWin32::initgraph_win32(200, 120, EW_NORMAL, L"The Second Window");

	// 此时存在两个窗口，为防止冲突，启动任务
	EasyWin32::BeginTask();
	{
		setbkcolor(LIGHTSKYBLUE);
		cleardevice();
		settextcolor(BLACK);
		outtextxy(20, 20, L"The Second Window");

		// 制作菜单
		HMENU hMenu2 = CreatePopupMenu();
		AppendMenu(hMenu2, MF_STRING, IDC_D, L"选项 D");

		EasyWin32::CreateTray(L"The second");				// 创建托盘
		EasyWin32::SetTrayMenu(hMenu2);						// 设置托盘菜单
		EasyWin32::SetTrayMenuProcFunc(OnMenu);				// 设置托盘菜单响应函数
	}
	EasyWin32::EndTask();
	EasyWin32::EnforceRedraw();

	EasyWin32::SetWorkingWindow(hWnd1);						// 将任务权交还第一个窗口
	SetForegroundWindow(hWnd1);

	EasyWin32::init_end();									// 阻塞等候
	return 0;
}
```

托盘效果：

![示例图片](./screenshot/8.png)

---
*示例部分已经结束，以下是零碎的注意事项。*

## 关于 IMAGE* 的空指针

由于 EasyWin32 绕开了 EasyX 原生的 `initgraph` 函数，所以在调用需要 `IMAGE` 对象指针的函数时，不可以传入 `NULL`，而需要传入您的窗口对应的 `IMAGE` 对象指针（使用 `GetWorkingImage()` 获取）。

诸如以下函数都默认会传入 `IMAGE* pImg = NULL`：`GetImageBuffer`，`SetWorkingImage`，`GetImageHDC` 等等，调用这些函数时都需要传入具体的指针。

## 添加程序图标

EasyWin32 将自绘一个 EasyX 的图标作为程序图标，这个图标模仿的是 EasyX 官网的网页 icon。

如果想要使用自己的图标，必须先在程序第一次创建窗口前就调用 `EasyWin32::SetCustomIcon` 函数，并传入自己的图标资源 ID。

### 在 Visual Studio 中添加图标资源

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

即可。

## 在原有 EasyX 项目上使用 EasyWin32

在大多数情况下，在原有 EasyX 项目上使用 EasyWin32 是很轻松的。

概括地说，只需要包含 EasyWin32 的头文件，然后在原先的绘图部分代码前后使用 `BEGIN_TASK` 系列宏或函数标识这是一段绘图任务即可。以下的内容是对此更详细的解释。

注意：目前 EasyWin32 仅支持 ExMessage 的鼠标消息获取，其他消息无法获取，可能造成兼容性问题，请使用 `_getch` 系列函数代替。

**应用 EasyWin32**

首先将 `#include <graphics.h>` 或 `#include <easyx.h>` 替换为 `#include "EasyWin32.h"`。

**标识绘图任务区块**

如果您的代码使用了批量绘图，那么在使用了批量绘图的地方必然存在 `FlushBatchDrawing()` 函数，比如下面这段代码：

```cpp
while(true)
{
	for (int i = 0; i < 10 ; i++)
	{
		line(10, i*10, 110, i*10);
	}
	Sleep(20);
	outtextxy(0, 0, L"Hello");
	FlushBatchDraw();	//<- here

	cleardevice();
}
```

那么，在绘图开始的地方加上 `BEGIN_TASK();`，然后在绘图结束的地方，也就是 `FlushBatchDraw()` 的前面，加上 `END_TASK();`，如下：

```cpp
while(true)
{
	BEGIN_TASK();	//<- add

	for (int i = 0; i < 10 ; i++)
	{
		line(10, i*10, 110, i*10);
	}
	Sleep(20);
	outtextxy(0, 0, L"Hello");

	END_TASK();	//<- add
	FlushBatchDraw();

	cleardevice();
}
```

此外，您必须确保任何的绘图操作都被放到了 `BEGIN_TASK();` 和 `END_TASK();` 的中间，就像下面这样：

```cpp
while(true)
{
	BEGIN_TASK();

	cleardevice();	//<- Move here
	for (int i = 0; i < 10 ; i++)
	{
		line(10, i*10, 110, i*10);
	}
	Sleep(20);
	outtextxy(0, 0, L"Hello");

	END_TASK();
	FlushBatchDraw();
}
```

而原先代码中的 `FlushBatchDraw()` 函数是不需要改的，因为会被宏自动替换为 `FLUSH_DRAW()`，`EndBatchDraw()` 同样也被替换为 `FLUSH_DRAW()`，至于 `BeginBatchDraw()`，他会被替换为空。

所以一般情况下您不需要删除任何代码，只需要在每个 `FlushBatchDraw()` 函数前，把整个绘图任务用 `BEGIN_TASK();` 和 `END_TASK();` 包起来即可。

**任务中存在无效的占用时间的代码**

这很关键：如果说 `Sleep()` 的调用只是为了降低 CPU 占用，则不要将其放在一个绘图任务中，而应该挪到外面来，如下：

```cpp
while(true)
{
	BEGIN_TASK();

	cleardevice();
	for (int i = 0; i < 10 ; i++)
	{
		line(10, i*10, 110, i*10);
	}
	outtextxy(0, 0, L"Hello");

	END_TASK();
	FlushBatchDraw();
	
	Sleep(20);	//<- Move here
}
```

因为如果不把 `Sleep()` 放出来，那么这个循环就会变得非常紧密，在 `END_TASK();` 刚结束不久，马上就会 `BEGIN_TASK();`，相当于程序几乎毫无空隙地占用了绘图权，这可能会导致用户操作（例如拉伸窗口）需要等待很久才能做出反应，因为 EasyWin32 必须等待程序没有占用绘图权的时候才能处理这些消息（例如拉伸窗口引发的画布大小调整）。

所以在多次绘图任务的间隙中插入 `Sleep()` 很必要，同时也能降低 CPU 占用，但是不要把 `Sleep()` 放在绘图任务中进行。如果必须要这么做，可以在 `Sleep()` 前后断开这个任务，将其一分为二，如下：

```cpp
while(true)
{
	BEGIN_TASK();

	cleardevice();
	for (int i = 0; i < 10 ; i++)
	{
		line(10, i*10, 110, i*10);
	}
	
	END_TASK();	//<- interrupt the task
	FlushBatchDraw();
	
	Sleep(3000);	//<- sleep
	
	BEGIN_TASK();	//<- start the task again
	
	outtextxy(0, 0, L"Hello");

	END_TASK();
	FlushBatchDraw();
}
```

**其他情况**

在上述情况下，原先的代码中使用了 `FlushBatchDraw()`，这样可以快速找到每个集中的绘图代码块，就不再需要重新找了。如果您原先的代码中没有使用 `FlushBatchDraw()`，那就得把代码中每个集中的绘图代码块找出来，然后再用上述方法标识绘图任务块。

温馨提示：出于兼容原生 EasyX 的目的，如果您在 EasyWin32 中使用了 `initgraph()` 函数来创建窗口，那么您就不再需要手动检测窗口的关闭，只要绘图窗口被销毁，程序就会自动退出。

## 自定义窗口样式

如果您想改变窗口样式，例如取消最大化按钮，禁止用户拉伸窗口，您可以使用 Win32 API `SetWindowLong` 函数，这需要一些 Win32 知识。

`SetWindowLong` 函数不仅仅能够设置窗口的样式，还可以做很多其他事情，但是设置窗口样式是比较常用的功能，所以 EasyWin32 将其做了简单封装。就设置窗口样式而言，您可以调用 `EasyWin32::GetWindowStyle()` 获取当前窗口样式，调用 `EasyWin32::SetWindowStyle()` 设置当前窗口样式（或另一套适用于 ExStyle 的函数，详见头文件）。

EasyWin32 预定义了数个宏用以设置一些常见的窗体属性，详见头文件。

## 代码中的常见问题整合

1. 创建窗口后没有判断窗口是否被关闭
2. 调用 EasyX 库函数前后没有使用 `BEGIN_TASK()` 系列宏或函数
3. `EasyWin32::BeginTask()` 可能启动任务失败。对于必须正确执行的任务，没有处理任务启动失败的情况（此情况也不能使用 `BEGIN_TASK()` 宏，它无法返回任务是否执行成功）
4. 循环结构的主程序中，每次读取鼠标操作使用了 `if` 语句读取。应改为使用 `while`，一次读完所有消息
5. 向 EasyX 库函数传入空的 IMAGE 指针来代指主画布，应使用 `GetWorkingImage()` 获取主画布地址
6. 任务耗时过长。可能是因为任务中存在不必要的延时代码（如 `Sleep`）、耗时的计算、死循环等等，或在两个任务之间几乎无间隙


