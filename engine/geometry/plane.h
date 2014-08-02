#ifndef kMathPlaneH
#define kMathPlaneH

#include "os.h"
#include "line.h"

class line;
struct plane
{
	enum eSide
	{
		eFront		= 0,
		eBack		= 1,
		eIntersect	= 2,
		eOn		= 3 
	};

			plane(void);
			plane(const v3& normal, const v3& point);
	f32		distanceToPoint(const v3& point) const;
	v3		mNormal;
	f32		mOffset;
};

#endif