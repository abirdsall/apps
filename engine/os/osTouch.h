#ifndef OS_TOUCH_H
#define OS_TOUCH_H

namespace os
{
    static const s32 TouchLimit = 32;

    enum TouchPhase
	{
        TouchPhaseBegan,
        TouchPhaseMoved,
        TouchPhaseStationary,
        TouchPhaseEnded,
        TouchPhaseCancelled,
		TouchPhaseCount
	};
    
    struct Touch
    {
        TouchPhase _phase;
        f32 _x;
        f32 _y;
        f32 _xPrevious;
        f32 _yPrevious;
        s32 _tapCount;
        s32 _timeStamp;
        hwInt _nativeTouch;
        
        void Init( TouchPhase phase, f32 x, f32 y, f32 xPrevious, f32 yPrevious, s32 tapCount, s32 timeStamp, hwInt nativeTouch )
        {
            _phase = phase;
            _x = x;
            _y = y;
            _xPrevious = xPrevious;
            _yPrevious = yPrevious;
            _tapCount = tapCount;
            _timeStamp = timeStamp;
            _nativeTouch = nativeTouch;
        }
        
        Touch()
        {
            Init( TouchPhaseCount, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0 );
        }
        
        Touch( TouchPhase phase, f32 x, f32 y, f32 xPrevious, f32 yPrevious, s32 tapCount, s32 timeStamp, hwInt nativeTouch )
        {
            Init( phase, x, y, xPrevious, yPrevious, tapCount, timeStamp, nativeTouch );
        }
    };
    
	void TouchInit();
	void TouchTick( f32 dt );
	void TouchDraw();
	void TouchKill();
    
    void TouchProcess( Touch touches[], s32 touchCount );
}

#endif