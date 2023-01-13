> [回到教程目录](./index.md)

# 在原有 EasyX 项目上使用 HiEasyX

> 要在原有项目上应用 HiEasyX 并非难事，最主要的改动就是在每个绘图代码块前后写上开启和关闭窗口任务的代码。

## Step 1: 改头文件

首先确保你已在项目中配置 HiEasyX。

然后，只需要将原先包含 EasyX 头文件的代码改成包含 HiEasyX 头文件的代码。

```cpp
// 将这个
#include <easyx.h>	// 可能是 #include <graphics.h>

// 改为
#include "HiEasyX.h"
```

## Step 2: 标识窗口任务

如果你希望在项目中使用原生 EasyX 窗口，那么这一步就可以省了，你只需要在 `HiDef.h` 中启用 `_NATIVE_EASYX_` 宏定义即可。

但大部分情况下，你可能需要用到 HiEasyX 提供的多窗口和 Win32 控件等模块，那么就只要在绘图代码段的前后分别加上：

```cpp
BEGIN_TASK();

// 绘图代码

END_TASK();

// 如果需要即时刷新窗口，则再加上
REDRAW_WINDOW();
```

即可。

如果原项目中使用了双缓冲，则一定会调用 `FlushBatchDraw` 函数，这样就能更方便地找出每个绘图代码块了，否则就需要将每个绘图代码块手动找出来。

窗口任务相关内容已在前面的教程中交代清楚，故不再重复。注意不要将不必要的代码也放入窗口任务中，详见 [这里](wnd_basic.md)。

> [回到教程目录](./index.md)
