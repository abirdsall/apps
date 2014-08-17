#ifndef OS
#define OS

#include "core.h"

#include "osHw.h"

#include "osKeyboard.h"
#include "osMouse.h"
#include "osTouch.h"
#include "osWindow.h"

namespace os
{
    enum eState
	{
		eStateUninitialised,
        eStateInitialising,
		eStateInitialised,
	};

    bool Main( int argc, char *argv[],
              void ( *appInit )(),
              void ( *appTick )( f32 dt ),
              void ( *appTouch )( const os::Touch* touches, s32 touchCount ),
              void ( *appDraw )(),
              void ( *appKill )(), WindowFormat windowFormat );
	
    bool FlowActive();
    void FlowInit();
    void FlowInitWindow();
    void FlowTick();
    void FlowTouch( const Touch* touches, s32 touchCount );
    void FlowDraw();
    void FlowKill();
    
    void Kill();
}

#endif