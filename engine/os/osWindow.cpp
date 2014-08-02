#include "os.h"

namespace os
{
    WindowFormat* sWindowFormat;

	f32 sPixelSizeX;
	f32 sPixelSizeY;
	f32 sAspect;
	
	bool sActive;
	bool sOpened;
	bool sIconified;
    
    void OnWindowClose()
	{
		os::Kill();
	}
	
	void OnWindowIconified( const bool iconified )
	{
		sIconified = iconified;
	}
	
	bool OnWindowActivated( const bool activated )
	{
		if ( activated )
		{
			sActive = true;
		}
		else
		{
			sActive = false;
			MouseReleaseAll();
			KeyboardReleaseAll();
		}
		return( true );
	}
	
	void OnWindowResize( s32 sizeX, s32 sizeY )
	{
        sWindowFormat->mSizeX = sizeX;
        sWindowFormat->mSizeY = sizeY;
        
        ASSERT( sWindowFormat->mSizeX > 0 );
        ASSERT( sWindowFormat->mSizeY > 0 );
        
        sPixelSizeX = 1.0f / f32( sWindowFormat->mSizeX );
        sPixelSizeY = 1.0f / f32( sWindowFormat->mSizeY );
        
        sAspect = f32( sWindowFormat->mSizeY ) / f32( sWindowFormat->mSizeX );
	}
	    
	bool WindowOpen( WindowFormat& windowFormat )
	{
		if( sOpened )
		{
			ASSERT( 0 ); // Error - Multiple windows are not supported
			return false;
		}

		sWindowFormat = &windowFormat;
        
        if( !windowFormat.mNative )
        {
            OnWindowResize( windowFormat.mSizeX, windowFormat.mSizeY );
        }
        
		sActive = true;
        sOpened = false;
        sIconified = false;
		
		if( WindowOpenHw( windowFormat ) )
		{
			sOpened = true;
			
			return true;
		}
		
		return false;
	}
	
	s32 WindowSizeX( void )
	{
		ASSERT( sOpened );
		return( sWindowFormat->mSizeX );
	}
	
	s32 WindowSizeY( void )
	{
		ASSERT( sOpened );
		return( sWindowFormat->mSizeY );
	}
	
	f32 WindowPixelSizeX( void )
	{
		ASSERT( sOpened );
		return( sPixelSizeX );
	}
	
	f32 WindowPixelSizeY( void )
	{
		ASSERT( sOpened );
		return( sPixelSizeY );
	}
	
	f32 WindowAspect( void )
	{
		ASSERT( sOpened );
		return( sAspect );
	}
	
	bool WindowOpened( void )
	{
		return( sOpened );
	}
	
	void WindowInit()
	{
        sPixelSizeX = 0.0f;
        sPixelSizeY = 0.0f;
        sAspect = 0.0f;
        
        sActive = false;
        sOpened = false;
        sIconified = false;

		WindowInitHw();
	}
	
	void WindowTick()
	{
		WindowTickHw();
	}
	
	void WindowKill()
	{
		WindowKillHw();
	}
}