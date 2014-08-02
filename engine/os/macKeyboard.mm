#include "os.h"

#if kBuildMac

namespace os
{
	extern void OnKey( const eKey key, const bool pressed );

	void OnModifierChange( const u32 modifiers )
	{
#if 0
		static u32 sPreviousModifiers = 0;
		
		const u32 changed = modifiers ^ sPreviousModifiers;
		
		sPreviousModifiers = modifiers;
		
		if( changed & shiftKey )
		{
			OnKey( eKeyShift, modifiers & shiftKey );
		}
		
		if( changed & rightShiftKey )
		{
			OnKey( eKeyShift, modifiers & rightShiftKey );
		}
		
		if( changed & controlKey )
		{
			OnKey( eKeyCtrl, modifiers & controlKey );
		}
		
		if( changed & rightControlKey )
		{
			OnKey( eKeyCtrl, modifiers & rightControlKey );
		}
#endif
	}

	void OnKeyChange( const u32 keyCode, const bool pressed )
	{
		switch( keyCode )
		{  
			case 0x00 : OnKey( eKeyA, pressed ); break;
			case 0x0B : OnKey( eKeyB, pressed ); break;
			case 0x08 : OnKey( eKeyC, pressed ); break;
			case 0x02 : OnKey( eKeyD, pressed ); break;
			case 0x0E : OnKey( eKeyE, pressed ); break;
			case 0x03 : OnKey( eKeyF, pressed ); break;
			case 0x05 : OnKey( eKeyG, pressed ); break;
			case 0x04 : OnKey( eKeyH, pressed ); break;
			case 0x22 : OnKey( eKeyI, pressed ); break;
			case 0x26 : OnKey( eKeyJ, pressed ); break;
			case 0x28 : OnKey( eKeyK, pressed ); break;
			case 0x25 : OnKey( eKeyL, pressed ); break;
			case 0x2E : OnKey( eKeyM, pressed ); break;
			case 0x2D : OnKey( eKeyN, pressed ); break;
			case 0x1F : OnKey( eKeyO, pressed ); break;
			case 0x23 : OnKey( eKeyP, pressed ); break;
			case 0x0C : OnKey( eKeyQ, pressed ); break;
			case 0x0F : OnKey( eKeyR, pressed ); break;
			case 0x01 : OnKey( eKeyS, pressed ); break;
			case 0x11 : OnKey( eKeyT, pressed ); break;
			case 0x20 : OnKey( eKeyU, pressed ); break;
			case 0x09 : OnKey( eKeyV, pressed ); break;
			case 0x0D : OnKey( eKeyW, pressed ); break;
			case 0x07 : OnKey( eKeyX, pressed ); break;
			case 0x10 : OnKey( eKeyY, pressed ); break;
			case 0x06 : OnKey( eKeyZ, pressed ); break;
			case 0x1D : OnKey( eKey0, pressed ); break;
			case 0x12 : OnKey( eKey1, pressed ); break;
			case 0x13 : OnKey( eKey2, pressed ); break;
			case 0x14 : OnKey( eKey3, pressed ); break;
			case 0x15 : OnKey( eKey4, pressed ); break;
			case 0x17 : OnKey( eKey5, pressed ); break;
			case 0x16 : OnKey( eKey6, pressed ); break;
			case 0x1A : OnKey( eKey7, pressed ); break;
			case 0x1C : OnKey( eKey8, pressed ); break;
			case 0x19 : OnKey( eKey9, pressed ); break;
			case 0x7B : OnKey( eKeyUp, pressed ); break;
			case 0x7C : OnKey( eKeyDown, pressed ); break;
			case 0x7E : OnKey( eKeyLeft, pressed ); break;
			case 0x7D : OnKey( eKeyRight, pressed ); break;
			case 0x3B : OnKey( eKeyCtrl, pressed ); break;
			case 0x38 : OnKey( eKeyShift, pressed ); break;
			case 0x24 : OnKey( eKeyEnter, pressed ); break;
			case 0x31 : OnKey( eKeySpace, pressed ); break;
			case 0x30 : OnKey( eKeyTab, pressed ); break;
			case 0x35 : OnKey( eKeyEscape, pressed ); break;
			case 0x33 : OnKey( eKeyDelete, pressed ); break;
		}
	}
}

#endif
