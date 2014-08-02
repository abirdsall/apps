#include "frustum.h"

frustum :: frustum(void) {}

frustum :: frustum(
	const plane& planeNear,
	const plane& planeFar,
	const plane& planeLeft,
	const plane& planeRight,
	const plane& planeTop,
	const plane& planeBottom)
{
	mPlanes[0] = planeNear;
	mPlanes[1] = planeFar;
	mPlanes[2] = planeLeft;
	mPlanes[3] = planeRight;
	mPlanes[4] = planeTop;
	mPlanes[5] = planeBottom;
}