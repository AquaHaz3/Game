#pragma once

#include "../Entity.h"

class Chest : public Entity
{

public:

	Chest();
	Chest(int x, int y);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

private:

	uint8_t frame;
	bool isPlayerNear;

};