////////////////////////////////////
//
//	HiCanvas.h
//	HiEasyX 库的绘制模块
//

#pragma once

#include "HiDrawingProperty.h"
#include <vector>
#include <list>
#include <string>

namespace HiEasyX
{

	// 根据透明度混合颜色
	// cDst				原像素点
	// cSrc				新像素点，根据该点的透明度进行混合
	// isCalculated		新像素是否已经混合过透明色
	// alpha			叠加透明度（默认为 255，即不启用）
	COLORREF MixAlphaColor(COLORREF cDst, COLORREF cSrc, bool isCalculated, BYTE alpha = 255);

	//
	// CopyImage_Alpha
	// 快速复制图像（可开启透明通道）
	// 
	// alpha
	//		叠加透明度（透明 0 ~ 255 不透明），默认为 255（表示不启用）
	// 
	// bUseSrcAlpha
	//		是否使用原图像透明度进行混合（可同时使用叠加透明度）
	//		开启时必须保证 IMAGE 中含有透明度信息。
	//		EasyX 中的图像一般无透明度（默认设为 0，即全透明），故一般不使用原图透明度。
	//		一般情况下，只有复制 png 图像、或是复制专门生成的含有透明信息的图像时，才开启此项。
	// 
	// isCalculated
	//		在启用原图像透明度的前提下，标记原图像是否已经计算好混合后的颜色
	//		通常 png 图像像素颜色都已进行过混合运算。
	//		开启后，原图像便不再计算混合颜色，只有目标图像参与计算。
	// 
	// 注：
	//		若未启用任何一种透明通道，则等同于覆盖式地复制图像内容，这可以保留原图像的透明信息（如果有）。
	//		无论启用哪种透明通道，目标图像最终都不再包含透明度信息（原图数据不受影响），因为已经混合完毕。
	//
	void CopyImage_Alpha(
		int x, int y,
		DWORD* pDst, int wDst, int hDst,
		DWORD* pSrc, int wSrc, int hSrc,
		BYTE alpha = 255,
		bool bUseSrcAlpha = false,
		bool isCalculated = true
	);

	// 旋转图像（保留透明信息，自适应大小）
	// pImg		原图像
	// radian	旋转弧度
	// bkcolor	背景填充颜色
	// 返回旋转后的图像
	IMAGE RotateImage_Alpha(IMAGE* pImg, double radian, COLORREF bkcolor = BLACK);

	// 缩放图像（保留透明度信息）
	// 原作者：豆焰 <beanflame@qq.com>（有改动）
	IMAGE ZoomImage_Alpha(IMAGE* srcimg, int width, int height);

	// 图像缩放（基于 Win32 API，保留透明度信息）
	IMAGE ZoomImage_Win32_Alpha(IMAGE* pImg, int width, int height);

	// 画布
	// 支持 Alpha 通道的函数名称中都带有 "_Alpha" 后缀
	class Canvas : public IMAGE
	{
	private:

		IMAGE m_imgEmpty;						// 空图像

	protected:

		DrawingProperty m_property;			// 保存外界绘图属性

		DWORD* m_pBuf = nullptr;			// 图像内存指针
		int m_nWidth, m_nHeight;			// 图像宽高
		int m_nBufSize;						// 图像面积

		bool m_bBindToImgPointer;			// 该画布是否绑定到图像指针
		IMAGE* m_pImg;						// 画布绑定的图像指针（若画布绑定到指针）

		bool m_bBatchDraw;					// 是否启用了批量绘制
		bool m_bDoNotEndDrawing;			// 标记不要结束绘制（用于处理绘图函数相互调用的情况，防止提前结束任务）

		HWND m_hBindWindow;					// 绑定到的窗口

		// 清空设置
		void CleanUpSettings();

		// 单独启动 HiWindow 的窗口任务（如果绑定了窗口）
		bool BeginWindowTask();

		// 结束 HiWindow 的窗口任务
		void EndWindowTask();

		// 调用 EasyX 绘图函数前，设置该画布为目标绘图画布
		// 若绑定了窗口，则自动启动窗口任务
		bool BeginDrawing();

		// 调用 EasyX 绘图函数完毕，恢复先前的绘图状态
		void EndDrawing();

	public:

		/// 画布操作函数

		Canvas();

		Canvas(int w, int h, COLORREF cBk = BLACK);

		// 只是复制图像指针中的内容，绑定图像指针请调用 BindToImage
		Canvas(IMAGE* pImg);
		Canvas(IMAGE img);

		// 只是复制图像指针中的内容，绑定图像指针请调用 BindToImage
		Canvas& operator= (IMAGE* pImg);
		Canvas& operator= (IMAGE img);

		// 重新加载图像信息
		void UpdateInfo();

		// 重设画布大小（若绑定了窗口，则不建议调用）
		virtual void Resize(int w, int h);

		// 绑定到图像指针
		// 注意：
		//		绑定到图像指针后，如果在外部调整了图像的大小，则必须调用 UpdateInfo 重新加载图像信息
		//		若要在外部使用图像指针，则必须调用 GetImagePointer
		//		若要绑定到窗口，请使用 BindWindowCanvas
		Canvas& BindToImage(IMAGE* pImg);

		// 将画布绑定到窗口（自动响应画布调整消息）
		// 此函数禁止用户调用，请使用 BindWindowCanvas
		Canvas& BindToWindow(HWND hWnd, IMAGE* pImg);

		// 获取画布 IMAGE 指针
		IMAGE* GetImagePointer() { return m_bBindToImgPointer ? m_pImg : this; }

		// 获取图像缓冲区指针
		DWORD* GetBuffer() { return m_bBindToImgPointer ? GetImageBuffer(m_pImg) : m_pBuf; }

		// 获取图像缓冲区大小（宽 * 高）
		int GetBufferSize() { return m_nBufSize; }

		// 获取画布的 HDC
		HDC GetHDC() { return GetImageHDC(GetImagePointer()); }

		int getwidth() { return m_nWidth; }

		int getheight() { return m_nHeight; }

		int Width() { return m_nWidth; }

		int Height() { return m_nHeight; }

		/// 绘图状态设置函数

		// 开始大批量绘制（该函数并非用于开启双缓冲）
		// 注意：
		//		调用该函数后，当前绘图目标将转移到该画布，此后每次绘制不会恢复绘图目标。
		//		仅针对 EasyX 原生绘图函数对应的大部分类函数有效，且仅在绘图量较大时有明显效果。
		//		若画布和窗口绑定，则开启批量绘制后必须手动结束批量绘制才能结束窗口任务
		void BeginBatchDrawing();

		// 结束批量绘制
		// 绘图目标将恢复到先前状态（即使在批量绘制中手动修改了绘图目标，也将被重置）
		void EndBatchDrawing();

		/// 绘图函数

		// 判断某点是否位于图像中
		// pIndex 返回该点的数组索引
		bool isValidPoint(int x, int y, int* pIndex = nullptr);

		// 将该画布的图像绘制到另一画布中
		void RenderTo_Alpha(
			int x, int y,
			IMAGE* pImg = nullptr,
			BYTE alpha = 255,
			bool bUseSrcAlpha = false,
			bool isCalculated = true
		);

		LINESTYLE GetLineStyle();

		void Clear(bool isSetColor = false, COLORREF bkcolor = BLACK);

		void SetLineStyle(LINESTYLE style);

		void SetLineStyle(int style, int thickness = 1, const DWORD* puserstyle = nullptr, DWORD userstylecount = 0);

		void SetLineThickness(int thickness);

		int GetLineThickness();

		FILLSTYLE GetFillStyle();

		void SetFillStyle(FILLSTYLE style);

		void SetFillStyle(int style, long hatch = 0, IMAGE* ppattern = nullptr);

		void SetFillStyle(BYTE* ppattern8x8);

		int GetRop2();

		void SetRop2(int mode);

		int GetPolyFillMode();

		void SetPolyFillMode(int mode);

		void SetDefault();

		COLORREF GetLineColor();

		void SetLineColor(COLORREF color);

		COLORREF GetTextColor();

		void SetTextColor(COLORREF color);

		COLORREF GetFillColor();

		void SetFillColor(COLORREF color);

		COLORREF GetBkColor();

		void SetBkColor(COLORREF color);

		int GetBkMode();

		void SetBkMode(int mode);

		COLORREF GetPixel(int x, int y);

		void PutPixel(int x, int y, COLORREF c);

		// 直接操作显存快速获取点
		COLORREF GetPixel_Fast(int x, int y);

		// 直接操作显存快速绘制点
		void PutPixel_Fast(int x, int y, COLORREF c);

		// 绘制带有透明度的点，透明度信息在 COLORREF 中
		void PutPixel_Fast_Alpha(int x, int y, COLORREF c);

		void Line(int x1, int y1, int x2, int y2, bool isSetColor = false, COLORREF c = 0);

		void Line(POINT pt1, POINT pt2, bool isSetColor = false, COLORREF c = 0);

		void Rectangle(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);

		void Rectangle(RECT rct, bool isSetColor = false, COLORREF c = 0);

		void FillRectangle(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void FillRectangle(RECT rct, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void SolidRectangle(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);

		void SolidRectangle(RECT rct, bool isSetColor = false, COLORREF c = 0);

		void ClearRectangle(int left, int top, int right, int bottom);

		void ClearRectangle(RECT rct);

		void Circle(int x, int y, int radius, bool isSetColor = false, COLORREF c = 0);

		void FillCircle(int x, int y, int radius, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void SolidCircle(int x, int y, int radius, bool isSetColor = false, COLORREF c = 0);

		void ClearCircle(int x, int y, int radius);

		void Ellipse(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);

		void Ellipse(RECT rct, bool isSetColor = false, COLORREF c = 0);

		void FillEllipse(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void FillEllipse(RECT rct, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void SolidEllipse(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);

		void SolidEllipse(RECT rct, bool isSetColor = false, COLORREF c = 0);

		void ClearEllipse(int left, int top, int right, int bottom);

		void ClearEllipse(RECT rct);

		void RoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);

		void RoundRect(RECT rct, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);

		void FillRoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void FillRoundRect(RECT rct, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void SolidRoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);

		void SolidRoundRect(RECT rct, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);

		void ClearRoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight);

		void ClearRoundRect(RECT rct, int ellipsewidth, int ellipseheight);

		void Arc(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);

		void Arc(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);

		void Pie(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);

		void Pie(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);

		void FillPie(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void FillPie(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void SolidPie(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);

		void SolidPie(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);

		void ClearPie(int left, int top, int right, int bottom, double stangle, double endangle);

		void ClearPie(RECT rct, double stangle, double endangle);

		void Polyline(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);

		void Polygon(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);

		void FillPolygon(const POINT* points, int num, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);

		void SolidPolygon(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);

		void ClearPolygon(const POINT* points, int num);

		void PolyBezier(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);

		// 填充某区域
		// filltype 填充模式
		//		FLOODFILLBORDER		指定 color 为填充区域的边框颜色
		//		FLOODFILLSURFACE	指定 color 为填充区域的连续颜色
		void FloodFill(int x, int y, COLORREF color, int filltype = FLOODFILLBORDER, bool isSetColor = false, COLORREF cFill = 0);

		// 在指定位置输出文本，返回文本像素宽度
		int OutTextXY(int x, int y, LPCTSTR lpszText, bool isSetColor = false, COLORREF c = 0);

		// 在指定位置输出字符，返回字符像素宽度
		int OutTextXY(int x, int y, TCHAR ch, bool isSetColor = false, COLORREF c = 0);

		// 在指定位置输出格式化文本，返回文本像素宽度
		int OutTextXY_Format(int x, int y, int _Size, LPCTSTR _Format, ...);

		// 获取文本像素宽度
		int TextWidth(LPCTSTR lpszText);

		// 获取字符像素宽度
		int TextWidth(TCHAR c);

		// 获取文本像素宽度
		int TextHeight(LPCTSTR lpszText);

		// 获取字符像素宽度
		int TextHeight(TCHAR c);

		int Draw_Text(LPCTSTR str, RECT* pRect, UINT uFormat, bool isSetColor = false, COLORREF c = 0);

		int Draw_Text(TCHAR ch, RECT* pRect, UINT uFormat, bool isSetColor = false, COLORREF c = 0);

		// 在某区域居中输出文字
		// rct 输出区域，默认为画布区域
		void CenterText(LPCTSTR lpszText, RECT rct = { -1 }, bool isSetColor = false, COLORREF c = 0);

		// 在画布居中输出格式化文本
		void CenterText_Format(int _Size, LPCTSTR _Format, ...);

		LOGFONT GetTextStyle();

		void SetTextStyle(int nHeight, int nWidth, LPCTSTR lpszFace);

		void SetTextStyle(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut);

		void SetTextStyle(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);

		void SetTextStyle(LOGFONT font);

		// 设置字体大小
		void SetFont(int nHeight, int nWidth = 0);

		// 设置使用字体的名称
		void SetTypeface(LPCTSTR lpsz);

		// 设置字符串的书写角度（单位 0.1 度）
		void SetTextEscapement(LONG lfEscapement);

		// 设置每个字符的书写角度（单位 0.1 度）
		void SetTextOrientation(LONG lfOrientation);

		// 设置字符的笔画粗细（范围 默认 0 ~ 1000 最粗）
		void SetTextWeight(LONG lfWeight);

		// 设置字体是否为斜体
		void SetTextItalic(bool lfItalic);

		// 设置字体是否有下划线
		void SetTextUnderline(bool lfUnderline);

		// 设置字体是否有删除线
		void SetTextStrikeOut(bool lfStrikeOut);

		// 获取前景色
		COLORREF GetColor();

		// 设置前景色
		void SetColor(COLORREF color);

		int GetX();

		int GetY();

		void MoveTo(int x, int y);

		void MoveRel(int dx, int dy);

		void LineTo(int x, int y, bool isSetColor = false, COLORREF c = 0);

		void LineRel(int dx, int dy, bool isSetColor = false, COLORREF c = 0);

		void OutText(LPCTSTR lpszText, bool isSetColor = false, COLORREF c = 0);

		void OutText(TCHAR ch, bool isSetColor = false, COLORREF c = 0);

		// 输出格式化文本，返回文本像素宽度
		int OutText_Format(int _Size, LPCTSTR _Format, ...);

		// 加载图片文件到画布
		// 
		// x, y					图像输出位置
		// bResize				标记是否调整画布大小以正好容纳图像
		// nWidth, nHeight		图像目标拉伸尺寸，为 0 表示不拉伸
		// alpha				输出图像的透明度
		// bUseSrcAlpha			是否使用原图的透明度信息进行混合（仅支持有透明度信息的 png 图像）
		// 
		// 注：开启任意一个透明通道，都会使得该画布最终不包含透明度信息。
		// 
		// 返回读取到的 IMAGE 对象
		IMAGE Load_Image_Alpha(
			LPCTSTR lpszImgFile,
			int x = 0, int y = 0,
			bool bResize = false,
			int nWidth = 0, int nHeight = 0,
			BYTE alpha = 255,
			bool bUseSrcAlpha = false
		);

		// 绘制图像到该画布
		void PutImage_Alpha(
			int x, int y,
			IMAGE* pImg,
			BYTE alpha = 255,
			bool bUseSrcAlpha = false,
			bool isCalculated = true
		);

		// EasyX 原生旋转函数
		void RotateImage(double radian, COLORREF bkcolor = BLACK, bool autosize = false, bool highquality = true);

		// 旋转图像（保留 Alpha 信息）
		void RotateImage_Alpha(double radian, COLORREF bkcolor = BLACK);

		// 缩放图像
		void ZoomImage_Alpha(int nW, int nH);

		// 缩放图像（基于 Win32 API）
		void ZoomImage_Win32_Alpha(int nW, int nH);

	};

	// 图像块（用于展示在图层）
	class ImageBlock
	{
	private:
		Canvas* m_pCanvas = nullptr;
		bool m_isCreated = false;			// 画布是否为自己创建的

		void DeleteMyCanvas();				// 删除自己创建的画布

	public:
		int x = 0, y = 0;					// 图像显示在图层的位置
		bool bUseSrcAlpha = false;			// 是否使用图像自身的 alpha 数据

		bool isAlphaCalculated = false;		// 图像自身的像素颜色是否已经完成了 alpha 混合
											// 注：启用此项，须先启用 bUseSrcAlpha

		BYTE alpha = 255;					// 绘制到图层时的叠加透明度
		bool bVisible = true;				// 图像是否可见

		ImageBlock();

		ImageBlock(Canvas* pCanvas);

		ImageBlock(int _x, int _y, Canvas* pCanvas);

		// 新建画布
		ImageBlock(int _x, int _y, int w, int h, COLORREF cBk = 0);

		virtual ~ImageBlock();

		// 不绑定外部画布，直接新建画布
		Canvas* CreateCanvas(int w, int h, COLORREF cBk = 0);

		Canvas* GetCanvas();

		void SetCanvas(Canvas* pCanvas);

		POINT GetPos();

		void SetPos(int _x, int _y);
	};

	// 图层
	class Layer : public std::list<ImageBlock*>
	{
	private:
		DrawingProperty m_property[2];		// 保存上次的绘图属性

	public:
		bool bVisible = true;				// 图层是否可见
		BYTE alpha = 255;					// 图层中所有图像块的叠加透明度
		bool bOutline = false;				// 是否显示轮廓
		bool bText = false;					// 若显示轮廓，是否显示文字

		// 渲染到...
		// bShowOutline 是否显示轮廓
		// strAddedText 轮廓中的附加文本内容
		void RenderTo_Alpha(IMAGE* pImg = nullptr, bool bShowOutline = false, bool bShowText = true, std::wstring wstrAddedText = L"");
	};

	// 特殊图层顺序标识
	enum LayerOrder
	{
		LAYERORDER_BOTTOM_MOST,
		LAYERORDER_BOTTOM,
		LAYERORDER_NORMAL,
		LAYERORDER_TOP,
		LAYERORDER_TOP_MOST
	};

	// 场景
	// 图层索引越大，图层越靠前
	class Scene : public std::vector<Layer*>
	{
	private:

		DrawingProperty m_property;				// 保存之前的绘图属性

		// 特殊图层
		Layer m_layerBottomMost;				// 最底层
		Layer m_layerBottom;					// 底层
		Layer m_layerTop;						// 顶层
		Layer m_layerTopMost;					// 最顶层

	public:

		// 获取所有图层的拷贝
		// 图层索引越大，图层越靠前
		std::vector<Layer*> GetAllLayer();

		// 获取所有图层的总数
		size_t GetAllLayerSize();

		// 获取特殊图层（除了普通图层外的其他图层，见 LayerOrder）
		// 建议多使用普通图层
		Layer* GetSpecialLayer(int order);

		// 渲染到...
		void RenderTo_Alpha(IMAGE* pImg = nullptr, bool bShowAllOutline = false, bool bShowAllText = true);
	};

}

