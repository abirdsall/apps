#ifndef OS_HW_KEYBOARD
#define OS_HW_KEYBOARD

namespace os
{
	void OnModifierChange( const u32 modifiers );
	void OnKeyChange( const u32 keyCode, const bool pressed );
}

#endif