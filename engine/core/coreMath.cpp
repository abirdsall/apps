#include "core.h"

#include <math.h>

namespace core
{
	static u64 _randomSeed = 0x0000000021454a53;
	
	f32 sinf(f32 a)
	{
		return ::sinf(a);
	}

	f32 cosf(f32 a)
	{
		return ::cosf(a);
	}
	
	f32 tanf(f32 a)
	{
		return ::tanf(a);
	}
	
	f32 acosf(f32 a)
	{
		return ::acosf(a);
	}
	
	f32 atanf(f32 a)
	{
		return ::atanf(a);
	}
	
	f32 sqrtf(f32 a)
	{
		return ::sqrtf(a);
	}

	f32 powf(f32 x, f32 y)
	{
        return ::powf(x, y);
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
	
	f32 randomrange(const f32 a, const f32 b)
	{
		return a + randomfraction() * (b - a);
	}
	
	void randomseed(const u32 a)
	{
		_randomSeed = a;
	}
}