/**
 * @brief 	第 0 阶段：开始绘制
 * @note
 * 			此例中，以最简洁的方式展现了 HiEasyX 的调用方式 <p>
 *			在后面的例子中，将不再解释 BEGIN_TASK() 系列宏 <p>
 *
 * @attention
 *			BEGIN_TASK() 是直接为当前工作窗口启动任务 <p>
 *			当开启多个窗口时，需要使用 BEGIN_TASK_WND() 启动任务，并指定任务窗口 <p>
 *
 * @author 	huidong <mailhuid@163.com>
 * @date 	2023.01.12
*/

#include "HiEasyX.h"		// 包含 HiEasyX 头文件

int main()
{
	initgraph();			// 初始化窗口

	BEGIN_TASK();			// （不同于 EasyX）启动任务，标识开始绘制

	circle(320, 240, 100);	// 画圆

	END_TASK();				// （不同于 EasyX）完成绘制，结束任务

	REDRAW_WINDOW();		// （不同于 EasyX）刷新屏幕，使刚才绘制的内容显示出来

	getmessage(EM_KEY);		// 任意键退出

	closegraph();			// 关闭窗口
	return 0;
}
