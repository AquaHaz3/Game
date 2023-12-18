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

Entity::~Entity()
{
	if (texture != nullptr) {
		texture.reset();
		texture = nullptr;
	}
}

void Entity::Draw()
{}

void Entity::Update(__int64 tick)
{}

#include "../events/ProjectileHitEvent.hpp"

void Entity::OnEvent(Event* event)
{
	if (event->uuid == ProjectileHitEvent::getClassUUID()) {
		auto arrowHit = (ProjectileHitEvent*)event;
		arrowHit->phase = true;
	}
}

std::vector<SpriteRef> Entity::textures = std::vector<SpriteRef>(20);
std::vector<EntityModel> Entity::models = std::vector<EntityModel>(20);

void Entity::addEntityMob(EntityID id, std::string tex_path,
	short hp, bool isDistance, bool solid, uint16_t detectR, uint16_t idleR)
{
	textures[(int)id] = SpriteLoader::GetSprite(tex_path);
	models[(int)id] = EntityModel(id, hp, isDistance, solid, detectR, idleR);
}

void Entity::addEntityBase(EntityID id, std::string tex_path)
{
	textures[(int)id] = SpriteLoader::GetSprite(tex_path);
}

void Entity::InitEntities()
{
	addEntityMob(EntityID::Ghost, "entity/ghost.png", 15, false, false, 140, 48);
	addEntityMob(EntityID::Bat, "entity/bat.png", 7, false, true, 128, 72);
	addEntityMob(EntityID::Dark, "entity/dark.png", 25, true, true, 160, 32);
	addEntityMob(EntityID::Agent, "entity/agent.png", 20, true, true, 196, 16);
	addEntityMob(EntityID::Glow, "entity/glow.png", 12, false, true, 196, 16);

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
		//printf("Build chest at {%d, %d}\n", x, y);
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
