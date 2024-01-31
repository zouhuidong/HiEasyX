/**
 * @brief	透明小球边界碰撞示例
 * @note	此示例用于演示 HiCanvas 的透明通道、场景和图层
 * @author	huidong<mailhuid@163.com> alan-crl<alan-crl@foxmail.com>
 * @version	HiEasyX Ver0.4.1
 * @date	2024.01.31
*/

#include "HiEasyX.h"

// 小球数量
#define BALL_NUM 14

// 小球半径
#define RADIUS 70

// 球体
struct Ball
{
	hiex::ImageBlock img;
	float x, y;
	float vx, vy;
};

hiex::SysButton btn;			// 按钮
bool show_outline = false;		// 是否显示轮廓

int main()
{
	hiex::Window wnd(640, 480);			// 创建窗口
	hiex::Canvas canvas;				// 创建画布对象
	wnd.BindCanvas(&canvas);			// 将窗口和画布绑定

	// 手动刷新双缓冲
	//hiex::EnableAutoFlush(false);

	canvas.Clear(true, BLACK);			// 设置背景色为黑色，清空画布

	// 创建按钮，用于点击设置是否显示轮廓
	btn.Create(wnd.GetHandle(), 50, 50, 120, 30, L"Hide outline");
	btn.RegisterMessage([]() {
		show_outline = !show_outline;
		btn.SetText(show_outline ? L"Show outline" : L"Hide outline");
		});

	// 初始化随机数
	srand((UINT)time(nullptr));

	hiex::Scene scene;		// 场景
	hiex::Layer layer;		// 图层

	Ball balls[BALL_NUM];	// 小球

	// 初始化小球
	for (auto& i : balls)
	{
		// 位置和速度的初始化
		i.x = (float)(rand() % canvas.GetWidth());
		i.y = (float)(rand() % canvas.GetHeight());
		i.vx = rand() % 5 * (rand() % 2 ? 2.0f : -2.0f);
		i.vy = rand() % 5 * (rand() % 2 ? 2.0f : -2.0f);
		if (i.vx == 0)
			i.vx = 3;
		if (i.vy == 0)
			i.vy = 3;

		// 小球的图像块设置
		i.img.CreateCanvas(RADIUS * 2, RADIUS * 2);
		i.img.GetCanvas()->Clear();
		i.img.GetCanvas()->SolidCircle(RADIUS, RADIUS, RADIUS, true, rand() % 0xffffff);
		ReverseAlpha(i.img.GetCanvas()->GetBuffer(), i.img.GetCanvas()->GetBufferSize());

		// 设置图像块透明度
		i.img.alpha = 200;
		i.img.bUseSrcAlpha = true;

		// 加入图像块到图层
		layer.push_back(&i.img);
	}

	// 加入图层到场景
	scene.push_back(&layer);

	// 主循环（窗口关闭或按下按键时退出）
	while (wnd.IsAlive() && !peekmessage(nullptr, EM_CHAR))
	{
		for (auto& i : balls)
		{
			i.x += i.vx;
			i.y += i.vy;

			// 碰撞判定
			if (i.x - RADIUS < 0)
			{
				i.x = RADIUS;
				i.vx = -i.vx;
			}
			if (i.x + RADIUS > canvas.GetWidth())
			{
				i.x = (float)canvas.GetWidth() - RADIUS;
				i.vx = -i.vx;
			}
			if (i.y - RADIUS < 0)
			{
				i.y = RADIUS;
				i.vy = -i.vy;
			}
			if (i.y + RADIUS > canvas.GetHeight())
			{
				i.y = (float)canvas.GetHeight() - RADIUS;
				i.vy = -i.vy;
			}

			// 更新位置
			i.img.SetPos((int)i.x - RADIUS, (int)i.y - RADIUS);
		}

		if (wnd.BeginTask())
		{
			// 渲染场景
			scene.Render(canvas.GetImagePointer(), show_outline);

			// 手动刷新双缓冲
			//wnd.FlushDrawing();

			wnd.EndTask();
			wnd.Redraw();
		}

		// 平衡帧率
		static hiex::tDelayFPS recond;
		hiex::DelayFPS(recond, 24);
	}

	return 0;
}
