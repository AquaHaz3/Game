#include "Player.h"

Player::Player(int x, int y) : Entity(x, y, 20, 30, "Player.png")
{
	health = 20;
	speed = 1;

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


}

void Player::Draw()
{
	texture.DrawPro(aabb.min.x-6, aabb.min.y-1, 32, 32, (int)direction + walk_tick * 4);
	//DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, RED);
}

#include "Movement.h"
#include "../GameLauncher.h"

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
}
