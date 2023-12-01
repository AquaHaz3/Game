#pragma once

#include "Entity.h"

class Mob : Entity
{

public:

	virtual void Draw();
	virtual void Update(__int64 tick);

	virtual void OnEvent(Event* event) {};

};

