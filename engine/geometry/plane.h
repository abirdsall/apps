#ifndef kMathPlaneH
#define kMathPlaneH

#include "os.h"
#include "line.h"

class line;
struct plane
{
	enum Side
	{
		eFront		= 0,
		eBack		= 1,
		eIntersect	= 2,
		eOn		= 3 
	};

			plane();
			plane(const v3& normal, const v3& point);
	f32		distanceToPoint(const v3& point) const;
	v3		_normal;
	f32		_offset;
};

#endif