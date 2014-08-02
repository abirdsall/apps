#include "os.h"

namespace os
{
	static f32 sKeyTime[ eKeyCount ];
	static u8 sKey[ eKeyCount ];
	
	void KeyboardInit()
	{
		for( s32 i = 0; i < eKeyCount; i++ )
		{
			sKeyTime[ i ] = 0.0f;
			sKey[ i ] = 0;
		}
	}
	
	void KeyboardTick( f32 dt )
	{
		for( s32 i = 0; i < eKeyCount; i++ )
		{
			sKey[ i ] &= ~0x5;
			
			if( KeyHeld( ( eKey )i ) )
			{
				sKeyTime[ i ] += dt;
			}
		}
	}
	
	void KeyboardReleaseAll( void )
	{
		for( s32 i = 0; i < eKeyCount; i++ )
		{
			if( sKey[ i ] & 0x2 )
			{
				OnKeyChange( ( eKey )i, false );
			}
		}
	}
	
	bool KeyDown( const eKey key )
	{
		return sKey[ key ] & 0x1;
	}
	
	bool KeyHeld( const eKey key )
	{
		return sKey[ key ] & 0x2;
	}
	
	bool KeyUp( const eKey key )
	{
		return sKey[ key ] & 0x4;
	}
	
	f32 KeyTime( const eKey key )
	{
		return sKeyTime[ key ];
	}
	
	void OnKey( const eKey key, const bool pressed )
	{
		if( key < eKeyCount )
		{
			if( pressed )
			{
				sKeyTime[ key ] = 0.0f;
				sKey[ key ] |= sKey[ key ] & 0x2 ? 0x2 : 0x3;
			}
			else
			{
				sKey[ key ] = 0x4;
			}
		}
	}
}
