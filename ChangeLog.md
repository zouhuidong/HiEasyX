# 更新日志

## Ver 0.4.0 pre 1 (暂未 pr)

**新增** Release模式全新启动动画  
**修复** `Bitmap2Image` 函数当定义不使用透明度时，透明度不为 255 的问题  

## Ver 0.3.5 (2023.06.23)

**新增**
- 新增 CreateImageColor 函数：可以创建并返回一张指定尺寸和填充颜色（可以包含透明度）的图像
- 新增 SetImageColor 函数：可以将一张 IMAGE 设置成指定背景颜色（可以包含透明度）的图像
- 新增 hiex::TransparentImage 函数：可绘制带有透明通道的图像（可叠加透明度绘制，可只绘制图像指定区域，可拉伸图像绘制）
- 新增 hiex::RECTToRectF 函数：可以将 RECT 转换为 RectF
- 新增 hiex::RectFToRECT 函数：可以将 RectF 转换为 RECT
- 新增 SetWindowTransparent 和 Window::SetTransparent 函数：可以设置窗口透明度，或启用/禁用窗口透明度
- 新增 hiex::init_console 函数：可创建/显示控制台窗口（如果已经创建则显示）
- 新增 hiex::hide_console 函数：可隐藏控制台窗口
- 新增 hiex::close_console 函数：可关闭控制台窗口（不关闭程序）
注意如果定义了 `#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )` 则控制台函数将不起作用

**改动**
- 统一设置文件编码为 unicode

## Ver 0.3.4 (2023.06.17)

**新增**
- 新增 `Bitmap2Image` 函数，可以将 `HBITMAP` 转换为 `IMAGE`。

**改动**
- 修复 `getmessage_win32` 和 `Window` 类中的消息函数无法正确获取指定窗口句柄消息的问题。
- 修复 `EasyX_Gdiplus_RoundRect` 等 GdiPlus 封装绘图函数绘制圆角矩形时出现圆角异常的 bug。

## Ver 0.3.3 (2023.04.29)

**修复**

修复 `EasyX_Gdiplus_RoundRect` 等 GdiPlus 封装绘图函数绘制圆角矩形时内部出现填充空隙的 bug。

## Ver 0.3.2 (2023.03.11)

**新增**

Gdiplus 系列封装函数新增圆角矩形绘制函数（Alan-CRL 提供）。

**改动**

之前 Gdiplus 系列封装函数都是直接使用布尔型参数设置是否抗锯齿，但是由于 Gdiplus 其实有很多种抗锯齿模式，所以现在改成传入 `Gdiplus::SmoothingMode` 枚举类型参数，以支持所有的抗锯齿模式。

## Ver 0.3.1 (2023.01.15)

**修复**

1. 修正 Ver 0.3.0 中 GDI+ 封装函数中，画弧线函数和画饼状图函数的 `sweepangle` 和 `endangle` 混淆的问题
2. 补充 `Canvas::Load_Image_Alpha` 函数的 `isCalculated` 参数
3. 在无宽高的画布中调用 `Canvas::Load_Image_Alpha` 函数，可以自动调整画布到图像大小

**已知 bug**

在 EasyX 20220901 中，如果在调用 `initgraph`，也就是创建窗口前创建 Canvas，可能会导致无法绘制内容：包括无法在画布上绘制内容，以及创建窗口后也无法绘制内容。

此问题很可能是由 EasyX 20220901 的一个 bug 导致的，详见：https://qa.codebus.cn/question/2480


## Ver 0.3.0 (2023.01.13)

**新增**
1. 新增 `HiGdiplus` 模块（对 GDI+ 的封装，可以绘制透明和抗锯齿图形，包括 `HiGdiplus.h` 和 `HiGdiplus.cpp`）
2. 新增 `DRAW_TNS_INIT_GRAPHICS` 宏和 `DRAW_TNS_RENDER_TO` 宏，可以很方便地绘制透明图形
3. 新增 `EnableAutoFlush` 函数（设置是否启用自动刷新双缓冲）
4. 新增 `Canvas::GP_` 系列函数（对应 GDI+ 绘图函数，支持透明和抗锯齿）
5. 新增 `Canvas::ZoomImage_Rough_Alpha` 函数（提供更多一种缩放函数）
6. 新增 `Canvas::Clear_Alpha` 函数（用于保留背景色透明度地清空画布）
7. 新增 `Canvas::EnableAutoMarkFlushWindow` 函数（在画布绑定到窗口的情况下，可以设置 Canvas 每次绘制后是否自动标记需要刷新双缓冲）
8. 新增 `FlushDrawing` 函数（支持部分刷新双缓冲）
9. 新增 `SetWindowTitle` 函数（设置窗口标题）

**改动**
1. 更新双缓冲刷新机制（原本是每次窗口任务结束后直接刷新双缓冲，现在只在任务结束时标记需要刷新，实际等到响应窗口重绘消息时才真正刷新双缓冲，但是可以通过 `EnableAutoFlush` 函数设置是否启用自动刷新双缓冲）
2. `EnforceRedraw` 函数改名为 `RedrawWindow`
3. `FLUSH_DRAW` 宏改名为 `REDRAW_WINDOW`（因为旧名称有歧义，会被误以为是刷新双缓冲，但其实是刷新窗口）
4. 响应 `WM_PAINT` 消息时只对需要部分进行重绘
5. `SysEdit::PreSetStyle` 函数和 `SysComboBox::PreSetStyle` 函数的形参改为了一个囊括了所有旧形参的结构体
6. `isXXX` 函数改名为 `IsXXX`
7. `Canvas::Render` 函数改名为 `Canvas::RenderTo`
8. `Canvas::GetPixel_Fast`、 `Canvas::PutPixel_Fast`、 `Canvas::PutPixel_Fast_Alpha` 函数的 `_Fast` 后缀都改为 `_Direct`
9. 绘制模式 `DM_Normal` 和 `DM_Real` 改成一样了

**删除**
1. 删除了 `FlushDrawingToWnd` 函数


## Ver 0.2.2 (2022.12.04)

**新增**
1. 系统控件（SysControlBase）新增 Remove 方法，可以移除控件
2. 新增 PreSetWindowStyleEx 函数，可以在创建窗口前预定窗口 Ex 样式
3. 新增 PreSetWindowShowState 函数，可以在创建窗口前预定窗口显示状态

**修复**
1. 修复 MixAlphaColor 函数透明叠加溢出问题（参见 https://github.com/zouhuidong/HiEasyX/issues/5 ）
2. 修复多窗口获取消息发生错误和崩溃的 bug
3. 修复多个窗口同时创建时预设样式交叉的 bug
4. 修复使用 Window 类创建窗口时，预先设置窗口样式无效的 bug
5. 修复 ZoomImage_Alpha 算法效率低下问题
6. 修复 DeleteTray 函数逻辑
7. 增加 InitWindow 稳定性

**修改**
1. 默认取消 release 模式程序开场动画，可在 hidef.h 设置

## Ver 0.2.1 (2022.08.29)

1. 修复 `hiex::GetMouseMsg_win32` 无法正确获取鼠标消息的 bug。
2. `hiex::To_MouseMsg` 函数加入消息类型判断，如果传入 ExMessage 不是 EM_MOUSE 消息类型，则返回空的 MOUSEMSG。

## Ver 0.2 (alpha) (2022.08.14)

从 [EasyWin32](http://github.com/zouhuidong/EasyWin32) 全面升级而成的第一个版本
