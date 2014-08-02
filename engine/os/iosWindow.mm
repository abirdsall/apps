#include "os.h"
#include "gs.h"

#if kBuildIos

namespace os
{
	extern void OnMouseButton( const eMouseButton button, const bool pressed );
	extern void OnMouseMove( s32 x, s32 y, s32 dx, s32 dy );
	
	extern void OnWindowResize( s32 sizeX, s32 sizeY );
	extern bool OnWindowActivated( const bool activated );
	extern void OnWindowIconified( const bool iconified );
	extern void OnWindowClose();
}

using namespace gs;

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface ViewController : GLKViewController
{
    EAGLContext* _resourceContext;
    dispatch_queue_t _resourceQueue;
    
    double _lastTouchTimestamp;
    BOOL _root;
}

@property (nonatomic, readonly) GLKView* glkView;
@property (nonatomic, readonly) EAGLContext* context;
@property (nonatomic, readonly) f32 pixelSizeX;
@property (nonatomic, readonly) f32 pixelSizeY;

@end

@implementation ViewController

-(instancetype)init
{
    if((self = [super initWithNibName:nil bundle:nil]))
    {
        [self setup];
    }
    return self;
}

-(void)dealloc
{
    [EAGLContext setCurrentContext:nil];
    
    if(_resourceQueue)
    {
        dispatch_release(_resourceQueue);
    }
    
    [_context release];
    [_resourceContext release];
    [super dealloc];
}

-(void)setup
{
    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if(!_context || ![EAGLContext setCurrentContext:_context])
    {
        NSLog(@"Could not create render context");
    }
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    [self glkView].context = _context;
    
    os::FlowInitWindow();
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)executeInResourceQueue:(dispatch_block_t)block
{
    if(!_resourceContext)
    {
        _resourceContext = [[EAGLContext alloc] initWithAPI:_context.API sharegroup:_context.sharegroup];
    }
    
    if(!_resourceQueue)
    {
        _resourceQueue = dispatch_queue_create("Sparrow-ResourceQueue", NULL);
    }
    
    dispatch_async(_resourceQueue, ^
                   {
                       [EAGLContext setCurrentContext:_resourceContext];
                       block();
                   });
}

#pragma mark - GLKView and GLKViewController delegate methods

-(void)update
{
    if( os::FlowActive() )
    {
        os::FlowTick();
    }
}

-(void)glkView:(GLKView*)view drawInRect:(CGRect)rect
{
    if( os::FlowActive() )
    {
        if( !_root )
        {
            _root = true;
            // ideally, we'd do this in 'viewDidLoad', but when iOS starts up in landscape mode,
            // the view width and height are swapped. In this method, however, they are correct.
            
            CGSize pointSize = self.view.bounds.size;
            f32 pointScale = [[UIScreen mainScreen] scale];
            _pixelSizeX = pointScale * pointSize.width;
            _pixelSizeY = pointScale * pointSize.height;
            os::OnWindowResize( _pixelSizeX, _pixelSizeY );
        }
        
        [EAGLContext setCurrentContext:_context];
        
        os::FlowDraw();
    }
}

#pragma mark - Touch Processing

-(void)setMultitouchEnabled:(BOOL)multitouchEnabled
{
    self.view.multipleTouchEnabled = multitouchEnabled;
}

-(BOOL)multitouchEnabled
{
    return self.view.multipleTouchEnabled;
}

-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self processTouchEvent:event];
}

-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self processTouchEvent:event];
}

-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self processTouchEvent:event];
}

-(void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    _lastTouchTimestamp -= 0.0001f; // cancelled touch events have an old timestamp -> workaround
    [self processTouchEvent:event];
}

-(void)processTouchEvent:(UIEvent*)event
{
    if( !self.paused && _lastTouchTimestamp != event.timestamp )
    {
        @autoreleasepool
        {
            os::Touch touches[ os::kTouchLimit ];
            
            s32 touchCount = 0;
            
            f32 pointScale = [[UIScreen mainScreen] scale];
            
            double now = CACurrentMediaTime();
            
            for( UITouch* uiTouch in [event touchesForView:self.view] )
            {
                CGPoint location = [uiTouch locationInView:self.view];
                CGPoint previousLocation = [uiTouch previousLocationInView:self.view];
                
                os::Touch touch = os::Touch( ( os::eTouchPhase )uiTouch.phase,
                                            location.x * pointScale,
                                            location.y * pointScale,
                                            previousLocation.x * pointScale,
                                            previousLocation.y * pointScale,
                                            (s32)uiTouch.tapCount,
                                            now, // timestamp of uiTouch not compatible to Sparrow timestamp
                                            (hwInt)uiTouch );
                
                if( touchCount < os::kTouchLimit )
                {
                    touches[ touchCount++ ] = touch;
                }
                else
                {
                    assert( false );
                }
            }
            
            os::TouchProcess( touches, touchCount );
            
            _lastTouchTimestamp = event.timestamp;
        }
    }
}

#pragma mark - Auto Rotation

// The following methods implement what I would expect to be the default behaviour of iOS:
// The orientations that you activated in the application plist file are automatically rotated to.

-(NSUInteger)supportedInterfaceOrientations
{
    NSArray* supportedOrientations =
    [[NSBundle mainBundle] infoDictionary][@"UISupportedInterfaceOrientations"];
    
    NSUInteger returnOrientations = 0;
    if ([supportedOrientations containsObject:@"UIInterfaceOrientationPortrait"])
        returnOrientations |= UIInterfaceOrientationMaskPortrait;
    if ([supportedOrientations containsObject:@"UIInterfaceOrientationLandscapeLeft"])
        returnOrientations |= UIInterfaceOrientationMaskLandscapeLeft;
    if ([supportedOrientations containsObject:@"UIInterfaceOrientationPortraitUpsideDown"])
        returnOrientations |= UIInterfaceOrientationMaskPortraitUpsideDown;
    if ([supportedOrientations containsObject:@"UIInterfaceOrientationLandscapeRight"])
        returnOrientations |= UIInterfaceOrientationMaskLandscapeRight;
    
    return returnOrientations;
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    NSArray* supportedOrientations =
    [[NSBundle mainBundle] infoDictionary][@"UISupportedInterfaceOrientations"];
    
    return ((interfaceOrientation == UIInterfaceOrientationPortrait &&
             [supportedOrientations containsObject:@"UIInterfaceOrientationPortrait"]) ||
            (interfaceOrientation == UIInterfaceOrientationLandscapeLeft &&
             [supportedOrientations containsObject:@"UIInterfaceOrientationLandscapeLeft"]) ||
            (interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown &&
             [supportedOrientations containsObject:@"UIInterfaceOrientationPortraitUpsideDown"]) ||
            (interfaceOrientation == UIInterfaceOrientationLandscapeRight &&
             [supportedOrientations containsObject:@"UIInterfaceOrientationLandscapeRight"]));
}

-(void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                                        duration:(NSTimeInterval)duration
{
    // inform all display objects about the new game size
    BOOL isPortrait = UIInterfaceOrientationIsPortrait(interfaceOrientation);
    
    float newSizeX  = isPortrait ? MIN(_pixelSizeX, _pixelSizeY) : MAX(_pixelSizeX, _pixelSizeY);
    float newSizeY = isPortrait ? MAX(_pixelSizeX, _pixelSizeY) : MIN(_pixelSizeX, _pixelSizeY);
    
    if(newSizeX != _pixelSizeX)
    {
        _pixelSizeX = newSizeX;
        _pixelSizeY = newSizeY;
        
        os::OnWindowResize( _pixelSizeX, _pixelSizeY );
    }
}

#pragma mark - Properties

-(GLKView*)glkView
{
    return (GLKView*)self.view;
}

@end

static UIWindow* sWindow;
static ViewController* sViewController;

namespace os
{
	void WindowInitHw()
	{
        sWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        sViewController = [[ViewController alloc] init];
        sWindow.rootViewController = sViewController;
        [sWindow makeKeyAndVisible];
	}
	
	void WindowKillHw()
	{
        [sWindow release];
        [sViewController release];
	}
	
	void WindowTickHw()
	{
	}
	
	bool WindowOpenHw( WindowFormat& windowFormat )
	{
		return true;
	}
}

#endif