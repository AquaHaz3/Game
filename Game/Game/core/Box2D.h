#pragma once

#include "GameObject.h"
#include <raylib.h>
#include "AABB.h"

class Box2D : public GameObject
{
public:

	Box2D() {
		w = 0; h = 0;
	};

	Box2D(int x, int y, int camWidth, int camHeight) 
	{
		aabb = AABB((float)x, (float)y, (float) x + camWidth, (float) y + camHeight);
		w = camWidth;
		h = camHeight;
		flags |= SOLID_OBJECT;

	}

	void setPos(float x, float y);
	void moveBy(float xRel, float yRel);

	float getX() { return aabb.min.x; };
	float getY() { return aabb.min.y; };
	Vector2& getPos() { return aabb.min; };

	AABB aabb;

protected: 

	int w;
	int h;

};

