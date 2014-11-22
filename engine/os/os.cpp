#include "os.h"
#include "gs.h"

#include <stdlib.h>


namespace os
{
    static void ( *_appInit )();
    static void ( *_appTick )( f32 dt );
    static void ( *_appTouch )( const Touch* touches, s32 touchCount );
    static void ( *_appDraw )();
    static void ( *_appKill )();
    static WindowFormat _windowFormat;
    static State _state = StateUninitialised;
	static bool _killed = false;
	
	void Kill( void )
	{
        _killed = true;
	}
	
    bool Main( int argc, char *argv[],
              void ( *appInit )(),
              void ( *appTick )( f32 dt ),
              void ( *appTouch )( const os::Touch* touches, s32 touchCount ),
              void ( *appDraw )(),
              void ( *appKill )(), WindowFormat windowFormat )
    {
        _appInit = appInit;
        _appTick = appTick;
        _appTouch = appTouch;
        _appDraw = appDraw;
        _appKill = appKill;
        _windowFormat = windowFormat;
        
        PANIC( 2 == sizeof( s16 ) );
		PANIC( 2 == sizeof( u16 ) );
		PANIC( 4 == sizeof( s32 ) );
		PANIC( 4 == sizeof( u32 ) );
		PANIC( 8 == sizeof( s64 ) );
		PANIC( 8 == sizeof( u64 ) );
		      
        return MainHw( argc, argv );
    }
    
    bool FlowActive()
    {
        return _state == StateInitialised && !_killed;
    }
    
    void FlowInit()
    {
        bool initHwSuccess = InitHw();
		
        PANIC( initHwSuccess );

        TimeInit();
        
		WindowInit();
		KeyboardInit();
		MouseInit();
    }

    void FlowInitWindow()
    {
		if( WindowOpen( _windowFormat ) )
		{
			gs::Init();
			
			if( _appInit )
			{
				_appInit();
			}
            
            _state = StateInitialised;
        }
    }
    
    void FlowTick()
    {
        f32 dt = core::TimeTick();
        
        WindowTick();
        
        if( _appTick )
        {
            _appTick( dt );
        }
        
        KeyboardTick( dt );
        MouseTick( dt );
        
        if( KeyboardDown( KeyEscape ) )
        {
            Kill();
        }
    }

    void FlowTouch( const Touch* touches, s32 touchCount )
    {
        if( _appTouch )
        {
            _appTouch( touches, touchCount );
        }
    }
    
    void FlowDraw()
    {
        gs::Put();
        gs::SetViewport( 0, 0, WindowSizeX(), WindowSizeY() );
        gs::SetWrite( gs::WriteMaskRgbaz );
        gs::Clear( true, true );
        gs::SetWrite( gs::WriteMaskRgbz );
        
        if( _appDraw )
        {
            _appDraw();
        }
        
        MouseDraw();
        TouchDraw();
        
        gs::Pop();
    }
    
    void FlowKill()
    {
        if( _state == StateInitialised )
        {
            if( _appKill )
			{
				_appKill();
			}
			
			gs::Kill();
        }
        
        TimeKill();
		
		KillHw();
    }
}