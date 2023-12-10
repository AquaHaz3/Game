#pragma once

#include "../Entity.h"

class Mob : public Entity
{

public:

	Mob(int x, int y, EntityID id);

	virtual void Draw();
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

	Vector2 idlePos;

    int _idle_MoveState;
    float _idle_dx;
    float _idle_dy;

	float max_hp;

};

