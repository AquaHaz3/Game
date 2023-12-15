#pragma once

#include "Event.h"

class SceneStartEvent : public Event {

public:

	MAKE_CLASS_UNIQUE;

	int index;

	SceneStartEvent(int index) : Event(GET_CLASS_UUID()) {
		this->index = index;
	}

};