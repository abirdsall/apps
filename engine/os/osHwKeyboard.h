#ifndef OS_HW_KEYBOARD
#define OS_HW_KEYBOARD

namespace os
{
	void OnModifierChange( u32 modifiers );
	void OnKeyChange( u32 keyCode, bool pressed );
}

#endif