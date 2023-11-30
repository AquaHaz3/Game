#pragma once

struct Vec2
{

	Vec2() { x = 0; y = 0; }
	Vec2(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int x;
	int y;

};

struct AABB
{

	Vec2 min;
	Vec2 max;

	AABB() {};
	AABB(int x1, int y1, int x2, int y2)
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

