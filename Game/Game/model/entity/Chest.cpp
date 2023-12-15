#include "Chest.h"
#include "../../core/Scene.h"
#include <raymath.h>
#include "../../core/Debug.h"

#include "../../events/ChestEvents.hpp"
#include "../../events/EventBus.hpp"
#include "../../events/SceneStartEvent.hpp"

Chest::Chest()
	: Entity(0, 0, 32, 32, EntityID::Chest)
{
	openTicks = -1;
	frame = 0;
}

Chest::Chest(int x, int y)
	: Entity(x, y, 32, 32, EntityID::Chest)
{
	texture = Sprite(textures[(int) EntityID::Chest]);
	if (!texture.isTiled) {
		texture.addTile(0,0,32,32);
		texture.addTile(32,0,32,32);
		texture.addTile(64,0,32,32);
		texture.addTile(96,0,32,32);
	}

	EventBus::getInstance()
		->registerEventListner(SceneStartEvent::getClassUUID(), this);

	openTicks = -1;
	isOpenTexture = false;
	isOpen = false;
	isDropped = false;
	frame = 0;
}

void Chest::Draw()
{
	texture.DrawTile(aabb.min.x, aabb.min.y, frame);
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, RED);
	}
	if (isPlayerNear && isOpen == false) {
		DrawText("Press 'E' to open", aabb.min.x - 32, aabb.min.y - 16, 10, WHITE);
	}
	if (isOpen && !isDropped && isOpenTexture) {
		if (dropColor.a > 0 && drawColorWhenOpen) {
			DrawTexture(Item::textures[(int)ItemID::SPOTLIGHT], dropPos.x, dropPos.y, dropColor);
		}
		DrawTexture(Item::textures[(int)droppedItem], dropPos.x, dropPos.y, WHITE);
	}
}

#include "../ItemEntity.h"
#include "../ui/SoundUI.h"

void Chest::Update(__int64 tick)
{

	bool checkForPlayer = false;
	auto players = SceneManager::GetPlayers();
	Entity* client = 0;
	float d = 0;

	for (auto player : players) {
		Vector2 pos = player->getPos();
		d = Vector2Distance(aabb.min, pos);
		if (d < 64) {
			checkForPlayer = true;
			client = player;
			break;
		}
	}

	if (!isPlayerNear && checkForPlayer) {
		isPlayerNear = true;
		SceneManager::current->cameraZoom.Notify();
	}

	if (!checkForPlayer && isPlayerNear && tick % 60 == 0 && isDropped == false) {
		SceneManager::current->cameraZoom.Notify();
		isPlayerNear = false;
	}

	if (IsKeyDown(KEY_E) && tick % 5 == 0 && isPlayerNear && isOpen == false) {
		auto event = new ChestOpenEvent();
		if(client != nullptr) client->OnEvent(event);
		delete event;
		isOpen = true;
		dropDest = chooseRandomPlace();
		dropPos = aabb.min;
		float dist = Vector2Distance(dropDest, dropPos);
		auto z = Vector2MoveTowards(dropPos, dropDest, dist / 64);
		dropMove = Vector2Subtract(z, dropPos);
		droppedItem = chooseRandomItem();
		openTicks = 128;
		frame = 1;
		printf("Chest::Drop[Id = %d]\n", (int)droppedItem);
	}

	if (openTicks > 0 && tick % 5 == 1) {
		openTicks--;
		frame = 4 - openTicks / 32;
		if (frame >= 4) frame = 3;
		if (frame < 0) frame = 0;
		if (openTicks < 80 && openTicks > 16) {
			if (isOpenTexture == false) {
				SoundUI::PlayOnce("chest_open");
			}
			isOpenTexture = true;
			dropPos.x += dropMove.x;
			dropPos.y += dropMove.y;
		}
	}
	if (openTicks == 0) {
		isDropped = true;
		auto event = new ChestDropEvent();
		if (client != nullptr) client->OnEvent(event);
		delete event;
		SceneManager::addObject(
			new ItemEntity(dropDest.x, dropDest.y, 32, 32, droppedItem, dropColor)
		);
		SceneManager::current->cameraZoom.toOrigin();
		openTicks = -1;
		frame = 3;
	}
}

#include "../../core/SceneFile.h"

void Chest::OnEvent(Event* event)
{
	if (event->uuid == SceneStartEvent::getClassUUID()) {
		auto startEvent = (SceneStartEvent*)event;
		if (startEvent->index == 1) {
			flags = flags & (~SOLID_OBJECT);
		}
		printf("Chest::Event[SceneStartEvent] {index = %d}\n", startEvent->index);
	}
}

ItemID Chest::chooseRandomItem()
{
	int min = SceneFile::getObjectDefaultID(SceneObjectType::ITEM_ENTITIY);
	int max = SceneFile::getObjectLastID(SceneObjectType::ITEM_ENTITIY);

	int index = (rand() % (max - min)) + min; 
	dropColor = { 0,0,0,0 };

	if ((ItemID)index == ItemID::POTION_HEAL) {
		dropColor = RED;
		drawColorWhenOpen = true;
	}
	if ((ItemID)index == ItemID::POTION_XP) {
		dropColor = BLUE;
		drawColorWhenOpen = true;
	}

	if (Item::isWeaponItem(index)) {
		WLevel level = (WLevel) Item::weapons[index].level;
		if (level == WLevel::UNUSUAL) dropColor = BLUE;
		if (level == WLevel::RARE) dropColor = PURPLE;
		if (level == WLevel::LEGENDARY) dropColor = RED;
	}

	return (ItemID) index;
}

Vector2 Chest::chooseRandomPlace()
{
	std::vector<Vector2> places = std::vector<Vector2>();
	places.push_back({ aabb.min.x - 32, aabb.min.y });
	places.push_back({ aabb.min.x + 32, aabb.min.y });
	places.push_back({ aabb.min.x + 32, aabb.min.y - 32});
	places.push_back({ aabb.min.x, aabb.min.y - 32});
	places.push_back({ aabb.min.x - 32, aabb.min.y - 32});
	places.push_back({ aabb.min.x + 32, aabb.min.y + 32});
	places.push_back({ aabb.min.x, aabb.min.y + 32});
	places.push_back({ aabb.min.x - 32, aabb.min.y + 32});

	int start = rand() % 8;
	int temp = start;
	do {
		temp = (temp + 1) % 8;
		for (auto solid : SceneManager::current->boxes) { // Проходим по всем твёрдым предметам
			if (solid->flags & ENTITY_OBJECT) continue;
			if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
				break;
			}
		}
	} while (temp != start);

	return places[temp];
}
