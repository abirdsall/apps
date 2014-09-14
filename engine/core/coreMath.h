#ifndef CORE_MATH
#define CORE_MATH

namespace core
{
	f32 sinf(f32 a);
	f32 cosf(f32 a);
	f32 tanf(f32 a);
	f32 acosf(f32 a);
	f32 atanf(f32 a);
	f32 sqrtf(f32 a);
	f32 powf(f32 x, f32 y);
	f64 pow(f64 x, f64 y);
	u32 random();
	f32 randomfraction();
	f32 randomrange(const f32 a, const f32 b);
	void randomseed(const u32 a);
	
	inline s32 abs(s32 a)
	{
		return a < 0.0f ? -a : a;
	}
	
	inline f32 abs(f32 a)
	{
		return a < 0.0f ? -a : a;
	}
	
	inline s32 min(s32 a, s32 b)
	{
		return a < b ? a : b;
	}
	
	inline f32 min(f32 a, f32 b)
	{
		return a < b ? a : b;
	}
	
	inline s32 max(s32 a, s32 b)
	{
		return a > b ? a : b;
	}
	
	inline f32 max(f32 a, f32 b)
	{
		return a > b ? a : b;
	}
	
	inline f32 clamp(f32 a, f32 b, f32 c)
	{
		return a < b ? b : a > c ? c : a;
	}
	
	inline f32 lerp(f32 a, f32 b, f32 c)
	{
		return a + (b - a) * c;
	}
	
	inline f32 rsqrt(f32 a)
	{
		f32 b = a * 0.5f;
		s32 c = *(s32*)&a;
		c = 0x5f3759df - (c >> 1);
		a = *(f32*)&c;
		return a * (1.5f - a * a * b);
	}
}

#endif