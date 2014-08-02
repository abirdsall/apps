#ifndef OS_KEYBOARD
#define OS_KEYBOARD

namespace os
{
	enum eKey
	{
		eKeyA = 0,
		eKeyB,
		eKeyC,
		eKeyD,
		eKeyE,
		eKeyF,
		eKeyG,
		eKeyH,
		eKeyI,
		eKeyJ,
		eKeyK,
		eKeyL,
		eKeyM,
		eKeyN,
		eKeyO,
		eKeyP,
		eKeyQ,
		eKeyR,
		eKeyS,
		eKeyT,
		eKeyU,
		eKeyV,
		eKeyW,
		eKeyX,
		eKeyY,
		eKeyZ,
		eKey0,
		eKey1,
		eKey2,
		eKey3,
		eKey4,
		eKey5,
		eKey6,
		eKey7,
		eKey8,
		eKey9,
		eKeyLeft,
		eKeyRight,
		eKeyUp,
		eKeyDown,
		eKeyCtrl,
		eKeyShift,
		eKeyEnter,
		eKeySpace,
		eKeyTab,
		eKeyEscape,
		eKeyDelete,
		eKeyCount
	};
	
	void KeyboardInit();
	void KeyboardTick( f32 dt );
	void KeyboardReleaseAll();
	
	bool KeyUp( const eKey key );
	bool KeyDown( const eKey key );
	bool KeyHeld( const eKey key );
	f32  KeyTime( const eKey key );
}

#endif