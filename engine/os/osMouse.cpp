#include "os.h"

namespace os
{
	s16 sPositionPixelsX;
	s16 sPositionPixelsY;
	v2 sPosition;
	v2 sVelocity;
	bool sLocked;
	
	f32 sButtonTime[ eMouseButtonCount ];
	u8 sButton[ eMouseButtonCount ];
	
	void OnMouseButton( const eMouseButton button, const bool pressed )
	{
		if( button < eMouseButtonCount )
		{
			if( pressed )
			{
				sButtonTime[ button ] = 0.0f;
				sButton[ button ] |= sButton[ button ] & 0x2 ? 0x2 : 0x3;
			}
			else
			{
				sButton[ button ] = 0x4;
			}
		}
	}
	
	void OnMouseMove( s32 x, s32 y, s32 dx, s32 dy )
	{
		if( sLocked )
		{
			sPositionPixelsX += dx;
			sPositionPixelsY += dy;
		}
		else
		{
			sPositionPixelsX = x;
			sPositionPixelsY = y;
		}
	}
	
	bool MouseButtonDown( const eMouseButton button )
	{
		return sButton[ button ] & 0x1 ? true : false;
	}
	
	bool MouseButtonHeld( const eMouseButton button )
	{
		return sButton[ button ] & 0x2 ? true : false;
	}
	
	bool MouseButtonUp( const eMouseButton button )
	{
		return sButton[ button ] & 0x4 ? true : false;
	}
	
	f32 MouseButtonTime( const eMouseButton button )
	{
		return sButtonTime[ button ];
	}
	
	void MouseInit()
	{
		sPositionPixelsX = 0;
		sPositionPixelsY = 0;
		sPosition.x = 0.0f;
		sPosition.y = 0.0f;
		sVelocity.x = 0.0f;
		sVelocity.y = 0.0f;
		sLocked = false;
		
		for( s32 i = 0; i < eMouseButtonCount; i++ )
		{
			sButtonTime[ i ] = 0.0f;
			sButton[ i ] = 0;
		}
	}
	
	void MouseTick( f32 dt )
	{
		v2 position = sPosition;
		
		sPosition = v2( f32( sPositionPixelsX ) / f32( WindowSizeX() ), f32( sPositionPixelsY ) / f32( WindowSizeY() ) );
		sVelocity = lerp( sVelocity, sPosition - position, 0.25f );
		
		for( s32 i = 0; i < eMouseButtonCount; i++ )
		{
			sButton[ i ] &= ~0x5;
			
			if( MouseButtonHeld( ( eMouseButton )i ) )
			{
				sButtonTime[ i ] += dt;
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
		for( s32 i = 0; i < eMouseButtonCount; i++ )
		{
			if( sButton[ i ] & 0x2 )
			{
				OnMouseButton( ( eMouseButton )i, false );
			}
		}
	}
	
	v2 MousePositionPixels()
	{
		return v2( sPositionPixelsX, sPositionPixelsY );
	}
	
	v2 MousePosition()
	{
		return sPosition;
	}
	
	void MouseSetPosition( s16 x, s16 y )
	{
		MouseSetPositionHw( x, y );
	}
	
	v2 MouseVelocity()
	{
		return sVelocity;
	}
	
	bool MouseVisibile()
	{
		return !sLocked;
	}
	
	void MouseSetVisibile( bool visible )
	{
		if( !WindowOpened() )
		{
			ASSERT( 0 );
			return;
		}
		
		sPosition.x = 0.5f;
		sPosition.y = 0.5f;
		
		s32 centerPosX = WindowSizeX() / 2;
		s32 centerPosY = WindowSizeY() / 2;
		
		if( visible )
		{
			if( !sLocked )
			{
				ASSERT( 0 );
				return;
			}
			sLocked = false;
		}
		else
		{
			if( sLocked )
			{
				ASSERT( 0 );
				return;
			}
		}
		
		if ( centerPosX != sPositionPixelsX || centerPosY != sPositionPixelsY )
		{
			MouseSetPosition( centerPosX, centerPosY );
			sPositionPixelsX = centerPosX;
			sPositionPixelsY = centerPosY;
		}
		
		return MouseSetVisibileHw( visible );
	}
}
