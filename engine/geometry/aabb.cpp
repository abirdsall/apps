#include "aabb.h"

aabb :: aabb(void){}
aabb :: aabb(const v3& min, const v3& max)
{
	mMin = min;
	mMax = max;
}

v3 aabb :: getVertex(const u8 i) const
{
	return v3(i & 1 ? mMax.x : mMin.x, i & 2 ? mMax.y : mMin.y, i & 4 ? mMax.z : mMin.z);
}
	
void aabb :: getFace(const u8 face, v3* v) const
{
	ASSERT(face < 7);
	
	const u8 faces[24] =
	{
		2, 3, 1, 0,
		0, 1, 5, 4,
		4, 6, 2, 0,
		7, 6, 4, 5,
		3, 2, 6, 7,
		7, 5, 1, 3
	};
	
	const u8* p = faces + face * 4;
	
	v[0] = getVertex(p[0]);
	v[1] = getVertex(p[1]);
	v[2] = getVertex(p[2]);
	v[3] = getVertex(p[3]);
}

f32 aabb :: distanceToPlane(const plane& plane) const
{
	f32	distance	= plane.distanceToPoint(lerp(mMin, mMax, 0.5f));
	f32 radius		= getRadiusInDirection(plane.mNormal);

	if(distance > radius) return distance - radius;
	if(distance <-radius) return distance + radius;
	return 0.0f;
}

plane::eSide aabb :: planeSide(const plane& plane) const
{
	f32	distance	= plane.distanceToPoint(lerp(mMin, mMax, 0.5f));
	f32 radius		= getRadiusInDirection(plane.mNormal);// + 1e-3f; // better error > than <

	if(distance > radius)
	{
		return plane::eFront;
	}
	if(distance <-radius) return plane::eBack;
	return plane::eIntersect;
}

f32 aabb :: getRadiusInDirection(const v3& dir) const
{
	return 0.5f * (	core::abs(dir.x * (mMax.x - mMin.x)) +
					core::abs(dir.y * (mMax.y - mMin.y)) +
					core::abs(dir.z * (mMax.z - mMin.z)) );
}