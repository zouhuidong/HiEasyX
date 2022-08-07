#include "HiFPS.h"

#include "HiFunc.h"

#include <time.h>

namespace HiEasyX
{
	clock_t tRecord = 0;
	
	void DelayFPS(int fps, bool wait_long)
	{
		if (wait_long)
		{
			Sleep(500);
			return;
		}

		clock_t tNow = clock();
		if (tRecord)
		{
			int delay = 1000 / fps - (tNow - tRecord);
			if (delay > 0)
			{
				HpSleep(delay);
			}
		}
		tRecord = clock();
	}
};

