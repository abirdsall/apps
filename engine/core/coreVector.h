#ifndef CORE_VECTOR
#define CORE_VECTOR

#define V3UnitX v3(1.0f, 0.0f, 0.0f)
#define V3UnitY v3(0.0f, 1.0f, 0.0f)
#define V3UnitZ v3(0.0f, 0.0f, 1.0f)
#define V3Zero v3(0.0f, 0.0f, 0.0f)
#define V3One v3(1.0f, 1.0f, 1.0f)

namespace core
{
    struct v2
    {
        union
        {
            f32 v[2];
            struct{ f32 x, y; };
        };
        v2(void)												{}
        v2(const f32 a, const f32 b)							{x  = a;   y  = b;}
        void operator =  (const f32 a)							{x  = a;   y  = a;}
        void operator -= (const f32 a)							{x -= a;   y -= a;}
        void operator += (const f32 a)							{x += a;   y += a;}
        void operator *= (const f32 a)							{x *= a;   y *= a;}
        void operator /= (const f32 a)							{x /= a;   y /= a;}
        void operator =  (const v2& a)							{x  = a.x; y  = a.y;}
        void operator -= (const v2& a)							{x -= a.x; y -= a.y;}
        void operator += (const v2& a)							{x += a.x; y += a.y;}
        void operator *= (const v2& a)							{x *= a.x; y *= a.y;}
        void operator /= (const v2& a)							{x /= a.x; y /= a.y;}
        operator float *() const { return (float *) &x; }
    };
    
    struct v3
    {
        union
        {
            f32 v[3];
            struct{ f32 x, y, z; };
        };
        v3(void)												{}
        v3(const f32 a)											{x  = a;   y  = a;   z  = a;}
        v3(const f32 a, const f32 b, const f32 c)				{x  = a;   y  = b;   z  = c;}
        void operator =  (const f32 a)							{x  = a;   y  = a;   z  = a;}
        void operator -= (const f32 a)							{x -= a;   y -= a;   z -= a;}
        void operator += (const f32 a)							{x += a;   y += a;   z += a;}
        void operator *= (const f32 a)							{x *= a;   y *= a;   z *= a;}
        void operator /= (const f32 a)							{x /= a;   y /= a;   z /= a;}
        void operator =  (const v3& a)							{x  = a.x; y  = a.y; z  = a.z;}
        void operator -= (const v3& a)							{x -= a.x; y -= a.y; z -= a.z;}
        void operator += (const v3& a)							{x += a.x; y += a.y; z += a.z;}
        void operator *= (const v3& a)							{x *= a.x; y *= a.y; z *= a.z;}
        void operator /= (const v3& a)							{x /= a.x; y /= a.y; z /= a.z;}
        operator float *() const { return (float *) &x; }
    };
    
    struct v4
    {
        union
        {
            f32 v[4];
            struct{ f32 x, y, z, w; };
        };
        v4(void) {}
        v4(const v3& a)											{x = a.x;  y  = a.y; z  = a.z; w  = 1.0f;}
        v4(const v3& a, const f32 b)							{x = a.x;  y  = a.y; z  = a.z; w  = b;}
        v4(const f32 a, const f32 b, const f32 c, const f32 d)	{x = a;    y  = b;   z  = c;   w  = d;}
        v2 xy() const											{return v2(x, y);}
        v3 xyz() const											{return v3(x, y, z);}
        void operator =  (const f32 a)							{x  = a;   y  = a;   z  = a;   w  = a;}
        void operator -= (const f32 a)							{x -= a;   y -= a;   z -= a;   w -= a;}
        void operator += (const f32 a)							{x += a;   y += a;   z += a;   w += a;}
        void operator *= (const f32 a)							{x *= a;   y *= a;   z *= a;   w *= a;}
        void operator /= (const f32 a)							{x /= a;   y /= a;   z /= a;   w /= a;}
        void operator =  (const v4& a)							{x  = a.x; y  = a.y; z  = a.z; w  = a.w;}
        void operator -= (const v4& a)							{x -= a.x; y -= a.y; z -= a.z; w -= a.w;}
        void operator += (const v4& a)							{x += a.x; y += a.y; z += a.z; w += a.w;}
        void operator *= (const v4& a)							{x *= a.x; y *= a.y; z *= a.z; w *= a.w;}
        void operator /= (const v4& a)							{x /= a.x; y /= a.y; z /= a.z; w /= a.w;}
        operator float *() const { return (float *) &x; }
    };
        
    inline bool operator == (const f32 a, const v2& b)			{return (a   == b.x  &&  a   == b.y									);}
    inline bool operator == (const f32 a, const v3& b)			{return (a   == b.x  &&  a   == b.y  &&	a   == b.z					);}
    inline bool operator == (const f32 a, const v4& b)			{return (a   == b.x  &&  a   == b.y  &&	a   == b.w  &&  a   == b.w	);}
    inline bool operator == (const v2& a, const v2& b)			{return (a.x == b.x	 &&  a.y == b.y									);}
    inline bool operator == (const v3& a, const v3& b)			{return (a.x == b.x	 &&  a.y == b.y  &&	a.z == b.z					);}
    inline bool operator == (const v4& a, const v4& b)			{return (a.x == b.x  &&  a.y == b.y  &&	a.z == b.w  &&  a.w == b.w	);}
    inline bool operator == (const v2& a, const f32 b)			{return (a.x == b    &&  a.y == b									);}
    inline bool operator == (const v3& a, const f32 b)			{return (a.x == b    &&  a.y == b    &&	a.z == b					);}
    inline bool operator == (const v4& a, const f32 b)			{return (a.x == b    &&  a.y == b    &&	a.z == b    &&  a.w == b	);}
    inline bool operator != (const f32 a, const v2& b)			{return (a   != b.x  ||  a   != b.y									);}
    inline bool operator != (const f32 a, const v3& b)			{return (a   != b.x  ||  a   != b.y  ||	a   != b.z					);}
    inline bool operator != (const f32 a, const v4& b)			{return (a   != b.x  ||  a   != b.y  ||	a   != b.w  ||  a   != b.w	);}
    inline bool operator != (const v2& a, const v2& b)			{return (a.x != b.x  ||  a.y != b.y									);}
    inline bool operator != (const v3& a, const v3& b)			{return (a.x != b.x  ||  a.y != b.y  ||	a.z != b.z					);}
    inline bool operator != (const v4& a, const v4& b)			{return (a.x != b.x  ||  a.y != b.y  ||	a.z != b.w  ||  a.w != b.w	);}
    inline bool operator != (const v2& a, const f32 b)			{return (a.x != b    ||  a.y != b									);}
    inline bool operator != (const v3& a, const f32 b)			{return (a.x != b    ||  a.y != b    ||	a.z != b					);}
    inline bool operator != (const v4& a, const f32 b)			{return (a.x != b    ||  a.y != b    ||	a.z != b    ||  a.w != b	);}
    inline v2	operator  - (const v2& a)						{return v2(-a.x,		-a.y								);}
    inline v3	operator  - (const v3& a)						{return v3(-a.x,		-a.y,		-a.z					);}
    inline v4	operator  - (const v4& a)						{return v4(-a.x,		-a.y,		-a.z,      -a.w			);}
    inline v2	operator  - (const f32 a, const v2& b)			{return v2(a - b.x,		a - b.y								);}
    inline v3	operator  - (const f32 a, const v3& b)			{return v3(a - b.x,		a - b.y,	a - b.z					);}
    inline v4	operator  - (const f32 a, const v4& b)			{return v4(a - b.x,		a - b.y,	a - b.z,	a - b.w		);}
    inline v2	operator  - (const v2& a, const v2& b)			{return v2(a.x - b.x,	a.y - b.y							);}
    inline v3	operator  - (const v3& a, const v3& b)			{return v3(a.x - b.x,	a.y - b.y,	a.z - b.z				);}
    inline v4	operator  - (const v4& a, const v4& b)			{return v4(a.x - b.x,	a.y - b.y,	a.z - b.z,	a.w - b.w	);}
    inline v2	operator  - (const v2& a, const f32 b)			{return v2(a.x - b,		a.y - b								);}
    inline v3	operator  - (const v3& a, const f32 b)			{return v3(a.x - b,		a.y - b,	a.z - b					);}
    inline v4	operator  - (const v4& a, const f32 b)			{return v4(a.x - b,		a.y - b,	a.z - b,	a.w - b		);}
    inline v2	operator  + (const f32 a, const v2& b)			{return v2(a + b.x,		a + b.y								);}
    inline v3	operator  + (const f32 a, const v3& b)			{return v3(a + b.x,		a + b.y,	a + b.z					);}
    inline v4	operator  + (const f32 a, const v4& b)			{return v4(a + b.x,		a + b.y,	a + b.z,	a + b.w		);}
    inline v2	operator  + (const v2& a, const v2& b)			{return v2(a.x + b.x,	a.y + b.y							);}
    inline v3	operator  + (const v3& a, const v3& b)			{return v3(a.x + b.x,	a.y + b.y,	a.z + b.z				);}
    inline v4	operator  + (const v4& a, const v4& b)			{return v4(a.x + b.x,	a.y + b.y,	a.z + b.z,	a.w + b.w	);}
    inline v2	operator  + (const v2& a, const f32 b)			{return v2(a.x + b,		a.y + b								);}
    inline v3	operator  + (const v3& a, const f32 b)			{return v3(a.x + b,		a.y + b,	a.z + b					);}
    inline v4	operator  + (const v4& a, const f32 b)			{return v4(a.x + b,		a.y + b,	a.z + b,	a.w + b		);}
    inline v2	operator  * (const f32 a, const v2& b)			{return v2(a * b.x,		a * b.y								);}
    inline v3	operator  * (const f32 a, const v3& b)			{return v3(a * b.x,		a * b.y,	a * b.z					);}
    inline v4	operator  * (const f32 a, const v4& b)			{return v4(a * b.x,		a * b.y,	a * b.z,	a * b.w		);}
    inline v2	operator  * (const v2& a, const v2& b)			{return v2(a.x * b.x,	a.y * b.y							);}
    inline v3	operator  * (const v3& a, const v3& b)			{return v3(a.x * b.x,	a.y * b.y,	a.z * b.z				);}
    inline v4	operator  * (const v4& a, const v4& b)			{return v4(a.x * b.x,	a.y * b.y,	a.z * b.z,	a.w * b.w	);}
    inline v2	operator  * (const v2& a, const f32 b)			{return v2(a.x * b,		a.y * b								);}
    inline v3	operator  * (const v3& a, const f32 b)			{return v3(a.x * b,		a.y * b,	a.z * b					);}
    inline v4	operator  * (const v4& a, const f32 b)			{return v4(a.x * b,		a.y * b,	a.z * b,	a.w * b		);}
    inline v2	operator  / (const f32 a, const v2& b)			{return v2(a / b.x,		a / b.y								);}
    inline v3	operator  / (const f32 a, const v3& b)			{return v3(a / b.x,		a / b.y,	a / b.z					);}
    inline v4	operator  / (const f32 a, const v4& b)			{return v4(a / b.x,		a / b.y,	a / b.z,	a / b.w		);}
    inline v2	operator  / (const v2& a, const v2& b)			{return v2(a.x / b.x,	a.y / b.y							);}
    inline v3	operator  / (const v3& a, const v3& b)			{return v3(a.x / b.x,	a.y / b.y,	a.z / b.z				);}
    inline v4	operator  / (const v4& a, const v4& b)			{return v4(a.x / b.x,	a.y / b.y,	a.z / b.z,	a.w / b.w	);}
    inline v2	operator  / (const v2& a, const f32 b)			{f32 c = 1.0f / b; return v2(a.x * c, a.y * c);}
    inline v3	operator  / (const v3& a, const f32 b)			{f32 c = 1.0f / b; return v3(a.x * c, a.y * c, a.z * c);}
    inline v4	operator  / (const v4& a, const f32 b)			{f32 c = 1.0f / b; return v4(a.x * c, a.y * c, a.z * c, a.w * c);}
    
    inline f32	dot(const v2& a, const v2& b)					{return a.x * b.x + a.y * b.y;}
    inline f32	dot(const v3& a, const v3& b)					{return a.x * b.x + a.y * b.y + a.z * b.z;}
    inline f32	dot(const v4& a, const v4& b)					{return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;}
    inline f32	length(const v2& a)								{return core::sqrt(dot(a, a));}
    inline f32	length(const v3& a)								{return core::sqrt(dot(a, a));}
    inline f32	length(const v4& a)								{return core::sqrt(dot(a, a));}
    inline f32	distance(const v2& a, const v2& b)				{return length(b - a);}
    inline f32	distance(const v3& a, const v3& b)				{return length(b - a);}
    inline f32	distance(const v4& a, const v4& b)				{return length(b - a);}
    inline f32	distanceSquared(const v2& a, const v2& b)		{v2 c = b - a; return dot(c, c);}
    inline f32	distanceSquared(const v3& a, const v3& b)		{v3 c = b - a; return dot(c, c);}
    inline f32	distanceSquared(const v4& a, const v4& b)		{v4 c = b - a; return dot(c, c);}
    inline v3	cross(const v3& a, const v3& b)					{return v3(a.y * b.z - b.y * a.z, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);}
    inline v2	abs(const v2& a)								{return v2(core::abs(a.x), core::abs(a.y));}
    inline v3	abs(const v3& a)								{return v3(core::abs(a.x), core::abs(a.y), core::abs(a.z));}
    inline v4	abs(const v4& a)								{return v4(core::abs(a.x), core::abs(a.y), core::abs(a.z), core::abs(a.w));}
    inline v2	min(const v2& a, const v2& b)					{return v2(core::min(a.x, b.x), core::min(a.y, b.y));}
    inline v3	min(const v3& a, const v3& b)					{return v3(core::min(a.x, b.x), core::min(a.y, b.y), core::min(a.z, b.z));}
    inline v4	min(const v4& a, const v4& b)					{return v4(core::min(a.x, b.x), core::min(a.y, b.y), core::min(a.z, b.z), core::min(a.w, b.w));}
    inline v2	max(const v2& a, const v2& b)					{return v2(core::max(a.x, b.x), core::max(a.y, b.y));}
    inline v3	max(const v3& a, const v3& b)					{return v3(core::max(a.x, b.x), core::max(a.y, b.y), core::max(a.z, b.z));}
    inline v4	max(const v4& a, const v4& b)					{return v4(core::max(a.x, b.x), core::max(a.y, b.y), core::max(a.z, b.z), core::max(a.w, b.w));}
    inline v2	clamp(const v2& a, const v2& b, const v2& c)	{return v2(core::clamp(a.x, b.x, c.x), core::clamp(a.y, b.y, c.y));}
    inline v3	clamp(const v3& a, const v3& b, const v3& c)	{return v3(core::clamp(a.x, b.x, c.x), core::clamp(a.y, b.y, c.y), core::clamp(a.z, b.z, c.z));}
    inline v2	lerp(const v2& a, const v2& b, const f32 c)		{return a + (b - a) * c;}
    inline v3	lerp(const v3& a, const v3& b, const f32 c)		{return a + (b - a) * c;}
    inline v4	lerp(const v4& a, const v4& b, const f32 c)		{return a + (b - a) * c;}
    inline v2	normaliseFast(const v2& a)						{return a * core::rsqrt(dot(a, a));}
    inline v3	normaliseFast(const v3& a)						{return a * core::rsqrt(dot(a, a));}
    inline v4	normaliseFast(const v4& a)						{return a * core::rsqrt(dot(a, a));}
    inline v2	normalise(const v2& a)							{f32 l = length(a); return l > 0.0f ? a / l : v2(0.0f, 0.0f);}
    inline v3	normalise(const v3& a)							{f32 l = length(a); return l > 0.0f ? a / l : v3(0.0f, 0.0f, 0.0f);}
    inline v4	normalise(const v4& a)							{f32 l = length(a); return l > 0.0f ? a / l : v4(0.0f, 0.0f, 0.0f, 0.0f);}
    inline v3	normalise(const v3& a, f32& b)					{b = length(a); return b > 0.0f ? a / b : v3(0.0f, 0.0f, 0.0f);}
    inline v3	genNormal(const v3& a, const v3& b, const v3& c){return normalise(cross(normalise(b - a), normalise(c - a)));}
    
    inline v3 rotate(const v3& a, const v3& b, const f32 c)
    {
        f32	sa = core::sin(c * 0.5f);
        f32	ca = core::cos(c * 0.5f);
        v4	q = v4(b.x * sa, b.y * sa, b.z * sa, ca);
        f32 x = a.x * q.w - a.y * q.z + a.z * q.y;
        f32 y = a.y * q.w - a.z * q.x + a.x * q.z;
        f32 z = a.z * q.w - a.x * q.y + a.y * q.x;
        f32 w = a.x * q.x + a.y * q.y + a.z * q.z;
        return v3(q.w * x + q.x * w + q.y * z - q.z * y, q.w * y + q.y * w + q.z * x - q.x * z, q.w * z + q.z * w + q.x * y - q.y * x);
    }
    
    inline u8 minAxis(const v3& a)
    {
        return (core::abs(a.x) < core::abs(a.y) ? (core::abs(a.x) < core::abs(a.z) ? 0 : 2) : (core::abs(a.y) < core::abs(a.z) ? 1 : 2));
    }
    
    inline u8 maxAxis(const v3& a)
    {
        return (core::abs(a.x) > core::abs(a.y) ? (core::abs(a.x) > core::abs(a.z) ? 0 : 2) : (core::abs(a.y) > core::abs(a.z) ? 1 : 2));
    }
    
    inline v3 perp(const v3& a)
    {
        switch(minAxis(a))
        {
            case 0:		return v3( 0.0f,  a.z, -a.y);
            case 1:		return v3( -a.z, 0.0f,  a.x);
            default:	return v3(  a.y, -a.x, 0.0f);
        }
    }
    
    inline v3 rndDir(void)
    {
        f32 p = core::randomfraction() * PiMul2;
        f32 z = core::randomrange(-1.0f, 1.0f);
        f32 r = core::sqrt(1.0f - z * z);
        return v3(core::sin(p) * r, core::cos(p) * r, z);
    }
    
    inline v3 rndDir(const v3& vz, const f32 minRotx, const f32 maxRotx, const f32 minRotz, const f32 maxRotz)
    {
        f32 a  = core::randomrange(minRotz, maxRotz) * PiMul2;
        f32 z  = core::randomrange(minRotx, maxRotx);
        f32 r  = core::sqrt(1.0f - z * z);
        f32	x  = core::sin(a) * r;
        f32	y  = core::cos(a) * r;
        v3	vx = normalise(perp(vz));
        v3	vy = cross(vx, vz);
        return x * vx + y * vy + z * vz;
    }
    
    inline u32 compress(const v4& a)
    {
        return u8(255.0f * a.x) | u8(255.0f * a.y) << 8 | u8(255.0f * a.z) << 16 | u8(255.0f * a.w) << 24;
    }
    
    inline v4 decompress(const u32 a)
    {
        f32 r = 1.0f / 255.0f;
        return v4(f32(a & 0xff) * r, f32((a & 0xff00) >> 8) * r, f32((a & 0xff0000) >> 16) * r, f32((a & 0xff000000) >> 24) * r);
    }

}

#endif