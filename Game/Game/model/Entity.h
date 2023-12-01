#pragma once

#include "../core/Box2D.h"
#include "../core/Sprite.h"

enum class EntityType {

	Unknown = 0,
	Player = 1,
	Item = 2

};

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
	EntityType type;


public:

	Entity(int x, int y, int w, int h);
	Entity(int x, int y, int w, int h, EntityType type);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

protected:

	Sprite texture;

};

