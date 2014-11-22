#include "os.h"

#if OsOsx

#include "mac.h"

#include <pthread.h>

void* _nsglFramework;
pthread_key_t _nsglThread;
CGEventSourceRef _eventSource;
id _autoreleasePool;

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
		if( pthread_key_create( &_nsglThread, nil ) != 0)
		{
			return false;
		}
		
		_nsglFramework = CFBundleGetBundleWithIdentifier( CFSTR( "com.apple.opengl" ) );
		if( !_nsglFramework )
		{
			return false;
		}
		
		return true;
	}
	
	void KillContext()
	{
		pthread_key_delete( _nsglThread );
		
        _nsglThread = 0;//nil;
		_nsglFramework = nil;
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
		_autoreleasePool = [[NSAutoreleasePool alloc] init];

		InitContext();
		
		changeToResourcesDirectory();
		
		_eventSource = CGEventSourceCreate( kCGEventSourceStateHIDSystemState );
        
		if( !_eventSource )
		{
			return false;
		}
		
		CGEventSourceSetLocalEventsSuppressionInterval( _eventSource, 0.0 );
		
		return true;
	}
    
	void KillHw()
	{
		if( _eventSource )
		{
			CFRelease( _eventSource );
			_eventSource = nil;
		}

		[_autoreleasePool release];
		_autoreleasePool = nil;
		
		KillContext();
	}
}

#endif
