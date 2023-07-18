> [回到教程目录](./index.md)

# 配置 HiEasyX

## 编译环境

* Windows 7 及以上，Windows Sever 2008 R2 及以上（推荐使用 Windows 10/11）
* Visual Studio 2013 及以上（推荐使用 Visual Studio 2022）
* 推荐 EasyX 版本：EasyX_20220901

> **注意：**
>
> * 需要编译器支持 C++ 14
> 
> * HiEasyX 对于 Unicode 和 MBCS 字符集均支持
> 
> * 必须已经配置 EasyX 图形库，如未配置，请访问图形库官网 https://easyx.cn
>
> * 对于 MinGW 等 GCC 编译器，需提前安装 EasyX-for-MinGW：https://codebus.cn/bestans/easyx-for-mingw

## 配置 HiEasyX 到项目中

1. 首先下载整个仓库到你的计算机，如果想使用旧版 HiEasyX，请在 [Release](https://github.com/zouhuidong/HiEasyX/releases) 页面中下载旧版的源码。

接下来要将 HiEasyX 配置到你的项目中，只需要：

1. 创建一个项目
2. 复制仓库项目中的 `./HiEasyX/HiEasyX.h` 和 `./HiEasyX/HiEasyX/` 整个文件夹到你的项目目录下（和项目源码放一起即可）
3. 将刚才复制的文件和文件夹加入到您的项目中（拖入编译器的项目资源管理器即可，或可以在项目文件手动选择添加这些文件）

对于 MinGW 等 GCC 编译器，需额外添加配置：需要添加链接选项: `-leasyx -lmsimg32 -lgdiplus -lgdi32 -lwinmm`

最后在源码中包含此库：

```cpp
#include "HiEasyX.h"
```

然后编写代码，测试运行。

> **温馨提示**
> 
> 由于 HiEasyX 源码文件较多，故建议在 Visual Studio 项目资源管理器中创建 HiEasyX 筛选器，将上述文件和文件夹都拖入此筛选器中，这样可以使项目结构更整洁。第一次编译需要编译全部文件，所以耗时较长，后面就不会了。

测试代码：

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

> [回到教程目录](./index.md)
