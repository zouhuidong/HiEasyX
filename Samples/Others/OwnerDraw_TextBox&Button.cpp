////////////////////////////////////////
//	
//	程　　序：基于 EasyX 的文本框 + 按钮控件
//	作　　者：BestAns
//	编译环境：VS2019，EasyX_20211109
//	编写日期：2021-10-28
//	最后修改：2021-11-20
// 
//	修　　改: huidong <mailhuid@163.com>
//	修改内容: 将 HiEasyX 应用到程序
//	修改日期：2022-08-13
// 
//	此示例将演示 HiEasyX 对鼠标消息和键盘消息的支持
//

#include "HiEasyX.h"


// 实现文本框控件
class EasyTextBox
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	wchar_t* text = NULL;							// 控件内容
	size_t maxlen = 0;									// 文本框最大内容长度

public:
	void Create(int x1, int y1, int x2, int y2, int max)
	{
		maxlen = max;
		text = new wchar_t[maxlen];
		text[0] = 0;
		left = x1, top = y1, right = x2, bottom = y2;

		// 绘制用户界面
		Show();
	}

	~EasyTextBox()
	{
		if (text != NULL)
			delete[] text;
	}

	wchar_t* Text()
	{
		return text;
	}

	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}

	// 绘制界面
	void Show()
	{
		BEGIN_TASK();

		// 备份环境值
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();

		setlinecolor(LIGHTGRAY);		// 设置画线颜色
		setbkcolor(0xeeeeee);			// 设置背景颜色
		setfillcolor(0xeeeeee);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + 10, top + 5, text);

		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);

		END_TASK();
		FLUSH_DRAW();
	}

	void OnMessage()
	{
		if (!hiex::BeginTask())
		{
			return;
		}

		// 备份环境值
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();

		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + 10, top + 5, text);

		int width = textwidth(text);	// 字符串总宽度
		int counter = 0;				// 光标闪烁计数器
		bool binput = true;				// 是否输入中

		hiex::EndTask();
		hiex::EnforceRedraw();

		ExMessage msg;
		while (binput)
		{
			BEGIN_TASK();
			while (binput && peekmessage(&msg, EM_MOUSE | EM_CHAR, false))	// 获取消息，但不从消息队列拿出
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					// 如果鼠标点击文本框外面，结束文本输入
					if (msg.x < left || msg.x > right || msg.y < top || msg.y > bottom)
					{
						binput = false;
						break;
					}
				}
				else if (msg.message == WM_CHAR)
				{
					size_t len = wcslen(text);
					switch (msg.ch)
					{
					case '\b':				// 用户按退格键，删掉一个字符
						if (len > 0)
						{
							text[len - 1] = 0;
							width = textwidth(text);
							counter = 0;
							clearrectangle(left + 10 + width, top + 1, right - 1, bottom - 1);
						}
						break;
					case '\r':				// 用户按回车键，结束文本输入
					case '\n':
						binput = false;
						break;
					default:				// 用户按其它键，接受文本输入
						if (len < maxlen - 1)
						{
							text[len++] = msg.ch;
							text[len] = 0;
							clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);	// 清除画的光标
							width = textwidth(text);				// 重新计算文本框宽度
							counter = 0;
							outtextxy(left + 10, top + 5, text);		// 输出新的字符串
						}
					}
				}
				peekmessage(NULL, EM_MOUSE | EM_CHAR);				// 从消息队列抛弃刚刚处理过的一个消息
				END_TASK();
				FLUSH_DRAW();

				Sleep(10);
			}

			BEGIN_TASK();

			// 绘制光标（光标闪烁周期为 20ms * 32）
			counter = (counter + 1) % 32;
			if (counter < 16)
				line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);				// 画光标
			else
				clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);		// 擦光标

			END_TASK();
			FLUSH_DRAW();

			// 延时 20ms
			Sleep(20);
		}

		BEGIN_TASK();
		clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);	// 擦光标

		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);

		END_TASK();
		FLUSH_DRAW();

		Show();
	}
};



// 实现按钮控件
class EasyButton
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	wchar_t* text = NULL;							// 控件内容
	void (*userfunc)() = NULL;						// 控件消息

public:
	void Create(int x1, int y1, int x2, int y2, const wchar_t* title, void (*func)())
	{
		text = new wchar_t[wcslen(title) + 1];
		wcscpy_s(text, wcslen(title) + 1, title);
		left = x1, top = y1, right = x2, bottom = y2;
		userfunc = func;

		// 绘制用户界面
		Show();
	}

	~EasyButton()
	{
		if (text != NULL)
			delete[] text;
	}

	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}

	// 绘制界面
	void Show()
	{
		BEGIN_TASK();

		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();

		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (bottom - top - textheight(text) + 1) / 2, text);

		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);

		END_TASK();
		FLUSH_DRAW();
	}

	void OnMessage()
	{
		if (userfunc != NULL)
			userfunc();
	}
};



// 定义控件
EasyTextBox txtName;
EasyTextBox txtPwd;
EasyButton btnOK;



// 按钮 btnOK 的点击事件
void On_btnOk_Click()
{
	if (wcscmp(L"123", txtPwd.Text()))
		MessageBox(GetHWnd(), L"密码错误", L"错误", MB_OK);
	else
	{
		wchar_t s[100] = L"Hello, ";
		wcscat_s(s, 100, txtName.Text());
		MessageBox(GetHWnd(), s, L"Hello", MB_OK);
	}
}



// 主函数
int main()
{
	// 创建图形窗口
	initgraph(640, 480);

	BEGIN_TASK();
	
	// 简单绘制界面
	setbkcolor(0xeeeeee);
	cleardevice();
	settextcolor(BLACK);
	outtextxy(50, 55, L"用户名：");
	txtName.Create(120, 50, 400, 75, 10);						// 创建用户名文本框控件
	outtextxy(50, 105, L"密　码：");
	txtPwd.Create(120, 100, 400, 125, 10);						// 创建密码文本框控件
	btnOK.Create(320, 150, 400, 175, L"OK", On_btnOk_Click);	// 创建按钮控件

	END_TASK();
	FLUSH_DRAW();

	ExMessage msg;
	while (true)
	{
		msg = getmessage(EM_MOUSE);			// 获取消息输入

		if (msg.message == WM_LBUTTONDOWN)
		{
			// 判断控件
			if (txtName.Check(msg.x, msg.y))	txtName.OnMessage();

			// 判断控件
			if (txtPwd.Check(msg.x, msg.y))		txtPwd.OnMessage();

			// 判断控件
			if (btnOK.Check(msg.x, msg.y))		btnOK.OnMessage();
		}
	}

	// 关闭绘图窗口
	closegraph();
	return 0;
}
