#pragma once

#include "../events/Event.h"
#include <cstdint>

class GameObject
{

public:

	virtual void Draw() = 0;
	virtual void Update(__int64 tick) = 0;
	virtual void OnEvent(Event* event) = 0;

	uint8_t flags = 0;

};

#define STUB_OBJECT 0b00000001
#define ENTITY_OBJECT 0b00000100
#define SOLID_OBJECT 0b00001000



