# HiEasyX
![Release](https://img.shields.io/github/v/release/zouhuidong/仓库名)
[![Blog](https://img.shields.io/badge/blog-huidong.xyz-green.svg)](http://huidong.xyz)
[![EasyX](https://img.shields.io/badge/graphics-EasyX-orange.svg)](https://easyx.cn)
![Stars](https://img.shields.io/github/stars/zouhuidong/仓库名)
![Forks](https://img.shields.io/github/forks/zouhuidong/仓库名)
![Downloads](https://img.shields.io/github/downloads/zouhuidong/仓库名/total)

HiEasyX 基于 EasyX 实现了在窗口、绘图、UI 等诸多方面的全面升级。

HiEasyX 的目的只有一个——让 EasyX 更易用。

> [查看教程](./Tutorial.md)

> [更新日志](./ChangeLog.md)

*前身：[EasyWin32](github.com/zouhuidong/EasyWin32)*

## Why HiEasyX

EasyX 从设计之初，它就仅仅是一个图形库，不涉及其它方面的功能。

当我们用 EasyX 制作软件或游戏时经常会遇到下列问题：

* 创建多个绘图窗口
* 完整的控件库
* 透明通道
* 图层
* 声音
* 播放 gif 动画

等等……

您是否曾经为它们苦恼？HiEasyX 可能是一个更完美的解决方案。

HiEasyX 支持创建多绘图窗口，拥有相对完善的控件库，支持透明通道，封装了画布、图层和场景。

您想用 EasyX 更高效地制作软件或游戏吗？HiEasyX 或许是适合您的选择。

> HiEasyX 不是独立的一个库，它是 EasyX 的充分扩展。它使用 C++，或许不适合 EasyX 的初学者。

## 支持功能

* HiEasyX
  + HiWindow：窗口支持
    - 支持创建多绘图窗口
    - 支持窗口拉伸
    - 支持 Win32 控件
    - 支持自定义窗口过程函数
    - 支持快速创建托盘
  + HiGUI：自绘控件（未完工）
    - Static
    - Button
    - ProgressCtrl
    - ScrollBar
    - Page
  + HiSysGUI：系统控件封装
    - SysStatic
    - SysButton
    - SysCheckBox
    - SysRadioButton
    - SysGroup
    - SysGroupBox
    - SysEdit
    - SysComboBox
  + HiCanvas：EasyX 绘图函数的 C++ 封装
    - 支持透明通道
    - 和 HiWindow 完美融合
    - 使用更加方便
  + HiGif：动图支持（改编自：依稀_yixy）
  + HiMusicMCI：声音 API 封装（原作者：悠远的苍穹 <2237505658@qq.com>）
  + HiMouseDrag：鼠标拖动消息封装
    
## 编译环境

* Windows 10
* VisualStudio 2022
* EasyX_20220610

> *您必须已经配置 EasyX 图形库，如未配置，请访问图形库官网 https://easyx.cn*

**注意：暂不支持 MingW 和 GCC 编译器，请使用 Visual Studio 编译项目。**

## 配置此库

您可以直接打开仓库中的项目并编译运行。下面是在您的项目中配置此库的方法：

1. 下载仓库到本地
2. 创建一个 Visual Studio 项目
3. 复制仓库项目中的 `./HiEasyX/HiEasyX.h` 和 `./HiEasyX/HiEasyX/` 整个文件夹到您的项目目录下
4. 将刚才复制的文件和文件夹加入到您的 Visual Studio 项目中（拖入 Visual Studio 的项目资源管理器即可）
5. 编写代码，编译运行

> **温馨提示**
> 
> 由于 HiEasyX 源码文件较多，故建议在 Visual Studio 项目资源管理器中创建 HiEasyX 筛选器，将上述文件和文件夹都拖入此筛选器中，这样可以使项目结构更整洁。第一次编译需要编译全部文件，所以耗时较长，后面就不会了。

测试代码：

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

## 示例程序截屏

以下的示例程序源码可以在 `./Samples/` 中找到

<div align=center>
<img src="./screenshot/start.png"><br>
<b>Release 模式启动动画效果</b>
</div><br>

<div align=center>
<img src="./screenshot/balls1.png"><br>
<b>透明通道 - 小球示例（1）</b>
</div><br>

<div align=center>
<img src="./screenshot/balls2.png"><br>
<b>透明通道 - 小球示例（2）</b>
</div><br>

<div align=center>
<img src="./screenshot/overview.png"><br>
<b>多窗口 & Win32 控件 & Canvas 绘图效果</b>
</div><br>

<div align=center>
<img src="./screenshot/sysgui.png"><br>
<b>系统控件</b>
</div><br>

<div align=center>
<img src="./screenshot/gui.png"><br>
<b>部分自绘控件</b>
</div><br>

## 立即开始

> [开始使用 HiEasyX](./Tutorial.md)
