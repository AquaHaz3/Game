#pragma once

#include "../core/Box2D.h"

class Block : Box2D
{

	Block() {};
	Block(int x, int y);

	virtual void Draw();

	virtual void Update(__int64 tick) { }

	virtual void OnEvent(Event* event) = 0;

};

