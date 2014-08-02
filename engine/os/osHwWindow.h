#ifndef OS_HW_WINDOW
#define OS_HW_WINDOW

#include "osWindow.h"

namespace os
{
	void WindowInitHw();
	void WindowTickHw();
	void WindowKillHw();
	bool WindowOpenHw( WindowFormat& windowFormat );
}

#endif

