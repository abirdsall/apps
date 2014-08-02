#include "plane.h"

plane :: plane(void) {}
plane :: plane(const v3& normal, const v3& point)
{
	mNormal = normal;
	mOffset = dot(mNormal, point);
}

f32 plane :: distanceToPoint(const v3& point) const
{
	return dot(mNormal, point) - mOffset;
}