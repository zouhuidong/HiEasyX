/**
 * @brief 	演示 HiEasyX 的自绘 GUI
 * @author 	huidong <mailhuid@163.com>
 * @date 	2023.01.12
*/

#include "HiEasyX.h"

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

	if (ctrl->IsPressed())
		pCanvas->FillPie({ 0,0,w,h }, 0, PI * 2, true, GREEN, WHITE);
	else if (ctrl->IsHovered())
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
	while (window.IsAlive())
	{
		while (window.Peek_Message(&msg, EM_MOUSE))
		{
			page.UpdateMessage(msg);
		}

		if (window.IsSizeChanged())
		{
			int interval = 25;
			int w = window.GetClientWidth();
			int h = window.GetClientHeight();
			page.SetRect({ 0,0,w,h });
			progress.SetWidth(w - progress.GetX() - interval);
			scrollbar.SetHeight(h - scrollbar.GetY() - interval);
			scrollbar_horizon.SetWidth(w - scrollbar_horizon.GetX() - interval);
		}

		if (progress.IsPressed())
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
		HiEasyX::DelayFPS(24, !window.IsForegroundWindow());
	}

	return 0;
}
