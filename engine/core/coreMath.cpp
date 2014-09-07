#include "core.h"

#include <math.h>

#define SinTableSize 0x800
#define SinTableMask 0x7ff

namespace core
{
	static u64 _randomSeed = 0x0000000021454a53;
	static f32 _sinTable[ SinTableSize ];
	
	void MathInit()
	{
		s32 i = SinTableSize;
        
		while(--i >= 0)
		{
			_sinTable[i] = sinf(PiMul2 * (((f32)i) / (f32)SinTableSize));
		}
	}
	
	void MathKill()
	{
	}
	
	f32 sin(f32 a)
	{
		return sinf(a);
	}

	f32 cos(f32 a)
	{
		return cosf(a);
	}

	f32 SinTable(f32 a)
	{
		return _sinTable[((s32)(((((f32)SinTableSize) / PiMul2) * a) + 0.5f)) & SinTableMask];
	}
	
	f32 costable(f32 a)
	{
		return _sinTable[(((s32)(((((f32)SinTableSize) / PiMul2) * a) + 0.5f)) + (SinTableSize / 4)) & SinTableMask];
	}
	
	f32 tan(f32 a)
	{
		return tanf(a);
	}
	
	f32 acos(f32 a)
	{
		return acosf(a);
	}
	
	f32 atan(f32 a)
	{
		return atanf(a);
	}
	
	f32 sqrt(f32 a)
	{
		return sqrtf(a);
	}

	f32 pow(f32 x, f32 y)
	{
		return powf(x, y);
	}

	f64 pow64(f64 x, f64 y)
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