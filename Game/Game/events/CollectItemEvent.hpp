#pragma once

#pragma once

#include "Event.h"
#include "../model/ItemEntity.h"

class CollectItemEvent : public Event {

public:

	MAKE_CLASS_UNIQUE;
	ItemID id;

public:

	CollectItemEvent(ItemEntity* item) : Event(GET_CLASS_UUID()) {
		this->id = (ItemID) item->id;
	}

};

RESOLVE_UNIQUE_SYMBOL(CollectItemEvent);