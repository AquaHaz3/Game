#pragma once

#include "../core/GameObject.h"
#include <map>
#include <vector>


class EventBus {

public:

	static EventBus* getInstance() {
		if (__defaultEventBus == 0)
		{
			__defaultEventBus = new EventBus();
		}
		return __defaultEventBus;
	}

	EventBus* registerEventListner(EventID forId, GameObject* subscriber);
	void unregisterEventListner(EventID forId, GameObject* subscriber);

	void post(Event* event);

private:

	EventBus();

	static EventBus* __defaultEventBus;

	std::map<EventID, std::vector<GameObject*>> subscribers;

};

static inline EventBus* getEventBus()
{
	return EventBus::getInstance();
}