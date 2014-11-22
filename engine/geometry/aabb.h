#ifndef kMathAabbH
#define kMathAabbH

#include "os.h"
#include "plane.h"

struct plane;
struct aabb
{
					aabb();
					aabb(const v3& min, const v3& max);
	v3				getVertex(u8 i) const;
	void			getFace(u8 face, v3* v) const;
	f32				getRadiusInDirection(const v3& dir) const;
	f32				distanceToPlane(const plane& plane) const;
	plane::Side	planeSide(const plane& plane) const;

	v3				_min;
	v3				_max;
};

#endif