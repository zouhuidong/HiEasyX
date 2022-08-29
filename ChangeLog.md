# 更新日志

**Ver 0.2.1** (2022.08.29)

1. 修复 `hiex::GetMouseMsg_win32` 无法正确获取鼠标消息的 bug。
2. `hiex::To_MouseMsg` 函数加入消息类型判断，如果传入 ExMessage 不是 EM_MOUSE 消息类型，则返回空的 MOUSEMSG。

---

**Ver 0.2 (alpha)** (2022.08.14)

从 [EasyWin32](http://github.com/zouhuidong/EasyWin32) 全面升级而成的第一个版本

---
