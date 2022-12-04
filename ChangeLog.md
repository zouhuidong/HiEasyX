# 更新日志

**Ver 0.2.2** (2022.12.04)

*新增*
1. 系统控件（SysControlBase）新增 Remove 方法，可以移除控件
2. 新增 PreSetWindowStyleEx 函数，可以在创建窗口前预定窗口 Ex 样式
3. 新增 PreSetWindowShowState 函数，可以在创建窗口前预定窗口显示状态

*修复*
1. 修复 MixAlphaColor 函数透明叠加溢出问题（参见 https://github.com/zouhuidong/HiEasyX/issues/5 ）
2. 修复多窗口获取消息发生错误和崩溃的 bug
3. 修复多个窗口同时创建时预设样式交叉的 bug
4. 修复使用 Window 类创建窗口时，预先设置窗口样式无效的 bug
5. 修复 ZoomImage_Alpha 算法效率低下问题
6. 修复 DeleteTray 函数逻辑
7. 增加 InitWindow 稳定性

*修改*
1. 默认取消 release 模式程序开场动画，可在 hidef.h 设置

---

**Ver 0.2.1** (2022.08.29)

1. 修复 `hiex::GetMouseMsg_win32` 无法正确获取鼠标消息的 bug。
2. `hiex::To_MouseMsg` 函数加入消息类型判断，如果传入 ExMessage 不是 EM_MOUSE 消息类型，则返回空的 MOUSEMSG。

---

**Ver 0.2 (alpha)** (2022.08.14)

从 [EasyWin32](http://github.com/zouhuidong/EasyWin32) 全面升级而成的第一个版本

---
