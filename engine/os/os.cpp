#include "os.h"
#include "gs.h"
#include "fw.h"

namespace os
{
    static void ( *sAppInit )();
    static void ( *sAppTick )( f32 dt );
    static void ( *sAppTouch )( const Touch* touches, s32 touchCount );
    static void ( *sAppDraw )();
    static void ( *sAppKill )();
    static WindowFormat sWindowFormat;
    static eState sState = eStateUninitialised;
	static bool sKilled = false;
	
	void Kill( void )
	{
        sKilled = true;
	}
	
    bool Main( int argc, char *argv[],
              void ( *appInit )(),
              void ( *appTick )( f32 dt ),
              void ( *appTouch )( const os::Touch* touches, s32 touchCount ),
              void ( *appDraw )(),
              void ( *appKill )(), WindowFormat windowFormat )
    {
        sAppInit = appInit;
        sAppTick = appTick;
        sAppTouch = appTouch;
        sAppDraw = appDraw;
        sAppKill = appKill;
        sWindowFormat = windowFormat;
        
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
        return sState == eStateInitialised && !sKilled;
    }
    
    void FlowInit()
    {
        InitHw();
		
		MathInit();
        TimeInit();
        
		WindowInit();
		KeyboardInit();
		MouseInit();
    }

    void FlowInitWindow()
    {
		if( WindowOpen( sWindowFormat ) )
		{
			gs::Init();
			fw::InitCameras();
			
			if( sAppInit )
			{
				sAppInit();
			}
            
            sState = eStateInitialised;
        }
    }
    
    void FlowTick()
    {
        f32 dt = core::TimeTick();
        
        WindowTick();
        
        fw::TickCameras( dt );
        
        if( sAppTick )
        {
            sAppTick( dt );
        }
        
        KeyboardTick( dt );
        MouseTick( dt );
        
        if( KeyDown( eKeyEscape ) )
        {
            Kill();
        }
    }

    void FlowTouch( const Touch* touches, s32 touchCount )
    {
        if( sAppTouch )
        {
            sAppTouch( touches, touchCount );
        }
    }
    
    void FlowDraw()
    {
        gs::Put();
        gs::SetViewport( 0, 0, WindowSizeX(), WindowSizeY() );
        gs::SetWrite( gs::eWriteRgbaz );
        gs::Clear( true, true );
        gs::SetWrite( gs::eWriteRgbz );
        
        fw::DrawCameras();
        
        MouseDraw();
        TouchDraw();
        
        gs::Pop();
    }
    
    void FlowKill()
    {
        if( sState == eStateInitialised )
        {
            if( sAppKill )
			{
				sAppKill();
			}
			
			fw::KillCameras();
			gs::Kill();
        }
        
        MathKill();
        TimeKill();
		
		KillHw();
    }
}