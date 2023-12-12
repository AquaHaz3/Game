#pragma once

#include "../Entity.h"
#include <vector>
#include "../Item.h"

class Arrow : public Entity
{

public:

	Arrow() : Entity(0,0,0,0) {};
	Arrow(int x, int y, float speed, float angle, Weapon* weapon, Entity* owner);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

	float angle;
	float xSpeed;
	float ySpeed;

public:

	static void InitArrows();
	static std::vector<Sprite> arrow_types;

	Entity* owner;
	uint8_t id;

private:

	int lifeTime;
	bool isMoving;
	bool isAlive;

	

};

