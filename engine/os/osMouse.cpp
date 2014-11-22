#include "os.h"

namespace os
{
	s16 _positionPixelsX;
	s16 _positionPixelsY;
	v2 _position;
	v2 _velocity;
	bool _locked;
	
	f32 _buttonTime[ MouseButtonCount ];
	u8 _button[ MouseButtonCount ];
	
	void OnMouseButton( MouseButton button, bool pressed )
	{
		if( button < MouseButtonCount )
		{
			if( pressed )
			{
				_buttonTime[ button ] = 0.0f;
				_button[ button ] |= _button[ button ] & 0x2 ? 0x2 : 0x3;
			}
			else
			{
				_button[ button ] = 0x4;
			}
		}
	}
	
	void OnMouseMove( s32 x, s32 y, s32 dx, s32 dy )
	{
		if( _locked )
		{
			_positionPixelsX += dx;
			_positionPixelsY += dy;
		}
		else
		{
			_positionPixelsX = x;
			_positionPixelsY = y;
		}
	}
	
	bool MouseButtonDown( MouseButton button )
	{
		return _button[ button ] & 0x1 ? true : false;
	}
	
	bool MouseButtonHeld( MouseButton button )
	{
		return _button[ button ] & 0x2 ? true : false;
	}
	
	bool MouseButtonUp( MouseButton button )
	{
		return _button[ button ] & 0x4 ? true : false;
	}
	
	f32 MouseButtonTime( MouseButton button )
	{
		return _buttonTime[ button ];
	}
	
	void MouseInit()
	{
		_positionPixelsX = 0;
		_positionPixelsY = 0;
		_position.x = 0.0f;
		_position.y = 0.0f;
		_velocity.x = 0.0f;
		_velocity.y = 0.0f;
		_locked = false;
		
		for( s32 i = 0; i < MouseButtonCount; i++ )
		{
			_buttonTime[ i ] = 0.0f;
			_button[ i ] = 0;
		}
	}
	
	void MouseTick( f32 dt )
	{
		v2 position = _position;
		
		_position = v2( f32( _positionPixelsX ) / f32( WindowSizeX() ), f32( _positionPixelsY ) / f32( WindowSizeY() ) );
		_velocity = lerp( _velocity, _position - position, 0.25f );
		
		for( s32 i = 0; i < MouseButtonCount; i++ )
		{
			_button[ i ] &= ~0x5;
			
			if( MouseButtonHeld( ( MouseButton )i ) )
			{
				_buttonTime[ i ] += dt;
			}
		}
	}
	
	void MouseDraw()
	{
		if( MouseVisibile() )
		{
			//gsFillCursor();
		}
	}
	
	void MouseReleaseAll()
	{
		for( s32 i = 0; i < MouseButtonCount; i++ )
		{
			if( _button[ i ] & 0x2 )
			{
				OnMouseButton( ( MouseButton )i, false );
			}
		}
	}
	
	v2 MousePositionPixels()
	{
		return v2( _positionPixelsX, _positionPixelsY );
	}
	
	v2 MousePosition()
	{
		return _position;
	}
	
	void MouseSetPosition( s16 x, s16 y )
	{
		MouseSetPositionHw( x, y );
	}
	
	v2 MouseVelocity()
	{
		return _velocity;
	}
	
	bool MouseVisibile()
	{
		return !_locked;
	}
	
	void MouseSetVisibile( bool visible )
	{
		if( !WindowOpened() )
		{
			ASSERT( 0 );
			return;
		}
		
		_position.x = 0.5f;
		_position.y = 0.5f;
		
		s32 centerPosX = WindowSizeX() / 2;
		s32 centerPosY = WindowSizeY() / 2;
		
		if( visible )
		{
			if( !_locked )
			{
				ASSERT( 0 );
				return;
			}
			_locked = false;
		}
		else
		{
			if( _locked )
			{
				ASSERT( 0 );
				return;
			}
		}
		
		if ( centerPosX != _positionPixelsX || centerPosY != _positionPixelsY )
		{
			MouseSetPosition( centerPosX, centerPosY );
			_positionPixelsX = centerPosX;
			_positionPixelsY = centerPosY;
		}
		
		return MouseSetVisibileHw( visible );
	}
}
