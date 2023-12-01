#pragma once

#include "Entity.h"
#include "Item.h"

class ItemEntity : public Entity
{

public:

	char id;

public:

	ItemEntity() : Entity(0, 0, 0, 0, EntityType::Item) { id = 0; };
	ItemEntity(int x, int y, ItemID id);

	virtual void Draw() override;

};

