#include "AABB.h"

/*

*/
bool UtilAABB::isOverlap(AABB* a, AABB* b)
{
    auto d1x = b->min.x - a->max.x;
    auto d1y = b->min.y - a->max.y;
    auto d2x = a->min.x - b->max.x;
    auto d2y = a->min.y - b->max.y;

    if (d1x > 0 || d1y > 0)
        return false;

    if (d2x > 0 || d2y > 0)
        return false;

    return true;
}

bool UtilAABB::isIntersects(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2)
{

	float ax = a2.x - a1.x;     // direction of line a
	float ay = a2.y - a1.y;     // ax and ay as above

	float bx = b1.x - b2.x;     // direction of line b, reversed
	float by = b1.y - b2.y;     // really -by and -by as above

	float dx = b1.x - a1.x;     // right-hand side
	float dy = b1.y - a1.y;

	float det = ax * by - ay * bx;

	if (det == 0) return false;

	float r = (dx * by - dy * bx) / det;
	float s = (ax * dy - ay * dx) / det;

	return !(r < 0 || r > 1 || s < 0 || s > 1);
}

bool UtilAABB::isIntersectsBox(AABB* box, Vector2 b1, Vector2 b2)
{
	bool t = isIntersects(box->min, box->max, b1, b2);
	t |= isIntersects({ box->min.x, box->max.y }, { box->max.x, box->min.y }, b1, b2);
	return t;
}

