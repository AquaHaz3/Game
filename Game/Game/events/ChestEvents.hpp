#pragma once

#include "Event.h"

class ChestOpenEvent : public Event {

public:

	MAKE_CLASS_UNIQUE;

	ChestOpenEvent() : Event(GET_CLASS_UUID()) {}

};

class ChestDropEvent : public Event {

public:

	MAKE_CLASS_UNIQUE;

	ChestDropEvent() : Event(GET_CLASS_UUID()) {}

};