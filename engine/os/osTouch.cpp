#include "os.h"
#include "gs.h"
#include "fw.h"

namespace os
{
    static Touch _existingTouches[ TouchLimit ];
    
    static s32 _existingTouchCount;
    
	void TouchInit()
	{
        _existingTouchCount = 0;
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
        Touch processedTouches[ TouchLimit ];
        s32 processedTouchCount = 0;
        
        for( s32 i = 0; i < touchCount; i++ )
        {
            Touch& touch = touches[ i ];
            s32 existingTouchIndex = 0;
            
            for( ; existingTouchIndex < _existingTouchCount; existingTouchIndex++ )
            {
                Touch& existingTouch = _existingTouches[ existingTouchIndex ];

                if( existingTouch._phase == TouchPhaseEnded || existingTouch._phase == TouchPhaseCancelled )
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
            
            if( processedTouchCount < TouchLimit )
            {
                if( existingTouchIndex < _existingTouchCount ) // existing touch
                {
                    processedTouches[ processedTouchCount++ ] = _existingTouches[ existingTouchIndex ];
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
            _existingTouches[ processedTouchIndex ] = processedTouches[ processedTouchIndex ];
        }
        
        _existingTouchCount = processedTouchCount;
        
        os::FlowTouch( _existingTouches, _existingTouchCount );
    }    
}
