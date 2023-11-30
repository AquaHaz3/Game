#pragma once

#include "../core/Box2D.h"
#include "../core/Sprite.h"

enum class Direction {
	RIGHT = 0,
	DOWN = 1,
	LEFT = 2,
	UP = 3,
};

class Entity : public Box2D
{

public:

	int health;
	float speed;
	Direction direction;

public:

	Entity(int x, int y, int w, int h, std::string sprite_path);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

protected:

	Sprite texture;

};

