#include "Entity.h"



Entity::Entity(int x, int y, int w, int h)
	: Box2D(x, y, w, h)
{
	flags |= ENTITY_OBJECT;
	health = 20;
	speed = 0.5;
	this->type = EntityID::Unknown;
}

Entity::Entity(int x, int y, int w, int h, EntityID type) : Box2D(x, y, w, h)
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

std::vector<Texture2D> Entity::textures = std::vector<Texture2D>(20);
std::vector<EntityModel> Entity::models = std::vector<EntityModel>(20);

void Entity::addEntity(EntityID id, std::string tex_path,
	short hp, bool isDistance, bool solid, uint16_t detectR, uint16_t idleR)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
	models[(int)id] = EntityModel(id, hp, isDistance, solid, detectR, idleR);
}

void Entity::InitEntities()
{
	addEntity(EntityID::Ghost, "entity/ghost.png", 20, false, false, 128, 64);
	addEntity(EntityID::Bat, "entity/bat.png", 10, false, false, 96, 72);
	addEntity(EntityID::Dark, "entity/dark.png", 30, true, true, 128, 32);
}
