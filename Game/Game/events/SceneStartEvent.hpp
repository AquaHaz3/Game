#pragma once

#include "Event.h"

class SceneStartEvent : public Event {

	MAKE_CLASS_UNIQUE;

public:

	SceneStartEvent() : Event(GET_CLASS_UUID()) {

	}

};

RESOLVE_UNIQUE_SYMBOL(SceneStartEvent);