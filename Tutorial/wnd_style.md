> [回到教程目录](./index.md)

# 自定义窗口样式

如果您想改变窗口样式，例如取消最大化按钮，禁止用户拉伸窗口，您可以使用 Win32 API `SetWindowLong` 函数，这需要一些 Win32 知识。

还有更简便的方式，直接使用 `hiex::SetWindowStyle()` 或 `hiex::Window::SetStyle()`。

如果是设置窗口标题，可以用 `hiex::SetWindowTitle()` 或 `hiex::Window::SetTitle()`。

HiEasyX 定义了如下宏用于快速设置窗口样式

* `EnableResizing` 设置是否允许窗口拉伸
* `EnableSystemMenu` 设置是否启用系统标题栏按钮
* `EnableToolWindowStyle` 设置是否启用窗口的工具栏样式

等等。更多请参见 [文档](https://zouhuidong.github.io) 和 `HiWindow.h`。

> [回到教程目录](./index.md)
