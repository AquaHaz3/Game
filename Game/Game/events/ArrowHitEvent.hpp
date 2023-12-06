#pragma once

#include "Event.h"
#include "../model/entity/Arrow.h"
class ArrowHitEvent : public Event {

public:

	MAKE_CLASS_UNIQUE;
	float damage;
	bool destroyArrowAfterHit;
	bool phase;

public:

	ArrowHitEvent(Arrow* arrow) : Event(GET_CLASS_UUID()) {
		damage = arrow->health;
		destroyArrowAfterHit = false;
		phase = false;
	}

};