> [回到教程目录](./index.md)

# 场景的使用

## 概念

**图像块**（ `hiex::ImageBlock` ）是 `hiex::Canvas` 的扩展，它保存了画布的位置，透明通道信息，可以更方便地存储在图层中。

**图层**（ `hiex::Layer` ）中存储有若干个图像块，图层的透明度可以叠加到所有图像块上。

**场景**（ `hiex::Scene` ）中存储有若干个图层，以及一些特殊图层。渲染整个场景时，可以使图层按次序渲染。

## 应用

可以看下面的一个例子：

<div align=center>
<img src="../screenshot/balls1.png"><br>
<b>透明通道 - 小球示例（1）</b>
</div><br>

<div align=center>
<img src="../screenshot/balls2.png"><br>
<b>透明通道 - 小球示例（2）</b>
</div><br>

示例中，透明小球在窗口中运动，在碰到边界时反弹。

> 在此查看 [源代码](../Samples/Recommend/Balls.cpp)

> [回到教程目录](./index.md)

