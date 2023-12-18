#pragma once

#include "../Entity.h"
#include "../Item.h"

class Chest : public Entity
{

public:

	Chest();
	Chest(int x, int y);
	~Chest();

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

private:

	ItemID chooseRandomItem();
	Vector2 chooseRandomPlace();

	uint8_t frame;
	bool isPlayerNear;
	bool isOpen;
	bool isDropped;
	bool isOpenTexture;
	int16_t openTicks;

	ItemID droppedItem;
	Vector2 dropDest;
	Vector2 dropPos;
	Vector2 dropMove;
	bool drawColorWhenOpen = false;

	Color dropColor;


};