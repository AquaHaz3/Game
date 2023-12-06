#include "Entity.h"

Entity::Entity(int x, int y, int w, int h)
	: Box2D(x, y, w, h)
{
	flags |= ENTITY_OBJECT;
	health = 20;
	speed = 0.5;
	this->type = EntityType::Unknown;
}

Entity::Entity(int x, int y, int w, int h, EntityType type) : Box2D(x, y, w, h)
{
	flags |= ENTITY_OBJECT;
	health = 20;
	speed = 0.5;
	this->type = type;
}

void Entity::Draw()
{}

void Entity::Update(__int64 tick)
{}

#include "../events/ArrowHitEvent.hpp"

void Entity::OnEvent(Event* event)
{
	if (event->uuid == ArrowHitEvent::getClassUUID()) {
		auto arrowHit = (ArrowHitEvent*)event;
		arrowHit->phase = true;
	}
}
