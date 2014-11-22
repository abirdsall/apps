#include "aabb.h"
#include "frustum.h"
#include "geometry.h"
#include "intersect.h"

bool intersect(const v3& position, const frustum& frustum)
{
	s32 i;
	for(i = 0; i < 6; i++)
	{
		if(frustum._planes[i].distanceToPoint(position) < 0.0f)
			return false;
	}
	return true;
}

f32 intersect(const line& line, const v3& v)
{
	v3	p1		= line.getPos();
	v3	p2		= line.getEnd();
	f32	size	= ::distance(p1, p2);
	f32	u		= dot(v - p1, p2 - p1) / (size * size);
	v3  pos		= p1 + (p2 - p1) * core::clamp(u, 0.0f, 1.0f);
	return distance(v, pos);
}

f32 intersect(const line& line1, const line& line2)
{
	v3	p1		= line1.getPos();
	v3	p2		= line1.getEnd();
	v3	p3		= line2.getPos();
	v3	p4		= line2.getEnd();
	v3	p21		= p2 - p1;
	v3	p43		= p4 - p3;
	v3	p13		= p1 - p3;
	f32	d1343	= dot(p13, p43);
	f32	d4321	= dot(p43, p21);
	f32	d1321	= dot(p13, p21);
	f32	d4343	= dot(p43, p43);
	f32	d2121	= dot(p21, p21);
	f32	denom	= d2121 * d4343 - d4321 * d4321;
	if(core::abs(denom) > 1e-5f && core::abs(d4343) > 1e-5f)		// avoid divide by zero
	{
		f32	numer	= d1343 * d4321 - d1321 * d4343;
		f32 mua		= numer / denom;					// distance along line 1
		f32 mub		= (d1343 + d4321 * (mua)) / d4343;	// distance along line 2
		mua			= core::clamp(mua, 0.0f, 1.0f);
		mub			= core::clamp(mub, 0.0f, 1.0f);
		v3	rp1		= p1 + p21 * mua;
		v3	rp2		= p3 + p43 * mub;
		return distance(rp1, rp2);
	}
	return MaxF32;
}

bool intersect(const line& line, const plane& plane, f32& distance, bool infinite)
{
	f32 angle = dot(line.getDir(), plane._normal);
    if(angle != 0.0f)
    {
        distance = dot(plane._normal, v3(plane._normal * plane._offset) -  line.getPos()) / angle;
		return (infinite || distance >= 0.0f ? true : false);
    }
	if(dot(line.getPos(), plane._normal) - plane._offset == 0.0f)
	{
		distance = 0.0f;
        return true;
	}
    return false;
}

bool intersect(const line& line, const plane& plane, v3& outPosition, bool infinite)
{
    f32 distance;
    if(intersect(line, plane, distance, infinite))
    {
        outPosition = line.getPos() + distance * line.getDir();
        return true;
    }
	return false;
}

bool intersect(const line& l, const frustum& frustum, f32& hitLen1, f32& hitLen2)
{
	const plane*	planes = frustum._planes;
	line			lineA;
	line			lineB;
	v3				pointA;
	v3				pointB;
	v3				hit1 = l.getPos();
	v3				hit2 = l.getPos() + l.getDir() * MaxF32;
	f32				angle;
	s32				i;

	intersect(planes[3], planes[4], lineA);
	intersect(planes[2], planes[5], lineB);
	intersect(lineA, planes[0], pointA, true);
	intersect(lineB, planes[1], pointB, true);

	for(i = 0; i < 6; i++)
	{
		bool map			= (i == 0 || i == 3 || i == 4);
		v3   point			= map ? pointA : pointB;
		v3   innerVector	= map ? pointB - pointA : pointA - pointB;
		v3   innerNormal	= dot(innerVector, planes[i]._normal) >= 0.0f ? planes[i]._normal : -planes[i]._normal;
		f32  val1			= dot(hit1 - point, innerNormal);
		f32  val2			= dot(hit2 - point, innerNormal);

		if(val1 < 0.0f && val2 < 0.0f) return false;
		if(val1 > 0.0f && val2 < 0.0f) intersect(l, planes[i], hit2, false);
		if(val1 < 0.0f && val2 > 0.0f) intersect(l, planes[i], hit1, false);
	}

	hitLen1 = ((angle = dot(hit1 - l.getPos(), l.getDir())) != 0.0f) ? dot(hit1 - l.getPos(), hit1 - l.getPos()) / angle : 0.0f;
	hitLen2 = ((angle = dot(hit2 - l.getPos(), l.getDir())) != 0.0f) ? dot(hit2 - l.getPos(), hit2 - l.getPos()) / angle : 0.0f;

	return true;
}

bool intersect(const line& line, const geometry::Triangle& triangle, v3* outPosition, v3* outNormal, f32 lineLength)
{
	v3	ea = triangle._vertices[1] - triangle._vertices[0];
	v3	eb = triangle._vertices[2] - triangle._vertices[0];
	v3	s;
	v3	p = cross(line.getDir(), eb);
	f32	u;
	f32 v;
	f32 t = dot(p, ea);

	if(t <= 0.0f)
		return false;

	t = 1.0f / t;

	s = line.getPos() - triangle._vertices[0];
	u = t * dot(s, p);
	if(u < 0.0f || u > 1.0f)
		return false;
	
	s = cross(s, ea);
	v = t * dot(line.getDir(), s);
	if(v < 0.0f || v > 1.0f ||	u + v > 1.0f)
		return false;

	t *= dot(eb, s);
	if(t < 0.0f || t > lineLength)
		return false;

	if(outPosition) *outPosition = line.getPos() + line.getDir() * t;
	if(outNormal) *outNormal = normalise(cross(ea, eb));
	return true;
}

bool intersect(const line& line, const aabb& aabb, f32 lineLength)
{
	f32 tf, tn, tm, t0, t1;

	if(line.getDir().x == 0.0f)
	{
		if(line.getPos().x < aabb._min.x || line.getPos().x > aabb._max.x)
			return false;
		tn = 0.0f;
		tf = lineLength;
	}
	else
	{
		tm = 1.0f / line.getDir().x;
		tn = tm * (aabb._min.x - line.getPos().x);
		tf = tm * (aabb._max.x - line.getPos().x);
		if(tn > tf)
		{
			tm = tn; tn = tf; tf = tm;
		}
		if(tf < 0.0f || tn > lineLength)
			return false;
	}	

	if(line.getDir().z == 0.0f)
	{
		if(line.getPos().z < aabb._min.z || line.getPos().z > aabb._max.z)
			return false;
	}
	else
	{
		tm = 1.0f / line.getDir().z;
		t0 = tm * (aabb._min.z - line.getPos().z);
		t1 = tm * (aabb._max.z - line.getPos().z);
		if(t0 > t1)
		{
			tm = t0; t0 = t1; t1 = tm;
		}
		if(t1 < tf)
		{
			if(t1 < 0.0f)
				return false;
			tf = t1;
		}
		if(t0 > tn)
		{
			if(t0 > lineLength)
				return false;
			tn = t0;
		}
		if(tn > tf)
			return false;
	}

	if(line.getDir().y == 0.0f)
	{
		if(line.getPos().y < aabb._min.y || line.getPos().y > aabb._max.y)
			return false;
	}
	else
	{
		tm = 1.0f / line.getDir().y;
		t0 = tm * (aabb._min.y - line.getPos().y);
		t1 = tm * (aabb._max.y - line.getPos().y);
		if(t0 > t1)
		{
			tm = t0; t0 = t1; t1 = tm;
		}
		if(t1 < tf)
		{
			if(t1 < 0.0f)
				return false;
			tf = t1;
		}
		if(t0 > tn)
		{
			if(t0 > lineLength)
				return false;
			tn = t0;
		}
		if(tn > tf)
			return false;
	}
	return true;
}

u32 intersectFace(const line& line, const aabb& aabb)
{
	v3 pos = line.getPos();
	v3 dir = line.getDir();
	f32 rx	= 1.0f / dir.x;
	f32 y0x	= pos.y + (aabb._min.x - pos.x) * dir.y * rx;
	f32 z0x	= pos.z + (aabb._min.x - pos.x) * dir.z * rx;
	if(pos.x < aabb._min.x && y0x > aabb._min.y && y0x < aabb._max.y && z0x > aabb._min.z && z0x < aabb._max.z) return 1;

	f32 y1x	= pos.y + (aabb._max.x - pos.x) * dir.y * rx;
	f32 z1x	= pos.z + (aabb._max.x - pos.x) * dir.z * rx;
	if(pos.x > aabb._max.x && y1x > aabb._min.y && y1x < aabb._max.y && z1x > aabb._min.z && z1x < aabb._max.z) return 2;

	f32 ry	= 1.0f / dir.y;
	f32 x0y	= pos.x + (aabb._min.y - pos.y) * dir.x * ry;
	f32 z0y	= pos.z + (aabb._min.y - pos.y) * dir.z * ry;
	if(pos.y < aabb._min.y && x0y > aabb._min.x && x0y < aabb._max.x && z0y > aabb._min.z && z0y < aabb._max.z) return 3;

	f32 x1y	= pos.x + (aabb._max.y - pos.y) * dir.x * ry;
	f32 z1y	= pos.z + (aabb._max.y - pos.y) * dir.z * ry;
	if(pos.y > aabb._max.y && x1y > aabb._min.x && x1y < aabb._max.x && z1y > aabb._min.z && z1y < aabb._max.z) return 4;

	f32 rz	= 1.0f / dir.z;
	f32 x0z	= pos.x + (aabb._min.z - pos.z) * dir.x * rz;
	f32 y0z	= pos.y + (aabb._min.z - pos.z) * dir.y * rz;
	if(pos.z < aabb._min.z && x0z > aabb._min.x && x0z < aabb._max.x && y0z > aabb._min.y && y0z < aabb._max.y) return 5;

	f32 x1z	= pos.x + (aabb._max.z - pos.z) * dir.x * rz;
	f32 y1z	= pos.y + (aabb._max.z - pos.z) * dir.y * rz;
	if(pos.z > aabb._max.z && x1z > aabb._min.x && x1z < aabb._max.x && y1z > aabb._min.y && y1z < aabb._max.y) return 6;

	return 0;
}

bool intersect(const plane& plane1, const plane& plane2, line& line)
{
	f32 o1  = plane1._offset;
	f32 o2  = plane2._offset;
	const v3& n1  = plane1._normal;
	const v3& n2  = plane2._normal;
	const v3  dir = cross(n1, n2);
	f32 len = length(dir); 
	if(len > 1e-5f)
	{
		switch(maxAxis(dir))
		{
			case 0: line.setPos(v3(0.f, (o1 * n2.z - o2 * n1.z) / dir.x, (o2 * n1.y - o1 * n2.y) / dir.x)); break;
			case 1: line.setPos(v3((o2 * n1.z - o1 * n2.z) / dir.y, 0.f, (o1 * n2.x - o2 * n1.x) / dir.y)); break;
			case 2: line.setPos(v3((o1 * n2.y - o2 * n1.y) / dir.z, (o2 * n1.x - n2.x * o1) / dir.z, 0.f)); break;
		}
		line.setDir(dir / len);
		return true;
	}
	return false;
}

bool intersectFast(const aabb& aabb, const frustum& frustum)
{
	u8 i;
	for(i = 0; i < 6; i++)
	{
		if(aabb.planeSide(frustum._planes[i]) == plane::eBack)
			return false;
	}
	return true;
}

bool intersect(const aabb& aabb, const frustum& frustum)
{
	u8 i, j;
	for(i = 0, j = 0; i < 6; i++)
	{
		plane::Side side = aabb.planeSide(frustum._planes[i]);
		if(side == plane::eBack ) return false; // no intersection
		if(side == plane::eFront && i > 1) j++;
	}
	
	if(j == 4)
		return true;

	for(i = 0; i < 4; i++)
		if(intersect(frustum._lines[i], aabb))
			return true; // frustum edge intersects aabb

	f32 hit1;
	f32 hit2;
	f32 dimX = aabb._max.x - aabb._min.x;
	f32 dimY = aabb._max.y - aabb._min.y;
	f32 dimZ = aabb._max.z - aabb._min.z;
	if(intersect(line(aabb._min        , v3( 1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb._min        , v3( 0.0f, 1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb._min        , v3( 0.0f, 0.0f, 1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	if(intersect(line(aabb._max        , v3(-1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb._max        , v3( 0.0f,-1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb._max        , v3( 0.0f, 0.0f,-1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	if(intersect(line(aabb.getVertex(4), v3( 1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb.getVertex(4), v3( 0.0f, 1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb.getVertex(2), v3( 0.0f, 0.0f, 1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	if(intersect(line(aabb.getVertex(2), v3( 1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb.getVertex(1), v3( 0.0f, 1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb.getVertex(1), v3( 0.0f, 0.0f, 1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	return false;
}