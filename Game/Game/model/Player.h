#pragma once

#include "Entity.h"

class Player : public Entity
{

public:

	Player() : Entity(0, 0, 32, 32, EntityType::Player) {}

	Player(int x, int y);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

private:

	int walk_tick;
	Sprite texture;

};

