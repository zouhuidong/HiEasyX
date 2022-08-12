#include "../HiEasyX/HiEasyX.h"

void TestMain()
{
	HiEasyX::Window window(640, 480);
	HiEasyX::Window windowChild(300, 200, EW_NORMAL, L"Child Window", nullptr, window.GetHandle());
	HiEasyX::Canvas canvas, canvasChild;
	window.BindCanvas(&canvas);
	windowChild.BindCanvas(&canvasChild);
	DisableResizing(true);

	HWND hwnd = initgraph(200, 200);
	BEGIN_TASK_WND(hwnd);
	rectangle(20, 20, 50, 50);
	END_TASK();

	canvasChild.FillCircle(canvasChild.GetWidth() / 2, canvasChild.GetHeight() / 2, 50);
	windowChild.Redraw();

	canvas.SetFont(72);
	canvas.SetBkMode(TRANSPARENT);

	HiEasyX::Gif gif;
	gif.load(L"test.gif");
	HiEasyX::Canvas canvasGif(200, 200);
	gif.setSize(200, 200);
	gif.bind(canvasGif.GetHDC());

	clock_t t = clock();
	size_t count = 0;
	while (true)
	{
		canvas.Clear();
		canvas.CenterText_Format(24, L"HiEasyX %.2f", PI);

		gif.draw();
		gif.toggle();
		canvas.PutImageIn_Alpha(0, 0, &canvasGif, { 0 }, 100);

		// 已输出帧数
		canvas.OutTextXY_Format(0, 0, 6, L"%d", count);

		window.Redraw();

		if (!window.isAlive())
		{
			exit(0);
		}

		//Sleep(50);

		count++;
	}

	_gettch();
	closegraph();
}


#define BTN_WIDTH 70
#define BTN_HEIGHT 23

#define ALIGN_X 60

HiEasyX::Page page;

HiEasyX::Canvas canvas;

HiEasyX::ControlBase btnSave(ALIGN_X, 140);
HiEasyX::Button btnSaveText(0, 0, BTN_WIDTH, BTN_HEIGHT, L"Save");
HiEasyX::Button btnSaveOption(0, 0, 25, BTN_HEIGHT, L"...");

HiEasyX::Static text(150, 45, 90, 30, L"static text ctrl.");

void MsgProc(HiEasyX::ControlBase* _Ctrl, int _MsgId, ExMessage msg)
{
	if (_Ctrl == &btnSaveText)
	{
		switch (_MsgId)
		{
		case HiEasyX::CM_CLICK:
		{
			static int i = 0;
			i++;
			canvas.SetTextColor(BLACK);
			canvas.CenterText_Format(6, L"%d", i);
		}
		break;
		}
	}
	else if (_Ctrl == &btnSaveOption)
	{
		switch (_MsgId)
		{
		case HiEasyX::CM_CLICK:

			text.ClearText();
			text.AddText(L"Text ", true, BLUE);
			text.AddText(L" bkcolor ", true, RED, true, GREEN);

			_Ctrl->GetParent()->MoveRel(0, 10);

			break;
		}
	}
}

int main()
{
	//TestMain();

	HiEasyX::Window window(640, 480);						// 创建窗口

	window.BindCanvas(&canvas);								// 绑定窗口画布
	canvas.Clear(true, WHITE);								// 清屏

	page.BindToCanvas(&canvas);

	// 按钮控件
	HiEasyX::Button btnOK(ALIGN_X, 50, BTN_WIDTH, BTN_HEIGHT, L"OK");
	btnOK.SetMsgProcFunc([](HiEasyX::ControlBase* btn, int id, ExMessage msg) {
		if (id == HiEasyX::CM_CLICK)
		{
			MessageBox(HiEasyX::GetHWnd_win32(), L"You pressed the button.", L"Tip", MB_OK);
		}
		});

	HiEasyX::Button btnClassic(ALIGN_X, 80, BTN_WIDTH, BTN_HEIGHT, L"Button");
	btnClassic.EnableClassicStyle(true);

	HiEasyX::Button btnDisabled(ALIGN_X, 110, BTN_WIDTH, BTN_HEIGHT, L"Button");
	btnDisabled.SetEnable(false);

	btnSave.EnableAutoSizeForChild(true);					// 设置复合控件
	btnSave.AddChild(&btnSaveText);
	btnSave.AddChild(&btnSaveOption, BTN_WIDTH);
	btnSaveText.SetMsgProcFunc(MsgProc);
	btnSaveOption.SetMsgProcFunc(MsgProc);
	btnSaveOption.SetTextColor(RED);

	HiEasyX::Static pic(20, 40, 30, 30);					// 静态图像控件
	HiEasyX::ImageBlock pic_imgblock(0, 0, 30, 30, WHITE);	// 绑定图像块
	pic.EnableAutoRedraw(true);
	pic.SetImage(&pic_imgblock);
	pic.GetImage()->GetCanvas()->FillPie({ 0,0,29,29 }, 2, PI * 2, true, GREEN, WHITE);
	pic.SetMsgProcFunc([](HiEasyX::ControlBase* ctrl, int id, ExMessage msg) {

		HiEasyX::Canvas* pCanvas = ((HiEasyX::Static*)ctrl)->GetImage()->GetCanvas();
		pCanvas->Clear();

		int w = ctrl->GetWidth() - 1;
		int h = ctrl->GetHeight() - 1;

		if (ctrl->isPressed())
			pCanvas->FillPie({ 0,0,w,h }, 0, PI * 2, true, GREEN, WHITE);
		else if (ctrl->isHovered())
			pCanvas->FillPie({ 0,0,w,h }, 1, PI * 2, true, GREEN, WHITE);
		else
			pCanvas->FillPie({ 0,0,w,h }, 2, PI * 2, true, GREEN, WHITE);

		});

	HiEasyX::ProgressCtrl progress(250, 55, 300, 20, 300);	// 进度条控件
	progress.SetProcess(33);
	//progress.SetBarColor(RED);
	progress.EnableAnimation(true);
	progress.SetText(L"Press me");
	progress.GetCanvas().SetBkMode(TRANSPARENT);

	HiEasyX::ScrollBar scrollbar(20, 100, 25, 300, 100, 20, false);
	scrollbar.SetViewLength(33);

	HiEasyX::ScrollBar scrollbar_horizon(ALIGN_X, 10, 300, 25, 100, 20, true);
	scrollbar_horizon.SetViewLength(33);

	page.push({
		&pic,
		&btnOK,
		&btnClassic,
		&btnDisabled,
		&btnSave,
		&text,
		&progress,
		&scrollbar,
		&scrollbar_horizon,
		});

	HiEasyX::SysButton sys_btn(window.GetHandle(), 200, 200, 100, 30, L"Sys Btn");

	ExMessage msg;
	while (window.isAlive())
	{
		while (window.Peek_Message(&msg, EM_MOUSE))
		{
			page.UpdateMessage(msg);
		}

		if (window.isSizeChanged())
		{
			int interval = 25;
			int w = window.GetClientWidth();
			int h = window.GetClientHeight();
			page.SetRect({ 0,0,w,h });
			progress.SetWidth(w - progress.GetX() - interval);
			scrollbar.SetHeight(h - scrollbar.GetY() - interval);
			scrollbar_horizon.SetWidth(w - scrollbar_horizon.GetX() - interval);
		}

		if (progress.isPressed())
		{
			progress.Step();
		}

		page.Draw();
		progress.Draw_Text();
		page.Render();

		//page.UpdateImage();

		window.FlushDrawing();
		window.Redraw();

		// 固定帧率，并在非活动窗口时释放 CPU 占用
		HiEasyX::DelayFPS(24, !window.isForegroundWindow());
	}

	return 0;
}
