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

	virtual ~Box2D() {};

	Box2D(int x, int y, int width, int height) 
	{
		aabb = AABB((float)x, (float)y, (float) x + width, (float) y + height);
		w = width;
		h = height;
		flags |= SOLID_OBJECT;

	}

	void setPos(float x, float y);
	void moveBy(float xRel, float yRel);

	float getX() { return aabb.min.x; };
	float getY() { return aabb.min.y; };
	Vector2& getPos() { return aabb.min; };

	AABB aabb;
	int w;
	int h;
	

};

#include <functional>

#define MORPHISM_STATE_UNACTIVE 0
#define MORPHISM_STATE_ACTION 1
#define MORPHISM_STATE_ACTION_RETURN 3
#define MORPHISM_STATE_END_ACTION 4
#define MORPHISM_STATE_STUB 5

template <typename T>
class Morphism {

public:

	Morphism() {
		this->max = T(); this->min = T();
		this->value = 0; this->speed = T();
		this->state = 0;
	}; 

	Morphism(T* value, T max, T min, T speed, std::function<void()> onAction) {
		this->value = value;
		this->state = MORPHISM_STATE_UNACTIVE;  
		*value = min;
		this->speed = speed;
		this->max = max;
		this->min = min;
		this->onAction = onAction;
	};

	uint8_t Notify() {
		if (state == MORPHISM_STATE_ACTION) return MORPHISM_STATE_STUB;
		if (state == MORPHISM_STATE_ACTION_RETURN) return MORPHISM_STATE_STUB;
		if (state == MORPHISM_STATE_UNACTIVE)
			state = MORPHISM_STATE_ACTION;
		if (state == MORPHISM_STATE_END_ACTION)
			state = MORPHISM_STATE_ACTION_RETURN;
		return state;
	};

	void toOrigin() {
		if (state == MORPHISM_STATE_UNACTIVE) return;
		if (state == MORPHISM_STATE_ACTION_RETURN) return;

		state = MORPHISM_STATE_ACTION_RETURN;
	}

	void Update()
	{
		if (value == nullptr) return;
		if (state == MORPHISM_STATE_UNACTIVE) return;
		if (state == MORPHISM_STATE_END_ACTION) return;

		if (state == MORPHISM_STATE_ACTION) {
			if (*value < max) {
				*value += speed;
				onAction();
			}
			else state = MORPHISM_STATE_END_ACTION;
		}
		else {
			if (*value > min) {
				*value -= speed;
				onAction();
			}
			else state = MORPHISM_STATE_UNACTIVE;
		}
	};

	T max;
	T min;

private:

	std::function<void()> onAction;
	
	T* value;
	T speed;
	uint8_t state;
};

