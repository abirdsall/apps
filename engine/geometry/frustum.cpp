#include "frustum.h"

frustum :: frustum() {}

frustum :: frustum(
	const plane& planeNear,
	const plane& planeFar,
	const plane& planeLeft,
	const plane& planeRight,
	const plane& planeTop,
	const plane& planeBottom)
{
	_planes[0] = planeNear;
	_planes[1] = planeFar;
	_planes[2] = planeLeft;
	_planes[3] = planeRight;
	_planes[4] = planeTop;
	_planes[5] = planeBottom;
}