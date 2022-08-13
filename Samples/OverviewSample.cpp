#include "HiEasyX.h"

#define WND_W 640
#define WND_H 480

#define EDIT_W 480
#define EDIT_H 400

#define BTN_X 20
#define BTN_Y 430

#define CHECKBOX_X 140
#define CHECKBOX_Y 435

hiex::Canvas canvas_main;
hiex::SysEdit edit;
hiex::SysButton btn;
hiex::SysCheckBox checkbox;

void OutInfo(hiex::Canvas& canvas)
{
	static LPCTSTR lpszText = L"Created by HiEasyX " _HIEASYX_VER_STR_;
	canvas.SetTextEscapement(0);
	canvas.SetTextOrientation(0);
	canvas.SetTextStyle(16, 0, L"Arial");
	canvas.OutTextXY(
		canvas.GetWidth() - canvas.TextWidth(lpszText),
		canvas.GetHeight() - canvas.TextHeight(lpszText),
		lpszText,
		true, GRAY
	);
}

// 连续输出垂直文字
// 调用前需要自行设置文字垂直属性
void VerticalText(LPCTSTR lpsz, bool text_c, COLORREF cText, bool  bk_c, COLORREF cBk, bool set = false, int x = 0, int y = 0)
{
	static int sx = 0, sy = 0;
	if (set)
	{
		sx = x;
		sy = y;
	}
	if (text_c)
		settextcolor(cText);
	if (bk_c)
		setbkcolor(cBk);
	outtextxy(sx, sy, lpsz);
	sy += textwidth(lpsz);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static COLORREF bk = CLASSICGRAY;

	switch (msg)
	{
	case WM_PAINT:
		canvas_main.SetTextEscapement(-900);
		canvas_main.SetTextOrientation(-900);
		canvas_main.SetTextStyle(140, 0, L"微软雅黑");

		canvas_main.BeginBatchDrawing();
		VerticalText(L"Hi", true, LIGHTRED, true, SKYBLUE, true, getwidth() + 10, 20);
		VerticalText(L" ", false, 0, true, bk);
		VerticalText(L"E", true, WHITE, true, RED);
		VerticalText(L"a", false, 0, true, ORANGE);
		VerticalText(L"s", false, 0, true, GREEN);
		VerticalText(L"y", false, 0, true, CYAN);
		VerticalText(L"X", false, 0, true, BLUE);
		canvas_main.EndBatchDrawing();

		canvas_main.SetBkColor(bk);
		OutInfo(canvas_main);
		
		break;

	case WM_SIZE:
	{
		canvas_main.Clear(true, bk);

		int dx = canvas_main.GetWidth() - WND_W;
		int dy = canvas_main.GetHeight() - WND_H;

		edit.Resize(
			EDIT_W + dx,
			EDIT_H + dy
		);

		btn.Move(BTN_X, BTN_Y + dy);
		checkbox.Move(CHECKBOX_X, CHECKBOX_Y + dy);

		break;
	}
	default:
		return HIWINDOW_DEFAULT_PROC;
		break;
	}

	return 0;
}
#include <map>
void OnClick()
{
	static bool running = false;

	static std::map<std::wstring, COLORREF> color_map = {
				{L"black", BLACK},
				{L"white", WHITE},
				{L"gray", GRAY},
				{L"red", RED},
				{L"purple", PURPLE},
				{L"orange", ORANGE},
				{L"yellow", YELLOW},
				{L"green", GREEN},
				{L"blue", BLUE},
				{L"cyan", CYAN}
	};

	static std::wstring typeface[] = {
		L"system", L"Arial", L"Consolas", L"微软雅黑", L"宋体", L"仿宋", L"黑体"
	};

	if (!running)
	{
		running = true;

		std::thread([]() {

			hiex::Window wnd_option(300, 300);
			SetWindowText(wnd_option.GetHandle(), L"Option");
			hiex::Canvas canvas;
			wnd_option.BindCanvas(&canvas);
			OutInfo(canvas);
			canvas.SetTypeface(L"system");

			DisableResizing(wnd_option.GetHandle(), true);

			hiex::SysComboBox combobox_ctext;
			hiex::SysComboBox combobox_cbk;
			hiex::SysComboBox combobox_typeface;
			hiex::SysRadioButton radio[2];

			canvas.SetTextColor(GRAY);
			canvas.OutTextXY(20, 20, L"Select text color");

			combobox_ctext.PreSetEtyle(false, false, true);
			combobox_ctext.Create(wnd_option.GetHandle(), 20, 40, 260, 200);

			canvas.OutTextXY(20, 80, L"Select background color");
			combobox_cbk.PreSetEtyle(false, false, true);
			combobox_cbk.Create(wnd_option.GetHandle(), 20, 100, 260, 200);

			canvas.OutTextXY(20, 140, L"Select typeface");
			combobox_typeface.PreSetEtyle(false, true, true);
			combobox_typeface.Create(wnd_option.GetHandle(), 20, 160, 260, 200);

			for (auto& color : color_map)
			{
				combobox_ctext.AddString(color.first);
				combobox_cbk.AddString(color.first);
			}

			for (auto& name : typeface)
			{
				combobox_typeface.AddString(name);
			}

			combobox_ctext.SetSel(0);
			combobox_ctext.RegisterSelMessage([](int nSel, std::wstring wstrSelText) {
				edit.SetTextColor(color_map[wstrSelText]);
				});

			combobox_cbk.SelectString(L"white");	// 通过字符串选择项
			combobox_cbk.RegisterSelMessage([](int nSel, std::wstring wstrSelText) {
				edit.SetTextBkColor(color_map[wstrSelText]);
				edit.SetBkColor(color_map[wstrSelText]);
				});

			combobox_typeface.SelectString(L"微软雅黑");
			combobox_typeface.RegisterSelMessage([](int nSel, std::wstring wstrSelText) {
				edit.SetFont(26, 0, wstrSelText);
				});
			combobox_typeface.RegisterEditMessage([](std::wstring wstrText) {
				edit.SetFont(26, 0, wstrText);
				});

			radio[0].Create(wnd_option.GetHandle(), 20, 200, 100, 30, L"Left align");
			radio[1].Create(wnd_option.GetHandle(), 20, 230, 100, 30, L"Right align");

			radio[0].Check(true);
			radio[0].RegisterMessage([](bool checked) {
				if (checked)
					edit.RightAlign(false);
				});
			radio[1].RegisterMessage([](bool checked) {
				if (checked)
					edit.RightAlign(true);
				});

			hiex::init_end(wnd_option.GetHandle());
			running = false;

			}).detach();
	}

}

void OnCheck(bool checked)
{
	edit.ReadOnly(checked);
}

int main()
{
	hiex::Window wnd(WND_W, WND_H);
	wnd.BindCanvas(&canvas_main);
	wnd.SetProcFunc(WndProc);
	hiex::AutoExit();

	HWND hwnd = wnd.GetHandle();

	edit.PreSetStyle(true, false, true);
	edit.Create(hwnd, 20, 20, EDIT_W, EDIT_H,
		L"Welcome to HiEasyX !\r\n"
		L"\r\n"
		L"Here you will experience the overall upgrade of EasyX.\r\n"
		L"Let's start HiEasyX from the simple sample.\r\n"
		L"\r\n"
		L"What's new in HiEasyX\r\n"
		L"\r\n"
		L">> You could create multi-window application with HiEasyX easily.\r\n"
		L">> You could use HiCanvas, which is a C++ canvas wrapper of EasyX, in the similar way of EasyX.\r\n"
		L">> The best support of Windows system controls integrated in HiEasyX is avalible.\r\n"
		L">> The most easy method to create a tray.\r\n"
		L"etc.\r\n"
	);
	edit.SetFont(26, 0, L"微软雅黑");

	btn.Create(hwnd, BTN_X, BTN_Y, 100, 30, L" Options...");
	btn.RegisterMessage(OnClick);
	hiex::Canvas btn_img(20, 20, 0xe1e1e1);
	btn_img.SolidCircle(10, 10, 10, true, GRAY);
	btn_img.Line(10, 0, 10, 20, true, WHITE);
	btn_img.Line(0, 10, 20, 10, true, WHITE);
	btn.Image(true, &btn_img, true);

	checkbox.Create(hwnd, CHECKBOX_X, CHECKBOX_Y, 100, 20, L"Read only");
	checkbox.RegisterMessage(OnCheck);

	hiex::init_end(hwnd);

	closegraph();
	return 0;
}
