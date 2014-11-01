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
        sWindowFormat->_sizeX = sizeX;
        sWindowFormat->_sizeY = sizeY;
        
        ASSERT( sWindowFormat->_sizeX > 0 );
        ASSERT( sWindowFormat->_sizeY > 0 );
        
        sPixelSizeX = 1.0f / f32( sWindowFormat->_sizeX );
        sPixelSizeY = 1.0f / f32( sWindowFormat->_sizeY );
        
        sAspect = f32( sWindowFormat->_sizeY ) / f32( sWindowFormat->_sizeX );
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
            OnWindowResize( windowFormat._sizeX, windowFormat._sizeY );
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
		return( sWindowFormat->_sizeX );
	}
	
	s32 WindowSizeY( void )
	{
		ASSERT( sOpened );
		return( sWindowFormat->_sizeY );
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