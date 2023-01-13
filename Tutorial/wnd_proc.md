> [回到教程目录](./index.md)

# 自定义窗口过程函数

> **注意：**
>
> 自定义窗口过程函数的前提是你有 Win32 基础。

为了维护 HiWindow 的正常运行，自定义窗口函数并不是使用 Win32 API 设置窗口的过程函数，您应该使用 `hiex::SetWndProcFunc()` 或 `hiex::Window::SetProcFunc()`。

自定义的过程函数的签名和普通的 Win32 过程函数相同，唯一的区别就是，返回 `DefWindowProc()` 时，改为返回 `HIWINDOW_DEFAULT_PROC` 宏标志。

下面是一段自定义过程函数的示例代码：

```cpp
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	case WM_USER_REDRAW:	// 特别注意：若要处理 WM_PAINT 消息，则需要同时处理 WM_USER_REDRAW 消息
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

其实很简单，看了这个示例你应该就明白怎么做了。

值得一提的是，用户调用 `hiex::RedrawWindow` 函数或者 `REDRAW_WINDOW` 宏时发出的重绘消息是 `WM_USER_REDRAW`，这个消息是 HiEasyX 自定义的一个消息。所以说如果你想在窗口过程函数中处理 `WM_PAINT` 消息，那么你还应该处理 `WM_USER_REDRAW` 消息，这样才能处理到所有的绘制消息。

> [回到教程目录](./index.md)
