#pragma once

#include "../Entity.h"

class Mob : public Entity
{

public:

	Mob(int x, int y, EntityID id);

	virtual void Draw();
	virtual void Update(__int64 tick);

	virtual void OnEvent(Event* event) {};

private:

	bool isHaveTarget;

	Entity* chooseTarget();

	Entity* target;
	int targetChangeTick;

};

