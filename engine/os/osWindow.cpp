#include "os.h"

namespace os
{
    WindowFormat* _windowFormat;

	f32 _pixelSizeX;
	f32 _pixelSizeY;
	f32 _aspect;
	
	bool _active;
	bool _opened;
	bool _iconified;
    
    void OnWindowClose()
	{
		os::Kill();
	}
	
	void OnWindowIconified( bool iconified )
	{
		_iconified = iconified;
	}
	
	bool OnWindowActivated( bool activated )
	{
		if ( activated )
		{
			_active = true;
		}
		else
		{
			_active = false;
			MouseReleaseAll();
			KeyboardReleaseAll();
		}
		return( true );
	}
	
	void OnWindowResize( s32 sizeX, s32 sizeY )
	{
        _windowFormat->_sizeX = sizeX;
        _windowFormat->_sizeY = sizeY;
        
        ASSERT( _windowFormat->_sizeX > 0 );
        ASSERT( _windowFormat->_sizeY > 0 );
        
        _pixelSizeX = 1.0f / f32( _windowFormat->_sizeX );
        _pixelSizeY = 1.0f / f32( _windowFormat->_sizeY );
        
        _aspect = f32( _windowFormat->_sizeY ) / f32( _windowFormat->_sizeX );
	}
	    
	bool WindowOpen( WindowFormat& windowFormat )
	{
		if( _opened )
		{
			ASSERT( 0 ); // Error - Multiple windows are not supported
			return false;
		}

		_windowFormat = &windowFormat;
        
        if( !windowFormat._native )
        {
            OnWindowResize( windowFormat._sizeX, windowFormat._sizeY );
        }
        
		_active = true;
        _opened = false;
        _iconified = false;
		
		if( WindowOpenHw( windowFormat ) )
		{
			_opened = true;
			
			return true;
		}
		
		return false;
	}
	
	s32 WindowSizeX( void )
	{
		ASSERT( _opened );
		return( _windowFormat->_sizeX );
	}
	
	s32 WindowSizeY( void )
	{
		ASSERT( _opened );
		return( _windowFormat->_sizeY );
	}
	
	f32 WindowPixelSizeX( void )
	{
		ASSERT( _opened );
		return( _pixelSizeX );
	}
	
	f32 WindowPixelSizeY( void )
	{
		ASSERT( _opened );
		return( _pixelSizeY );
	}
	
	f32 WindowAspect( void )
	{
		ASSERT( _opened );
		return( _aspect );
	}
	
	bool WindowOpened( void )
	{
		return( _opened );
	}
	
	void WindowInit()
	{
        _pixelSizeX = 0.0f;
        _pixelSizeY = 0.0f;
        _aspect = 0.0f;
        
        _active = false;
        _opened = false;
        _iconified = false;

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