#include "Player.h"
#include "../core/Debug.h"

Player::Player(int x, int y) : Entity(x, y, 20, 30, EntityType::Player)
{

	health = 20;
	speed = 1;
	texture = Sprite("player.png");

	texture.addTile(32, 64, 32, 32); // Go to RIGHT
	texture.addTile(32, 0, 32, 32); // Go to DOWN
	texture.addTile(32, 32, 32, 32); // Go to LEFT
	texture.addTile(32, 96, 32, 32); // Go to UP

	texture.addTile(0, 64, 32, 32); // Go to RIGHT
	texture.addTile(0, 0, 32, 32); // Go to DOWN
	texture.addTile(0, 32, 32, 32); // Go to LEFT
	texture.addTile(0, 96, 32, 32); // Go to UP

	texture.addTile(64, 64, 32, 32); // Go to RIGHT
	texture.addTile(64, 0, 32, 32); // Go to DOWN
	texture.addTile(64, 32, 32, 32); // Go to LEFT
	texture.addTile(64, 96, 32, 32); // Go to UP

	if (w < 32) {
		x_offset = (32 - w) / 2;
		aabb.min.x = x + x_offset;
	}
	if (h < 32) {
		y_offset = (32 - h) / 2;
		aabb.min.y = y + y_offset;
	}

}

void Player::Draw()
{
	
	//texture.Draw({ aabb.min.x, aabb.min.y });

	texture.DrawPro((int)aabb.min.x - x_offset, (int)aabb.min.y - y_offset, 32, 32, (int)direction + walk_tick * 4);
	Rectangle render = { aabb.min.x, aabb.min.y, 96, 96 };
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLinesEx(render, 1, PURPLE);
		DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, {250, 20, 20, 200});
	}
}

#include "Movement.h"
#include "../GameLauncher.h"
#include "../events/CollectItemEvent.hpp"

void Player::Update(__int64 tick)
{
	if (tick % 3 != 0) return;
	walk_tick = (tick / 100) % 2 + 1;
	Vector2 pre = aabb.min;
	bool isMoved = Movement::EntityWASDControl(this);
	if (isMoved) {
		bool isCollided = false;
		for (auto solid : GameLauncher::current_scene->boxes) {
			if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
				if (solid->flags & ENTITY_OBJECT) {
					Entity* e = (Entity*) solid;
					if (e->type == EntityType::Item) {
						auto event = new CollectItemEvent((ItemEntity*)e);
						OnEvent(event);
						delete event;
						GameLauncher::current_scene->removeObject(e);
					}
				}
				isCollided = true;
				break;
			}
		}
		if (isCollided) setPos(pre.x, pre.y);
	}
	else {
		walk_tick = 0;
	}
}

void Player::OnEvent(Event* event)
{
	if (event->uuid == CollectItemEvent::getClassUUID()) 
	{
		
	}
}
