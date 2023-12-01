#pragma once

#include "Block.h"



class Wall : public Box2D
{

public:

	short blocksW;
	short blocksH;
	char id;

public:

	Wall() { blocksW = 0; blocksH = 0; id = 0; };
	Wall(int x, int y, int x2, int y2, BlockID id);

	virtual void Draw();

	virtual void Update(__int64 tick) { }

	virtual void OnEvent(Event* event) {};

};

