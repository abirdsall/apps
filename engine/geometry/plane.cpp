#include "plane.h"

plane :: plane() {}
plane :: plane(const v3& normal, const v3& point)
{
	_normal = normal;
	_offset = dot(_normal, point);
}

f32 plane :: distanceToPoint(const v3& point) const
{
	return dot(_normal, point) - _offset;
}