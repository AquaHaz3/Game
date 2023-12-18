#pragma once

#include "Event.h"
#include "../model/entity/Arrow.h"
#include "../model/entity/Bullet.h"

class ProjectileHitEvent : public Event {

public:

	MAKE_CLASS_UNIQUE;
	int damage;
	bool destroyArrowAfterHit;
	bool phase;
	bool isExplosiveArrow;
	uint16_t x;
	uint16_t y;
	float xSpeed = 0;
	float ySpeed = 0;
	Entity* arrowOwner;

public:

	ProjectileHitEvent(Arrow* arrow) : Event(GET_CLASS_UUID()) {
		damage = arrow->health;
		destroyArrowAfterHit = false;
		phase = false;
		arrowOwner = arrow->owner;
		this->isExplosiveArrow = (arrow->id == 1);
		x = (uint16_t)arrow->aabb.min.x;
		y = (uint16_t)arrow->aabb.min.y;
	}
	
	ProjectileHitEvent(Bullet* bullet) : Event(GET_CLASS_UUID()) {
		damage = bullet->health;
		destroyArrowAfterHit = true;
		phase = false;
		arrowOwner = bullet->owner;
		this->isExplosiveArrow = bullet->isExplosive();
		x = (uint16_t)bullet->aabb.min.x;
		y = (uint16_t)bullet->aabb.min.y;
	}

};