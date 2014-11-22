#include "os.h"

namespace os
{
	static f32 _keyTime[ KeyCount ];
	static u8 _key[ KeyCount ];
	
	void KeyboardInit()
	{
		for( s32 i = 0; i < KeyCount; i++ )
		{
			_keyTime[ i ] = 0.0f;
			_key[ i ] = 0;
		}
	}
	
	void KeyboardTick( f32 dt )
	{
		for( s32 i = 0; i < KeyCount; i++ )
		{
			_key[ i ] &= ~0x5;
			
			if( KeyboardHeld( ( Key )i ) )
			{
				_keyTime[ i ] += dt;
			}
		}
	}
	
	void KeyboardReleaseAll( void )
	{
		for( s32 i = 0; i < KeyCount; i++ )
		{
			if( _key[ i ] & 0x2 )
			{
				OnKeyChange( ( Key )i, false );
			}
		}
	}
	
	bool KeyboardDown( const Key key )
	{
		return _key[ key ] & 0x1;
	}
	
	bool KeyboardHeld( const Key key )
	{
		return _key[ key ] & 0x2;
	}
	
	bool KeyboardUp( const Key key )
	{
		return _key[ key ] & 0x4;
	}
	
	f32 KeyboardTime( const Key key )
	{
		return _keyTime[ key ];
	}
	
	void OnKey( const Key key, bool pressed )
	{
		if( key < KeyCount )
		{
			if( pressed )
			{
				_keyTime[ key ] = 0.0f;
				_key[ key ] |= _key[ key ] & 0x2 ? 0x2 : 0x3;
			}
			else
			{
				_key[ key ] = 0x4;
			}
		}
	}
}
