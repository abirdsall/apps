#ifndef OS_MOUSE_H
#define OS_MOUSE_H

namespace os
{
	enum MouseButton
	{
		MouseButtonLeft,
		MouseButtonRight,
		MouseButtonMiddle,
		MouseButtonCount
	};
	
	void MouseInit();
	void MouseTick( f32 dt );
	void MouseDraw();
	void MouseReleaseAll();
	
	bool MouseButtonDown( MouseButton button );
	bool MouseButtonHeld( MouseButton button );
	bool MouseButtonUp( MouseButton button );
	f32 MouseButtonTime( MouseButton button );
	
	v2 MousePositionPixels();
	v2 MousePosition();
	void MouseSetPosition( s16 x, s16 y );
	v2 MouseVelocity();
	
	void MouseSetVisibile( bool visible );
	bool MouseVisibile();
}

#endif