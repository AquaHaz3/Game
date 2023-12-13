#pragma once

#include "../core/GameObject.h"
#include <raylib.h>
#include "Block.h"

class Background : public GameObject
{

public:

	Background();
	Background(int x, int y, int w, int h, BlockID id);


	virtual void Draw() override;
	virtual void Update(__int64 tick) {};
	virtual void OnEvent(Event* event) {};

public:

	uint8_t id;
	int x;
	int y;
	short blocksW;
	short blocksH;

};

