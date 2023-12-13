#pragma once

#include "../Entity.h"

class Mob : public Entity
{

public:

	Mob(int x, int y, EntityID id);

	virtual void Draw();
	void inline updateDirection(float& angle);
	virtual void Update(__int64 tick);

	virtual void OnEvent(Event* event) override;

private:

	bool isHaveTarget;

	void chooseTarget(__int64 tick);
	void toogleState();

	Entity* target;
	int targetChangeTick;
	bool isIdle;

	int idleTick;
	char direction;

	float detectRadius;
	float idleRadius;
	bool isDistanceBattle;
	bool isSolid;

	Vector2 idlePos;

    int _idle_MoveState;
    float _idle_dx;
    float _idle_dy;

	int max_hp;

};

