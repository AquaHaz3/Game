#pragma once

#include <raylib.h>
#include "Player.h"

namespace Movement {
	/*
	@return true if entity is moving
	*/
	static inline bool EntityWASDControl(Player* entity)
	{
		bool isMoved = false;
		if (IsKeyDown(KEY_W)) {
			entity->moveBy(0, -entity->speed);
			entity->direction = Direction::UP;
			isMoved = true;
		}
		if (IsKeyDown(KEY_S)) {
			entity->moveBy(0, entity->speed);
			entity->direction = Direction::DOWN;
			isMoved = true;
		}
		if (IsKeyDown(KEY_A)) {
			entity->moveBy(-entity->speed, 0);
			entity->direction = Direction::LEFT;
			isMoved = true;
		}
		if (IsKeyDown(KEY_D)) {
			entity->moveBy(entity->speed, 0);
			entity->direction = Direction::RIGHT;
			isMoved = true;
		}
		return isMoved;
	}

}