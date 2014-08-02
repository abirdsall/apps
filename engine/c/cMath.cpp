#include "os.h"

#include <math.h>

#define kSinTableSize 0x800
#define kSinTableMask 0x7ff

namespace math
{
	static u64 sRandomSeed = 0x0000000021454a53;
	static f32 sSinTable[kSinTableSize];
	
	bool init(void)
	{
		s32 i = kSinTableSize;
		while(--i >= 0)
		{
			sSinTable[i] = sinf(kPiMul2 * (((f32)i) / (f32)kSinTableSize));
		}
		return true;
	}
	
	bool kill(void)
	{
		return true;
	}
	
	f32 sin(f32 a)
	{
		return sinf(a);
	}

	f32 cos(f32 a)
	{
		return cosf(a);
	}

	f32 sintable(f32 a)
	{
		return sSinTable[((s32)(((((f32)kSinTableSize) / kPiMul2) * a) + 0.5f)) & kSinTableMask];
	}
	
	f32 costable(f32 a)
	{
		return sSinTable[(((s32)(((((f32)kSinTableSize) / kPiMul2) * a) + 0.5f)) + (kSinTableSize / 4)) & kSinTableMask];
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
		return (u32)(((sRandomSeed = ((u64)214013) * sRandomSeed + ((u64)2531011))));
	}
	
	f32 randomfraction()
	{
		return (f32)random() / (f32)kMaxU32;
	}
	
	f32 randomrange(const f32 a, const f32 b)
	{
		return a + randomfraction() * (b - a);
	}
	
	void randomseed(const u32 a)
	{
		sRandomSeed = a;
	}
}