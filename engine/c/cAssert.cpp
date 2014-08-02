#include "os.h"
//#include "gs.h"

#if kEnableAssert || kEnablePanic

namespace os
{
	void Assert( const c8* fileName, s32 lineNumber, bool success, bool fatal )
	{
		if ( !success )
		{
#if 0
			osString reason( "assert\nfile %s\nline %d\npress c to continue\npress q to quit", fileName, lineNumber );
			
			printf( "%s\n", reason.getString() );
			
			while ( true )
			{
				osWindow().beginFrame();
				gs::Set2d();
				v2 textSize = gsGetTextSize( reason.getString() );
				gsFillRectangle( rect( 0.0f, 0.0f, osWindow().sizeX(), osWindow().sizeY() ), v4( 0.0f, 0.0f, 0.0f, 0.8f ) );
				gsFillRectangle( rect( 0.0f, 0.0f, textSize.x, textSize.y ), v4( 1.0f, 0.25f, 0.0f, 1.0f ) );
				gsFillText( v2( 0.0f, 0.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ), reason.getString() );
				if ( osKeyboard().getKeyDown( OS_KEY_C ) )
				{
					break;
				}
				if ( osKeyboard().getKeyDown( OS_KEY_Q ) )
				{
					fatal = true;
					break;
				}
				osWindow().endFrame();
			}
			
			if ( fatal )
			{
				os->kill();
			}
#endif
		}
	}
}

#endif