#ifndef OS_KEYBOARD
#define OS_KEYBOARD

namespace os
{
	enum Key
	{
		KeyA = 0,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ,
		Key0,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
		KeyLeft,
		KeyRight,
		KeyUp,
		KeyDown,
		KeyCtrl,
		KeyShift,
		KeyEnter,
		KeySpace,
		KeyTab,
		KeyEscape,
		KeyDelete,
		KeyCount
	};
	
	void KeyboardInit();
	void KeyboardTick( f32 dt );
	void KeyboardReleaseAll();
	
	bool KeyboardUp( const Key key );
	bool KeyboardDown( const Key key );
	bool KeyboardHeld( const Key key );
	f32  KeyboardTime( const Key key );
}

#endif