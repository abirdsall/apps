#ifndef kMathFrustumH
#define kMathFrustumH

#include "plane.h"

struct frustum
{
    enum
	{
		kNone		= 0x00,
		kNear		= 0x01, 
		kFar		= 0x02,
		kLeft		= 0x04,
		kRight		= 0x08,
		kTop		= 0x10,
		kBottom		= 0x20,
		kAll		= kNear | kFar | kLeft | kRight | kTop | kBottom
	};

	frustum(void);
	frustum(
		const plane& planeNear, // stupid 'plane_' prefix required due to obsolete keyword 'near'
		const plane& planeFar,
		const plane& planeLeft,
		const plane& planeRight,
		const plane& planeTop,
		const plane& planeBottom);

	plane	mPlanes[6];
	line	mLines[4];
};

#endif