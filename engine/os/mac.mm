#include "os.h"

#if OsOsx

#include "mac.h"

#include <pthread.h>

void* sNsglFramework;
pthread_key_t sNsglThread;
CGEventSourceRef sEventSource;
id sAutoreleasePool;

static void changeToResourcesDirectory( void )
{
    char resourcesPath[MAXPATHLEN];
    CFBundleRef bundle = CFBundleGetMainBundle();
    if( !bundle )
	{
        return;
	}
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL( bundle );
    CFStringRef last = CFURLCopyLastPathComponent( resourcesURL );
    if( CFStringCompare( CFSTR( "Resources" ), last, 0 ) != kCFCompareEqualTo )
    {
        CFRelease( last );
        CFRelease( resourcesURL );
        return;
    }
    CFRelease( last );
    if( !CFURLGetFileSystemRepresentation( resourcesURL, true,(UInt8*) resourcesPath, MAXPATHLEN) )
    {
        CFRelease( resourcesURL );
        return;
    }
    CFRelease( resourcesURL );
    chdir( resourcesPath );
}

namespace os
{
	bool InitContext()
	{
		if( pthread_key_create( &sNsglThread, nil ) != 0)
		{
			return false;
		}
		
		sNsglFramework = CFBundleGetBundleWithIdentifier( CFSTR( "com.apple.opengl" ) );
		if( !sNsglFramework )
		{
			return false;
		}
		
		return true;
	}
	
	void KillContext()
	{
		pthread_key_delete( sNsglThread );
		
        sNsglThread = 0;//nil;
		sNsglFramework = nil;
	}
    
    bool MainHw( int argc, char *argv[] )
    {
        os::FlowInit();
        os::FlowInitWindow();
        
        while( os::FlowActive() )
        {
            os::FlowTick();
            os::FlowDraw();
        }
        
        os::FlowKill();
        
        return true;
    }
	
	bool InitHw()
	{
		sAutoreleasePool = [[NSAutoreleasePool alloc] init];

		InitContext();
		
		changeToResourcesDirectory();
		
		sEventSource = CGEventSourceCreate( kCGEventSourceStateHIDSystemState );
		if( !sEventSource )
		{
			return false;
		}
		
		CGEventSourceSetLocalEventsSuppressionInterval( sEventSource, 0.0 );
		
		return true;
	}
    
	void KillHw()
	{
		if( sEventSource )
		{
			CFRelease( sEventSource );
			sEventSource = nil;
		}

		[sAutoreleasePool release];
		sAutoreleasePool = nil;
		
		KillContext();
	}
}

#endif
