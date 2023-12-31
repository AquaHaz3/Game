#pragma once

#include "Entity.h"
#include "Item.h"

class ItemEntity : public Entity
{

public:

	uint8_t id;

public:

	ItemEntity();
	ItemEntity(int x, int y, int w, int h, ItemID id);
	ItemEntity(int x, int y, int w, int h, ItemID id, Color light);

	virtual void Draw() override;

private:

	char x_offset;
	char y_offset;
	Color tint;

};

