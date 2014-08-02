#include "aabb.h"
#include "frustum.h"
#include "geometry.h"
#include "intersect.h"

bool intersect(const v3& position, const frustum& frustum)
{
	s32 i;
	for(i = 0; i < 6; i++)
	{
		if(frustum.mPlanes[i].distanceToPoint(position) < 0.0f)
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
	v3  pos		= p1 + (p2 - p1) * math::clamp(u, 0.0f, 1.0f);
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
	if(math::abs(denom) > 1e-5f && math::abs(d4343) > 1e-5f)		// avoid divide by zero
	{
		f32	numer	= d1343 * d4321 - d1321 * d4343;
		f32 mua		= numer / denom;					// distance along line 1
		f32 mub		= (d1343 + d4321 * (mua)) / d4343;	// distance along line 2
		mua			= math::clamp(mua, 0.0f, 1.0f);
		mub			= math::clamp(mub, 0.0f, 1.0f);
		v3	rp1		= p1 + p21 * mua;
		v3	rp2		= p3 + p43 * mub;
		return distance(rp1, rp2);
	}
	return kMaxF32;
}

bool intersect(const line& line, const plane& plane, f32& distance, const bool infinite)
{
	f32 angle = dot(line.getDir(), plane.mNormal);
    if(angle != 0.0f)
    {
        distance = dot(plane.mNormal, v3(plane.mNormal * plane.mOffset) -  line.getPos()) / angle;
		return (infinite || distance >= 0.0f ? true : false);
    }
	if(dot(line.getPos(), plane.mNormal) - plane.mOffset == 0.0f)
	{
		distance = 0.0f;
        return true;
	}
    return false;
}

bool intersect(const line& line, const plane& plane, v3& outPosition, const bool infinite)
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
	const plane*	planes = frustum.mPlanes;
	line			lineA;
	line			lineB;
	v3				pointA;
	v3				pointB;
	v3				hit1 = l.getPos();
	v3				hit2 = l.getPos() + l.getDir() * kMaxF32;
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
		v3   innerNormal	= dot(innerVector, planes[i].mNormal) >= 0.0f ? planes[i].mNormal : -planes[i].mNormal;
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

bool intersect(const line& line, const geometry::triangle& triangle, v3* outPosition, v3* outNormal, const f32 lineLength)
{
	v3	ea = triangle.mVertices[1] - triangle.mVertices[0];
	v3	eb = triangle.mVertices[2] - triangle.mVertices[0];
	v3	s;
	v3	p = cross(line.getDir(), eb);
	f32	u;
	f32 v;
	f32 t = dot(p, ea);

	if(t <= 0.0f)
		return false;

	t = 1.0f / t;

	s = line.getPos() - triangle.mVertices[0];
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

bool intersect(const line& line, const aabb& aabb, const f32 lineLength)
{
	f32 tf, tn, tm, t0, t1;

	if(line.getDir().x == 0.0f)
	{
		if(line.getPos().x < aabb.mMin.x || line.getPos().x > aabb.mMax.x)
			return false;
		tn = 0.0f;
		tf = lineLength;
	}
	else
	{
		tm = 1.0f / line.getDir().x;
		tn = tm * (aabb.mMin.x - line.getPos().x);
		tf = tm * (aabb.mMax.x - line.getPos().x);
		if(tn > tf)
		{
			tm = tn; tn = tf; tf = tm;
		}
		if(tf < 0.0f || tn > lineLength)
			return false;
	}	

	if(line.getDir().z == 0.0f)
	{
		if(line.getPos().z < aabb.mMin.z || line.getPos().z > aabb.mMax.z)
			return false;
	}
	else
	{
		tm = 1.0f / line.getDir().z;
		t0 = tm * (aabb.mMin.z - line.getPos().z);
		t1 = tm * (aabb.mMax.z - line.getPos().z);
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
		if(line.getPos().y < aabb.mMin.y || line.getPos().y > aabb.mMax.y)
			return false;
	}
	else
	{
		tm = 1.0f / line.getDir().y;
		t0 = tm * (aabb.mMin.y - line.getPos().y);
		t1 = tm * (aabb.mMax.y - line.getPos().y);
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
	f32 y0x	= pos.y + (aabb.mMin.x - pos.x) * dir.y * rx;
	f32 z0x	= pos.z + (aabb.mMin.x - pos.x) * dir.z * rx;
	if(pos.x < aabb.mMin.x && y0x > aabb.mMin.y && y0x < aabb.mMax.y && z0x > aabb.mMin.z && z0x < aabb.mMax.z) return 1;

	f32 y1x	= pos.y + (aabb.mMax.x - pos.x) * dir.y * rx;
	f32 z1x	= pos.z + (aabb.mMax.x - pos.x) * dir.z * rx;
	if(pos.x > aabb.mMax.x && y1x > aabb.mMin.y && y1x < aabb.mMax.y && z1x > aabb.mMin.z && z1x < aabb.mMax.z) return 2;

	f32 ry	= 1.0f / dir.y;
	f32 x0y	= pos.x + (aabb.mMin.y - pos.y) * dir.x * ry;
	f32 z0y	= pos.z + (aabb.mMin.y - pos.y) * dir.z * ry;
	if(pos.y < aabb.mMin.y && x0y > aabb.mMin.x && x0y < aabb.mMax.x && z0y > aabb.mMin.z && z0y < aabb.mMax.z) return 3;

	f32 x1y	= pos.x + (aabb.mMax.y - pos.y) * dir.x * ry;
	f32 z1y	= pos.z + (aabb.mMax.y - pos.y) * dir.z * ry;
	if(pos.y > aabb.mMax.y && x1y > aabb.mMin.x && x1y < aabb.mMax.x && z1y > aabb.mMin.z && z1y < aabb.mMax.z) return 4;

	f32 rz	= 1.0f / dir.z;
	f32 x0z	= pos.x + (aabb.mMin.z - pos.z) * dir.x * rz;
	f32 y0z	= pos.y + (aabb.mMin.z - pos.z) * dir.y * rz;
	if(pos.z < aabb.mMin.z && x0z > aabb.mMin.x && x0z < aabb.mMax.x && y0z > aabb.mMin.y && y0z < aabb.mMax.y) return 5;

	f32 x1z	= pos.x + (aabb.mMax.z - pos.z) * dir.x * rz;
	f32 y1z	= pos.y + (aabb.mMax.z - pos.z) * dir.y * rz;
	if(pos.z > aabb.mMax.z && x1z > aabb.mMin.x && x1z < aabb.mMax.x && y1z > aabb.mMin.y && y1z < aabb.mMax.y) return 6;

	return 0;
}

bool intersect(const plane& plane1, const plane& plane2, line& line)
{
	const f32 o1  = plane1.mOffset;
	const f32 o2  = plane2.mOffset;
	const v3& n1  = plane1.mNormal;
	const v3& n2  = plane2.mNormal;
	const v3  dir = cross(n1, n2);
	const f32 len = length(dir); 
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
		if(aabb.planeSide(frustum.mPlanes[i]) == plane::eBack)
			return false;
	}
	return true;
}

bool intersect(const aabb& aabb, const frustum& frustum)
{
	u8 i, j;
	for(i = 0, j = 0; i < 6; i++)
	{
		plane::eSide side = aabb.planeSide(frustum.mPlanes[i]);
		if(side == plane::eBack ) return false; // no intersection
		if(side == plane::eFront && i > 1) j++;
	}
	
	if(j == 4)
		return true;

	for(i = 0; i < 4; i++)
		if(intersect(frustum.mLines[i], aabb))
			return true; // frustum edge intersects aabb

	f32 hit1;
	f32 hit2;
	f32 dimX = aabb.mMax.x - aabb.mMin.x;
	f32 dimY = aabb.mMax.y - aabb.mMin.y;
	f32 dimZ = aabb.mMax.z - aabb.mMin.z;
	if(intersect(line(aabb.mMin        , v3( 1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb.mMin        , v3( 0.0f, 1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb.mMin        , v3( 0.0f, 0.0f, 1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	if(intersect(line(aabb.mMax        , v3(-1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb.mMax        , v3( 0.0f,-1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb.mMax        , v3( 0.0f, 0.0f,-1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	if(intersect(line(aabb.getVertex(4), v3( 1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb.getVertex(4), v3( 0.0f, 1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb.getVertex(2), v3( 0.0f, 0.0f, 1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	if(intersect(line(aabb.getVertex(2), v3( 1.0f, 0.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimX) return true;
	if(intersect(line(aabb.getVertex(1), v3( 0.0f, 1.0f, 0.0f)), frustum, hit1, hit2) && hit1 <= dimY) return true;
	if(intersect(line(aabb.getVertex(1), v3( 0.0f, 0.0f, 1.0f)), frustum, hit1, hit2) && hit1 <= dimZ) return true;
	return false;
}