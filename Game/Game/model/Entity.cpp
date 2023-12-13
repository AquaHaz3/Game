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

void Entity::addEntityMob(EntityID id, std::string tex_path,
	short hp, bool isDistance, bool solid, uint16_t detectR, uint16_t idleR)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
	models[(int)id] = EntityModel(id, hp, isDistance, solid, detectR, idleR);
}

void Entity::addEntityBase(EntityID id, std::string tex_path)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
}

void Entity::InitEntities()
{
	addEntityMob(EntityID::Ghost, "entity/ghost.png", 15, false, false, 128, 64);
	addEntityMob(EntityID::Bat, "entity/bat.png", 7, false, true, 96, 72);
	addEntityMob(EntityID::Dark, "entity/dark.png", 25, true, true, 128, 32);
	addEntityBase(EntityID::Chest, "entity/chest.png");
}

Entity* Entity::EntityFactory(EntityID id, int x, int y, int w, int h, uint16_t ord)
{
	return EntityFactory(id, x, y, w, h, ord, { 0,0,0,0 });
}

#include "entity/Mob.h"
#include "entity/Chest.h"
#include "ItemEntity.h"
#include "Player.h"

Entity* Entity::EntityFactory(EntityID id, int x, int y, int w, int h, uint16_t ord, Color optional)
{
	if ((int)id > (int)EntityID::Chest) {
		return new Mob(x,y, id);
	}
	if (id == EntityID::Item) {
		return new ItemEntity(x, y, w, h, (ItemID)ord, optional);
	}
	if (id == EntityID::Player) {
		return new Player(x, y);
	}
	if (id == EntityID::Chest) {
		printf("Build chest at {%d, %d}\n", x, y);
		return new Chest(x, y);
	}

	return new Entity(x, y, w, h, EntityID::Unknown);
}

EntityModel::EntityModel()
{
	this->id = EntityID::Unknown;
	health = 0;
	isDistanceBattle = 0;
	this->solid = 0;
	this->detectRadius = 0;
	this->idleRadius = 0;
}
