#include "aabb.h"

aabb :: aabb(){}
aabb :: aabb(const v3& min, const v3& max)
{
	_min = min;
	_max = max;
}

v3 aabb :: getVertex(u8 i) const
{
	return v3(i & 1 ? _max.x : _min.x, i & 2 ? _max.y : _min.y, i & 4 ? _max.z : _min.z);
}
	
void aabb :: getFace(u8 face, v3* v) const
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
	f32	distance	= plane.distanceToPoint(lerp(_min, _max, 0.5f));
	f32 radius		= getRadiusInDirection(plane._normal);

	if(distance > radius) return distance - radius;
	if(distance <-radius) return distance + radius;
	return 0.0f;
}

plane::Side aabb :: planeSide(const plane& plane) const
{
	f32	distance	= plane.distanceToPoint(lerp(_min, _max, 0.5f));
	f32 radius		= getRadiusInDirection(plane._normal);// + 1e-3f; // better error > than <

	if(distance > radius)
	{
		return plane::eFront;
	}
	if(distance <-radius) return plane::eBack;
	return plane::eIntersect;
}

f32 aabb :: getRadiusInDirection(const v3& dir) const
{
	return 0.5f * (	core::abs(dir.x * (_max.x - _min.x)) +
					core::abs(dir.y * (_max.y - _min.y)) +
					core::abs(dir.z * (_max.z - _min.z)) );
}