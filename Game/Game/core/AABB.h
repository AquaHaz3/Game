#pragma once

#include <raylib.h>

struct AABB
{

	Vector2 min;
	Vector2 max;

	AABB() { max = { 0,0 }; min = { 0,0 }; }

	AABB(float x1, float y1, float x2, float y2)
	{
		min.x = x1;
		min.y = y1;
		max.x = x2;
		max.y = y2;
	};

};

namespace UtilAABB {

	static bool isOverlap(AABB* a, AABB* b);

}

