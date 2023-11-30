#pragma once

#include "GameObject.h"
#include <raylib.h>
#include "AABB.h"

class Box2D : public GameObject
{
public:

	Box2D() {};

	Box2D(int x, int y, int width, int height) 
	{
		aabb = AABB(x, y, x + width, y + height);
		w = width;
		h = height;
		flags |= SOLID_OBJECT;
	}

	void setPos(int x, int y);
	void moveBy(int xRel, int yRel);

	int getX() { return aabb.min.x; };
	int getY() { return aabb.min.y; };
	Vec2& getPos() { return aabb.min; };

#if 1

	virtual void Draw()
	{
		DrawRectangle(aabb.min.x, aabb.min.y, w, h, GREEN);
	}
	virtual void Update(){
		 n
	}
	virtual void OnEvent(Event* event){}

#endif

private: 

	AABB aabb;
	int w;
	int h;

};

