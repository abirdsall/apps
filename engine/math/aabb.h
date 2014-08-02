#ifndef kMathAabbH
#define kMathAabbH

#include "os.h"
#include "plane.h"

struct v3;
struct plane;
struct aabb
{
					aabb(void);
					aabb(const v3& min, const v3& max);
	v3				getVertex(const u8 i) const;
	void			getFace(const u8 face, v3* v) const;
	f32				getRadiusInDirection(const v3& dir) const;
	f32				distanceToPlane(const plane& plane) const;
	plane::eSide	planeSide(const plane& plane) const;

	v3				mMin;
	v3				mMax;
};

#endif