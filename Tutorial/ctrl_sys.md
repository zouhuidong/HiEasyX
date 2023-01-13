> [回到教程目录](./index.md)

# 使用封装的 Win32 控件

HiEasyX 封装了常用 Win32 控件，这个控件模块被称为 HiSysGUI。

目前支持的控件类型 *（此文档可能更新不及时）* ：

* 分组框
* 静态文本（图像）
* 按钮
* 复选框
* 单选框
* 编辑框
* 组合框

一般情况下，这些控件已经足够。而且，您也可以自定义窗口过程函数，直接调用其它 Win32 控件。

接下来此教程以几个常用的控件为例，演示如何使用一部分控件，对于其他的控件，使用方式都是大同小异。你可以在 `HiSysGUI.h` 中看到所有封装好的 Win32 控件。

## 简单快速地创建控件

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
<img src="../screenshot/fast_btn.png"><br>
<b>创建按钮</b>
</div><br>

没错！使用按钮就是这么容易。

还可以在按钮中添加图片，像这样：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);
	hiex::SysButton btn(wnd.GetHandle(), 100, 85, 100, 30, L"Button");

	// 创建画布，绘制绿色填充圆
	hiex::Canvas canvas(30, 22);
	canvas.Clear(true, 0xe1e1e1);
	canvas.SolidCircle(15, 10, 10, true, GREEN);

	// 添加按钮图像
	btn.Image(true, &canvas, true);

	hiex::init_end();
	return 0;
}
```

<div align=center>
<img src="../screenshot/fast_btn_2.png"><br>
<b>添加按钮图片</b>
</div><br>

> **提示：**
> 
> 代码中使用了 Canvas 绘制按钮图像，如果使用 IMAGE 同样可以。

## 处理控件消息

如果要响应按钮消息，可以使用 `RegisterMessage` 方法，或者使用 `GetClickCount` 函数获取按钮点击次数。

例如，使用 `GetClickCount` 函数获取按钮点击次数：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);

	hiex::SysButton btn(wnd.GetHandle(), 100, 85, 100, 30, L"Button");
	
	// 窗口存在时，程序才保持运行
	while (wnd.IsAlive())
	{
		// 如果按钮的点击次数不为 0，说明用户已点击按钮
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

我们还可以看看编辑框，像这样：

```cpp
#include "HiEasyX.h"

int main()
{
	hiex::Window wnd(300, 200);
	hiex::SysEdit edit;	// 编辑框

	// 预设样式为支持多行输入，因为有的控件样式必须在创建之前就指定
	hiex::SysEdit::PreStyle pre_style;
	pre_style.multiline = true;
	pre_style.vscroll = true;
	pre_style.hscroll = true;
	pre_style.auto_vscroll = true;
	pre_style.auto_hscroll = true;
	edit.PreSetStyle(pre_style);

	edit.Create(wnd.GetHandle(), 10, 10, 280, 180, L"Multiline Edit Box\r\n\r\nEdit here");

	// 设置编辑框字体
	edit.SetFont(24, 0, L"微软雅黑");

	hiex::init_end();
	return 0;
}
```

<div align=center>
<img src="../screenshot/fast_edit.png"><br>
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
	edit.PreSetStyle({true, false, true, true});	// 预设样式可以不用创建结构体，直接简写成这样
	edit.Create(wnd.GetHandle(), 10, 10, 280, 140, L"Type here~");
	edit.SetFont(24, 0, L"微软雅黑");

	// 按钮
	hiex::SysButton btn;
	btn.Create(wnd.GetHandle(), 190, 160, 100, 30,L"Submit");

	while (wnd.IsAlive())
	{
		// 按下按钮时，弹窗显示输入的文本
		if (btn.IsClicked())
			MessageBox(wnd.GetHandle(), edit.GetText().c_str(), L"Submit", MB_OK);
		Sleep(50);
	}

	return 0;
}
```

<div align=center>
<img src="../screenshot/fast_edit_2.png"><br>
<b>获取编辑框文本</b>
</div><br>

还可以设置文字颜色、背景颜色、密码框、左中右对齐方式、仅数字输入、禁用控件，等等，不一一列举。这个教程不可能面面俱到，也有可能更新延迟，如果您想具体了解每个控件，可以看看它们的声明，此处不再展开。

> 参阅 [文档](https://zouhuidong.github.io/HiEasyX/doxygen/html/class_hi_easy_x_1_1_sys_control_base.html)。

下面这个示例用到的控件比较全面，可以帮您更深入地了解 HiSysGUI：

<div align=center>
<img src="../screenshot/overview.png"><br>
<b>控件示例</b>
</div><br>

> 在此查看此示例的 [源代码](../Samples/Recommend/GUISample.cpp)


> [回到教程目录](./index.md)
