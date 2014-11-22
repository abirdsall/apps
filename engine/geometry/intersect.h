#ifndef kIntersectH
#define kIntersectH

#include "os.h"
#include "geometry.h"

struct aabb;
class line;
struct plane;
struct frustum;
struct triangle;

bool intersect(const v3& position, const frustum& frustum);
f32  intersect(const line& line, const v3& v);
f32  intersect(const line& line1, const line& line2);
bool intersect(const line& line, const plane& plane, f32& distance, bool infinite);
bool intersect(const line& line, const plane& plane, v3& outPosition, bool infinite);
bool intersect(const line& line, const frustum& frustum, f32& enter, f32& exit);
bool intersect(const line& line, const geometry::Triangle& triangle, v3* outPosition, v3* outNormal, f32 lineLength = MaxF32);
bool intersect(const line& line, const aabb& aabb, f32 lineLength = MaxF32);
bool intersect(const plane& plane1, const plane& plane2, line& line);
bool intersect(const aabb& aabb, const frustum& frustum);
bool intersectFast(const aabb& aabb, const frustum& frustum);
u32  intersectFace(const line& line, const aabb& aabb);

#endif