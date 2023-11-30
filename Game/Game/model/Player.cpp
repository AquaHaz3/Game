#include "Player.h"

Player::Player(int x, int y) : Entity(x, y, 32, 32, "Player.png")
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
	texture.DrawPro(aabb.min, 32, 32, (int)direction + walk_tick * 4);
}

#include "Movement.h"

void Player::Update(__int64 tick)
{
	if (tick % 3 != 0) return;
	walk_tick = (tick / 100) % 2 + 1;
	bool isMoved = Movement::EntityWASDControl(this);
	if(!isMoved) walk_tick = 0;
}

void Player::OnEvent(Event* event)
{
}
