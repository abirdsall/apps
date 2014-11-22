#include "core.h"

namespace core
{
	static u64 _milliseconds;
	
	static u64 TimeMilliseconds()
	{
		struct timeval tv;
		gettimeofday( &tv, Null );
		u64 seconds = tv.tv_sec;
		u64 milliseconds = tv.tv_usec / 1000; // usec is microseconds
		return seconds * 1000 + milliseconds;
	}
	
	void TimeInit()
	{
		_milliseconds = TimeMilliseconds();
	}
	
    void TimeKill()
    {
        
    }
    
	f32 TimeTick()
	{
		u64 oldMilliseconds = _milliseconds;
		_milliseconds = TimeMilliseconds();
		return 0.001f * ( f32 )( _milliseconds - oldMilliseconds );
	}
}