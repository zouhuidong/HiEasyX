> [回到教程目录](./index.md)

# 自定义程序图标

在 EasyX 中，只要在 Visual Studio 项目中加入图标资源，程序就会自动加载你的图标。在 HiEasyX 中也差不多。

HiEasyX 默认为程序加载 HiEasyX 图标，如果想要使用其它图标，也需要先在 VS 项目中加入图标资源，和 EasyX 不同的是，你还需要在第一次创建窗口前调用一次 `hiex::SetCustomIcon` 函数，这样就行了。

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

> [回到教程目录](./index.md)
