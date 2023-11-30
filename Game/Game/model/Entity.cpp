#include "Entity.h"

Entity::Entity(int x, int y, int w, int h, std::string sprite_path)
	: Box2D(x, y, w, h)
{
	health = 20;
	speed = 0.5;
	texture = Sprite(sprite_path);
}

void Entity::Draw()
{
	texture.Draw(this->aabb.min);
}

void Entity::Update(__int64 tick)
{
	
}

void Entity::OnEvent(Event* event)
{
}
