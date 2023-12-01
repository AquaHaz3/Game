#include "EventBus.hpp"

EventBus* EventBus::__defaultEventBus = 0;

EventBus::EventBus()
{
	subscribers = std::map<EventID, std::vector<GameObject*>>();
}

EventBus* EventBus::registerEventListner(EventID forId, GameObject* sub)
{
	if (subscribers.count(forId) <= 0) {
		subscribers.emplace(forId, std::vector<GameObject*>());
	}
	subscribers[forId].push_back(sub);
	return __defaultEventBus;
}

void EventBus::unregisterEventListner(EventID forId, GameObject* listener)
{
	if (subscribers.count(forId) <= 0) {
		return;
	}

	int deleteIndex = 0;

	for (int i = 0; i < subscribers[forId].size(); i++) {
		if (subscribers[forId][i] == listener) {
			deleteIndex = i;
			break;
		}
	}

	subscribers[forId].erase(subscribers[forId].begin() + deleteIndex);
}


void EventBus::post(Event* event)
{
	if (subscribers.count(event->uuid) <= 0) {
		return;
	}
	for (int i = 0; i < subscribers[event->uuid].size(); i++) {
		subscribers[event->uuid][i]->OnEvent(event);
	}
}