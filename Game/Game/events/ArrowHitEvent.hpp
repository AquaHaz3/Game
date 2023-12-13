#pragma once

#include "Event.h"
#include "../model/entity/Arrow.h"
class ArrowHitEvent : public Event {

public:

	MAKE_CLASS_UNIQUE;
	int damage;
	bool destroyArrowAfterHit;
	bool phase;
	bool isExplosiveArrow;
	uint16_t x;
	uint16_t y;
	Entity* arrowOwner;

public:

	ArrowHitEvent(Arrow* arrow) : Event(GET_CLASS_UUID()) {
		damage = arrow->health;
		destroyArrowAfterHit = false;
		phase = false;
		arrowOwner = arrow->owner;
		this->isExplosiveArrow = (arrow->id == 1);
		x = (uint16_t)arrow->aabb.min.x;
		y = (uint16_t)arrow->aabb.min.y;
	}

};