#ifndef OS_MOUSE_H
#define OS_MOUSE_H

namespace os
{
	enum eMouseButton
	{
		eMouseButtonLeft,
		eMouseButtonRight,
		eMouseButtonMiddle,
		eMouseButtonCount
	};
	
	void MouseInit();
	void MouseTick( f32 dt );
	void MouseDraw();
	void MouseReleaseAll();
	
	bool MouseButtonDown( const eMouseButton button );
	bool MouseButtonHeld( const eMouseButton button );
	bool MouseButtonUp( const eMouseButton button );
	f32 MouseButtonTime( const eMouseButton button );
	
	v2 MousePositionPixels();
	v2 MousePosition();
	void MouseSetPosition( s16 x, s16 y );
	v2 MouseVelocity();
	
	void MouseSetVisibile( bool visible );
	bool MouseVisibile();
}

#endif