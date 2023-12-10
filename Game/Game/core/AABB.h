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

	/* Проверка на столкновение */
	bool isOverlap(AABB* a, AABB* b);

	/* Лежит ли точка на прямоугольнике? */
	inline bool isPointOn(Vector2& p, AABB* aabb) 
	{
		if (p.x >= aabb->min.x && p.x < aabb->max.x) {
			return (p.y >= aabb->min.y && p.y < aabb->max.y);
		}
		return false;
	}
	
	bool isIntersects(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2);
	bool isIntersectsBox(AABB* box, Vector2 b1, Vector2 b2);

}

