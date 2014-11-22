#include "os.h"

#if OsOsx

namespace os
{
	extern void OnKey( const Key key, bool pressed );

	void OnModifierChange( u32 modifiers )
	{
#if 0
		static u32 sPreviousModifiers = 0;
		
		u32 changed = modifiers ^ sPreviousModifiers;
		
		sPreviousModifiers = modifiers;
		
		if( changed & shiftKey )
		{
			OnKey( KeyShift, modifiers & shiftKey );
		}
		
		if( changed & rightShiftKey )
		{
			OnKey( KeyShift, modifiers & rightShiftKey );
		}
		
		if( changed & controlKey )
		{
			OnKey( KeyCtrl, modifiers & controlKey );
		}
		
		if( changed & rightControlKey )
		{
			OnKey( KeyCtrl, modifiers & rightControlKey );
		}
#endif
	}

	void OnKeyChange( u32 keyCode, bool pressed )
	{
		switch( keyCode )
		{  
			case 0x00 : OnKey( KeyA, pressed ); break;
			case 0x0B : OnKey( KeyB, pressed ); break;
			case 0x08 : OnKey( KeyC, pressed ); break;
			case 0x02 : OnKey( KeyD, pressed ); break;
			case 0x0E : OnKey( KeyE, pressed ); break;
			case 0x03 : OnKey( KeyF, pressed ); break;
			case 0x05 : OnKey( KeyG, pressed ); break;
			case 0x04 : OnKey( KeyH, pressed ); break;
			case 0x22 : OnKey( KeyI, pressed ); break;
			case 0x26 : OnKey( KeyJ, pressed ); break;
			case 0x28 : OnKey( KeyK, pressed ); break;
			case 0x25 : OnKey( KeyL, pressed ); break;
			case 0x2E : OnKey( KeyM, pressed ); break;
			case 0x2D : OnKey( KeyN, pressed ); break;
			case 0x1F : OnKey( KeyO, pressed ); break;
			case 0x23 : OnKey( KeyP, pressed ); break;
			case 0x0C : OnKey( KeyQ, pressed ); break;
			case 0x0F : OnKey( KeyR, pressed ); break;
			case 0x01 : OnKey( KeyS, pressed ); break;
			case 0x11 : OnKey( KeyT, pressed ); break;
			case 0x20 : OnKey( KeyU, pressed ); break;
			case 0x09 : OnKey( KeyV, pressed ); break;
			case 0x0D : OnKey( KeyW, pressed ); break;
			case 0x07 : OnKey( KeyX, pressed ); break;
			case 0x10 : OnKey( KeyY, pressed ); break;
			case 0x06 : OnKey( KeyZ, pressed ); break;
			case 0x1D : OnKey( Key0, pressed ); break;
			case 0x12 : OnKey( Key1, pressed ); break;
			case 0x13 : OnKey( Key2, pressed ); break;
			case 0x14 : OnKey( Key3, pressed ); break;
			case 0x15 : OnKey( Key4, pressed ); break;
			case 0x17 : OnKey( Key5, pressed ); break;
			case 0x16 : OnKey( Key6, pressed ); break;
			case 0x1A : OnKey( Key7, pressed ); break;
			case 0x1C : OnKey( Key8, pressed ); break;
			case 0x19 : OnKey( Key9, pressed ); break;
			case 0x7E : OnKey( KeyUp, pressed ); break;
			case 0x7D : OnKey( KeyDown, pressed ); break;
			case 0x7B : OnKey( KeyLeft, pressed ); break;
			case 0x7C : OnKey( KeyRight, pressed ); break;
			case 0x3B : OnKey( KeyCtrl, pressed ); break;
			case 0x38 : OnKey( KeyShift, pressed ); break;
			case 0x24 : OnKey( KeyEnter, pressed ); break;
			case 0x31 : OnKey( KeySpace, pressed ); break;
			case 0x30 : OnKey( KeyTab, pressed ); break;
			case 0x35 : OnKey( KeyEscape, pressed ); break;
			case 0x33 : OnKey( KeyDelete, pressed ); break;
		}
	}
}

#endif
