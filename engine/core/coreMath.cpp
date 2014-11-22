#include "core.h"

#include <math.h>

namespace core
{
	static u64 _randomSeed = 0x0000000021454a53;
	
	f32 sin(f32 a)
	{
		return ::sin(a);
	}

	f32 cos(f32 a)
	{
		return ::cos(a);
	}
	
	f32 tan(f32 a)
	{
		return ::tan(a);
	}
	
	f32 acos(f32 a)
	{
		return ::acos(a);
	}
	
	f32 atan(f32 a)
	{
		return ::atan(a);
	}
	
	f32 sqrt(f32 a)
	{
		return ::sqrt(a);
	}

	f32 pow(f32 x, f32 y)
	{
        return ::pow(x, y);
	}

	f64 pow(f64 x, f64 y)
	{
		return ::pow(x, y);
	}
	
	u32 random()
	{
		return (u32)(((_randomSeed = ((u64)214013) * _randomSeed + ((u64)2531011))));
	}
	
	f32 randomfraction()
	{
		return (f32)random() / (f32)MaxU32;
	}
	
	f32 randomrange(f32 a, f32 b)
	{
		return a + randomfraction() * (b - a);
	}
	
	void randomseed(u32 a)
	{
		_randomSeed = a;
	}
}