#include "Box2D.h"

void Box2D::setPos(int x, int y)
{
	aabb.min.x = x;
	aabb.min.y = y;
	aabb.max.x = x + w;
	aabb.max.y = y + h;
}

void Box2D::moveBy(int xRel, int yRel)
{
	aabb.min.x += xRel;
	aabb.min.y += yRel;
	aabb.max.x += xRel;
	aabb.max.y += yRel;
}
