#include "os.h"
#include <sys/sysctl.h> // required for gettimeofday

namespace os
{
	static u64 sMilliseconds;
	
	static u64 TimeMilliseconds()
	{
		struct timeval tv;
		gettimeofday( &tv, kNull );
		u64 seconds = tv.tv_sec;
		u64 milliseconds = tv.tv_usec / 1000; // usec is microseconds
		return seconds * 1000 + milliseconds;
	}
	
	void TimeInit()
	{
		sMilliseconds = TimeMilliseconds();
	}
	
	f32 TimeTick()
	{
		u64 oldMilliseconds = sMilliseconds;
		sMilliseconds = TimeMilliseconds();
		return 0.001f * ( f32 )( sMilliseconds - oldMilliseconds );
	}
}