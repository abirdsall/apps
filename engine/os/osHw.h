#ifndef OS_HW_H
#define OS_HW_H

#include "osHwKeyboard.h"
#include "osHwMouse.h"
#include "osHwWindow.h"

namespace os
{
    bool MainHw( int argc, char *argv[] );
	bool InitHw();
	void KillHw();
}

#endif