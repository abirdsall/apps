#include "os.h"
#include "gs.h"
#include "fw.h"

namespace os
{
    static Touch sExistingTouches[ kTouchLimit ];
    
    static s32 sExistingTouchCount;
    
	void TouchInit()
	{
        sExistingTouchCount = 0;
	}
    
    void TouchTick( f32 dt )
    {
    }
    
    void TouchDraw()
    {
    }
    
    void TouchKill()
    {
    }
    
	void TouchProcess( Touch touches[], s32 touchCount )
	{
        Touch processedTouches[ kTouchLimit ];
        s32 processedTouchCount = 0;
        
        for( s32 i = 0; i < touchCount; i++ )
        {
            Touch& touch = touches[ i ];
            s32 existingTouchIndex = 0;
            
            for( ; existingTouchIndex < sExistingTouchCount; existingTouchIndex++ )
            {
                Touch& existingTouch = sExistingTouches[ existingTouchIndex ];

                if( existingTouch._phase == eTouchPhaseEnded || existingTouch._phase == eTouchPhaseCancelled )
                {
                    continue;
                }
                
                if( existingTouch._nativeTouch == touch._nativeTouch )
                {
                    existingTouch._timeStamp = touch._timeStamp;
                    existingTouch._xPrevious = touch._xPrevious;
                    existingTouch._yPrevious = touch._yPrevious;
                    existingTouch._x = touch._x;
                    existingTouch._y = touch._y;
                    existingTouch._phase = touch._phase;
                    existingTouch._tapCount = touch._tapCount;
                    break;
                }
            }
            
            if( processedTouchCount < kTouchLimit )
            {
                if( existingTouchIndex < sExistingTouchCount ) // existing touch
                {
                    processedTouches[ processedTouchCount++ ] = sExistingTouches[ existingTouchIndex ];
                }
                else // new touch
                {
                    processedTouches[ processedTouchCount++ ] = touch;
                }
            }
            else
            {
                ASSERT(false);
            }
        }
        
        for( s32 processedTouchIndex = 0; processedTouchIndex < processedTouchCount; processedTouchIndex++ )
        {
            sExistingTouches[ processedTouchIndex ] = processedTouches[ processedTouchIndex ];
        }
        
        sExistingTouchCount = processedTouchCount;
        
        os::FlowTouch( sExistingTouches, sExistingTouchCount );
    }    
}
