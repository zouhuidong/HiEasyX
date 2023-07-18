# HiEasyX
![Release](https://img.shields.io/github/v/release/zouhuidong/HiEasyX)
[![Blog](https://img.shields.io/badge/blog-huidong.xyz-green.svg)](http://huidong.xyz)
[![EasyX](https://img.shields.io/badge/graphics-EasyX-orange.svg)](https://easyx.cn)
![Stars](https://img.shields.io/github/stars/zouhuidong/HiEasyX)
![Forks](https://img.shields.io/github/forks/zouhuidong/HiEasyX)

HiEasyX 是基于 EasyX 的扩展库，支持创建多窗口、透明抗锯齿绘图、系统 UI 组件等等。

**近期重大更新**：同时支持 Unicode 和 MBCS 字符集  
**近期重大更新**：同时支持 MSVC、MinGW、TDM-GCC 等编译器  
[详情](https://github.com/zouhuidong/HiEasyX/releases)

---

HiEasyX 和 EasyX 的契合度很高，它能让你在使用 EasyX 的过程中如虎添翼~

> [HiEasyX 教程目录](./Tutorial/index.md)
>
> [Alan-CRL 的教程](https://hiex.alan-crl.top/)

> [在线文档](https://zouhuidong.github.io/HiEasyX)

> [更新日志](./ChangeLog.md)

*前身：[EasyWin32](https://github.com/zouhuidong/EasyWin32)*

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
    - 支持 GDI+ 透明和抗锯齿绘制
    - 和 HiWindow 完美融合
  + HiGif：动图支持（改编自：依稀_yixy）
  + HiMusicMCI：声音 API 封装（原作者：悠远的苍穹 <2237505658@qq.com>）
  + HiMouseDrag：鼠标拖动消息封装

以上模块如非原创，均已特别标注。

**如果你有什么需求，请分享给我们~**

### 鸣谢

依稀_yixy

悠远的苍穹

## 立即开始

> [HiEasyX 教程目录](./Tutorial/index.md)
>
> [Alan-CRL 的教程](https://hiex.alan-crl.top/)

## 如何提问？

你可以直接在这个仓库提出 issue，例如起一个这样的 issue 标题：

> [ 问题咨询 ] 请问如何使用 Canvas 贴图？

或者，你可以加我的 QQ（1442701429），备注一下你要问 HiEasyX 的哪方面的问题。

由于学业繁忙，所以我很难及时回复，一般来说，最多不会超过一个月。不过，如果缘分好的话，我可能一分钟之内就会回复你，嘻嘻。

或者可以加 Alan-CRL 的 QQ（2685549821）会在更短的时间内获得回复。

提问之前，你可以先看看本仓库的历史 issue 列表，看看之前的问答能否解决你的问题。 

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
