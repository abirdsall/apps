#include "os.h"

#if OsOsx

#include "mac.h"

extern id _windowRef;

namespace os
{	
	void MouseSetVisibileHw( bool visible )
	{
		if( visible )
		{
			[NSCursor unhide];
			CGAssociateMouseAndMouseCursorPosition( true );
		}
		else
		{
			[NSCursor hide];
			CGAssociateMouseAndMouseCursorPosition( false );
		}
	}
	
	void MouseSetPositionHw( s16 x, s16 y )
	{
		bool fullscreen = false;
		if( fullscreen )
		{
			NSPoint globalPoint = NSMakePoint( x, y );
			CGPoint globalPointCG = CGPointMake( globalPoint.x, globalPoint.y );
			CGDisplayMoveCursorToPoint( CGMainDisplayID(), globalPointCG );
		}
		else
		{
			NSPoint localPoint = NSMakePoint( x, os::WindowSizeY() - y - 1 );
			NSPoint globalPoint = [_windowRef convertBaseToScreen:localPoint];
			CGPoint mainScreenOrigin = CGDisplayBounds( CGMainDisplayID() ).origin;
			double mainScreenHeight = CGDisplayBounds( CGMainDisplayID() ).size.height;
			CGPoint targetPoint = CGPointMake( globalPoint.x - mainScreenOrigin.x, mainScreenHeight - globalPoint.y - mainScreenOrigin.y );
			CGDisplayMoveCursorToPoint( CGMainDisplayID(), targetPoint );
		}
	}
}

#endif
