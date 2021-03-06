#include "os.h"
#include "gs.h"

#if OsOsx

#include "mac.h"

#include <AvailabilityMacros.h>
#include <crt_externs.h>

namespace os
{
	extern void OnMouseButton( MouseButton button, bool pressed );
	extern void OnMouseMove( s32 x, s32 y, s32 dx, s32 dy );
	
	extern void OnWindowResize( s32 sizeX, s32 sizeY );
	extern bool OnWindowActivated( bool activated );
	extern void OnWindowIconified( bool iconified );
	extern void OnWindowClose();
}

id _windowContext;
id _windowRef;
id _windowDelegate;
id _windowPixelFormat;
NSDictionary* _originalMode;

//////////////////////////////////////////////////
//
// CocoaApplication
//
//////////////////////////////////////////////////

@interface CocoaApplication : NSApplication
@end

@implementation CocoaApplication

- (void)sendEvent:(NSEvent *)event
{
    if( [event type] == NSKeyUp && ( [event modifierFlags] & NSCommandKeyMask ) )
    {
        [[self keyWindow] sendEvent:event];
    }    
    else
    {
        [super sendEvent:event];
    }
}

@end

// Prior to Snow Leopard, we need to use this oddly-named semi-private API
// to get the application menu working properly.  Need to be careful in
// case it goes away in a future OS update.
@interface NSApplication (NSAppleMenu)
- (void)setAppleMenu:(NSMenu *)m;
@end

//////////////////////////////////////////////////
//
// Helper functions
//
//////////////////////////////////////////////////

// ABX ...this seems to be doing all the focusing crap!!!
static NSString* findAppName( void )
{
    // Keys to search for as potential application names
    NSString* keys[] = { @"CFBundleDisplayName", @"CFBundleName", @"CFBundleExecutable" };
	
    NSDictionary* infoDictionary = [[NSBundle mainBundle] infoDictionary];
	
    unsigned int i;
    for( i = 0; i < sizeof(keys) / sizeof(keys[0]); i++ )
    {
        id name = [infoDictionary objectForKey:keys[i]];
        if( name && [name isKindOfClass:[NSString class]] && ![@"" isEqualToString:name] )
        {
            return name;
        }
    }
	
    // Could do this only if we discover we're unbundled, but it should
    // do no harm...
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType( &psn, kProcessTransformToForegroundApplication );
	
    // Having the app in front of the terminal window is also generally
    // handy.  There is an NSApplication API to do this, but...
    SetFrontProcess( &psn );
	
    char** progname = _NSGetProgname();
    if( progname && *progname )
    {
        // TODO: UTF8?
        return [NSString stringWithUTF8String:*progname];
    }
	
    // Really shouldn't get here
    return @"Cocoa Application";
}

static void setUpMenuBar( void )
{
    NSString* appName = findAppName();
	
    NSMenu* bar = [[NSMenu alloc] init];
    [NSApp setMainMenu:bar];
	
    NSMenuItem* appMenuItem = [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
    NSMenu* appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];
	
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName] action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
    [appMenu addItem:[NSMenuItem separatorItem]];
	
	NSMenu *servicesMenu = [[NSMenu alloc] init];
    [NSApp setServicesMenu:servicesMenu];
    [[appMenu addItemWithTitle:@"Services" action:NULL keyEquivalent:@""] setSubmenu:servicesMenu];
    [appMenu addItem:[NSMenuItem separatorItem]];
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", appName] action:@selector(hide:) keyEquivalent:@"h"];
    [[appMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"] setKeyEquivalentModifierMask:NSAlternateKeyMask | NSCommandKeyMask];
    [appMenu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];
    [appMenu addItem:[NSMenuItem separatorItem]];
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName] action:@selector(terminate:) keyEquivalent:@"q"];
	
    NSMenuItem *windowMenuItem = [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
    NSMenu *windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
    [NSApp setWindowsMenu:windowMenu];
	[windowMenuItem setSubmenu:windowMenu];
	
    [windowMenu addItemWithTitle:@"Miniaturize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
    [windowMenu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
    [windowMenu addItem:[NSMenuItem separatorItem]];
    [windowMenu addItemWithTitle:@"Bring All to Front" action:@selector(arrangeInFront:) keyEquivalent:@""];
	
    // At least guard the call to private API to avoid an exception if it
    // goes away.  Hopefully that means the worst we'll break in future is to
    // look ugly...
    if( [NSApp respondsToSelector:@selector(setAppleMenu:)] )
    {
        [NSApp setAppleMenu:appMenu];
    }
}

static GLboolean initializeAppKit( void )
{
    if( NSApp )
    {
        return true;//GL_TRUE;
    }
	
    // Implicitly create shared NSApplication instance
    [CocoaApplication sharedApplication];
	
    // Setting up the menu bar must go between sharedApplication
    // above and finishLaunching below, in order to properly emulate the
    // behavior of NSApplicationMain
    setUpMenuBar();
	
	
    [NSApp finishLaunching];
	
    return true;//GL_TRUE;
}

//////////////////////////////////////////////////
//
// CocoaWindowDelegate
//
//////////////////////////////////////////////////

@interface CocoaWindowDelegate : NSObject
@end

@implementation CocoaWindowDelegate

- (BOOL)windowShouldClose:(id)window
{
	os::OnWindowClose();
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification
{
    [_windowContext update];
    NSRect contentRect = [_windowRef contentRectForFrameRect:[_windowRef frame]];
	os::OnWindowResize(contentRect.size.width,contentRect.size.height);
}

- (void)windowDidMove:(NSNotification *)notification
{
#if 0
    NSPoint point = [_windowRef mouseLocationOutsideOfEventStream];
    //_glfwInput.MousePosX = lround(floor(point.x));
    //_glfwInput.MousePosY = _glfwWin.height - lround(ceil(point.y));
	// OnMouseMove(^^^);
#endif
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
    os::OnWindowIconified( true );
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
    os::OnWindowIconified( false );
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	os::OnWindowActivated( true );
}

- (void)windowDidResignKey:(NSNotification *)notification
{
	os::OnWindowActivated( false );
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
	os::OnWindowClose();
    return NSTerminateCancel;
}
@end

//////////////////////////////////////////////////
//
// CocoaContentView
//
//////////////////////////////////////////////////

@interface CocoaContentView : NSView
@end

@implementation CocoaContentView

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)mouseDown:(NSEvent *)event
{
	os::OnMouseButton( os::MouseButtonLeft, true );
}

- (void)mouseUp:(NSEvent *)event
{
	os::OnMouseButton( os::MouseButtonLeft, false );
}

- (void)rightMouseDown:(NSEvent *)event
{
	os::OnMouseButton( os::MouseButtonRight, true );
}

- (void)rightMouseUp:(NSEvent *)event
{
	os::OnMouseButton( os::MouseButtonRight, false );
}

- (void)mouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)rightMouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

- (void)mouseMoved:(NSEvent *)event
{
	os::OnMouseMove( [event locationInWindow].x, [event locationInWindow].y, [event deltaX], [event deltaY] );
}

- (void)keyDown:(NSEvent *)event
{
	os::OnKeyChange( [event keyCode], true );
}

- (void)keyUp:(NSEvent *)event
{
	os::OnKeyChange( [event keyCode], false );
}

- (void)flagsChanged:(NSEvent *)event
{
	os::OnModifierChange( (u32)([event modifierFlags] | NSDeviceIndependentModifierFlagsMask) );
}
@end


//////////////////////////////////////////////////
//
// Window funtions
//
//////////////////////////////////////////////////

extern id _autoreleasePool;

namespace os
{
	void WindowInitHw()
	{
	}
	
	void WindowKillHw()
	{
		bool fullscreen = false;
		[_windowRef orderOut:nil];
		if( fullscreen )
		{
//			[[_windowRef contentView] exitFullScreenModeWithOptions:nil];
//			CGDisplaySwitchToMode( CGMainDisplayID(), (CFDictionaryRef)_originalMode );
//			CGReleaseAllDisplays();
		}
		[_windowPixelFormat release];
		_windowPixelFormat = nil;
		[NSOpenGLContext clearCurrentContext];
		[_windowContext release];
		_windowContext = nil;
		[_windowRef setDelegate:nil];
		[NSApp setDelegate:nil];
		[_windowDelegate release];
		_windowDelegate = nil;
		[_windowRef close];
		_windowRef = nil;
	}
	
	void WindowTickHw()
	{
		// swap buffer
		[_windowContext flushBuffer];
		
		// poll events
		NSEvent *event;
		do
		{
			event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
			if( event )
			{
				[NSApp sendEvent:event];
			}
		}
		while( event );
		[_autoreleasePool drain];
		_autoreleasePool = [[NSAutoreleasePool alloc] init];
	}
	
	
	void* WindowRefHw()
	{
		return ( void* )_windowRef;
	}
	
	bool WindowOpenHw( WindowFormat& windowFormat )
	{
		bool gl3 = GsOpenGl3;
		bool resizable = false;
		
		if( !initializeAppKit() )
		{
			// critical error
		}

		_windowDelegate = [[CocoaWindowDelegate alloc] init];
		if( _windowDelegate == nil )
		{
			// critical error
		}
		
		[NSApp setDelegate:_windowDelegate];
		
		unsigned int styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
			
			if( resizable )
			{
				styleMask |= NSResizableWindowMask;
			}
		
		_windowRef = [[NSWindow alloc] initWithContentRect:NSMakeRect( 0, 0, windowFormat._sizeX, windowFormat._sizeY ) styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
		[_windowRef setContentView:[[CocoaContentView alloc] init]];
		[_windowRef setDelegate:_windowDelegate];
		[_windowRef setAcceptsMouseMovedEvents:YES];
		[_windowRef center];

		if( [_windowRef respondsToSelector:@selector(setRestorable)] )
		{
			[_windowRef setRestorable:NO];
		}
		
		unsigned int attribute_count = 0;
#define ADD_ATTR(x) attributes[attribute_count++] = x
#define ADD_ATTR2(x, y) (void)({ ADD_ATTR(x); ADD_ATTR(y); })
#define MAX_ATTRS 64 // urrgh
		NSOpenGLPixelFormatAttribute attributes[MAX_ATTRS];
		
		ADD_ATTR( NSOpenGLPFADoubleBuffer );
		
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
		if( gl3 )
		{
			ADD_ATTR2( NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core );
		}
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
		
		ADD_ATTR2( NSOpenGLPFAColorSize, (windowFormat._bitsR+windowFormat._bitsG+windowFormat._bitsB) );
		
		if( windowFormat._bitsA > 0)
		{
			ADD_ATTR2( NSOpenGLPFAAlphaSize, windowFormat._bitsA );
		}
		
		if( windowFormat._bitsZ > 0)
		{
			ADD_ATTR2( NSOpenGLPFADepthSize, windowFormat._bitsZ );
		}
		
		ADD_ATTR( 0 );
		
		_windowPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
		if( _windowPixelFormat == nil )
		{
			// critical error
		}
		
		_windowContext = [[NSOpenGLContext alloc] initWithFormat:_windowPixelFormat shareContext:nil];
		if( _windowContext == nil )
		{
            int y=0;y++;
			// critical error
		}
		
		[_windowRef makeKeyAndOrderFront:nil];
		[_windowContext setView:[_windowRef contentView]];
		[_windowContext makeCurrentContext];
		
		//ABX FIX THIS MOUSE STUFF
		//NSPoint point = [_windowRef mouseLocationOutsideOfEventStream];
		//_glfwInput.MousePosX = point.x;
		//_glfwInput.MousePosY = _glfwWin.height - point.y;
		
		
		// set title
		const char *title = "lights";
		[_windowRef setTitle:[NSString stringWithCString:title encoding:NSISOLatin1StringEncoding]];
		
		// enable vsync
		GLint sync = 1;
		[_windowContext setValues:&sync forParameter:NSOpenGLCPSwapInterval];

        // Start by clearing the front buffer to black (avoid ugly desktop remains in our OpenGL window)
        glClear( GL_COLOR_BUFFER_BIT );
        WindowTick();

		return( true );
	}
}

#endif