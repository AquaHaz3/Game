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
