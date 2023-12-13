#include "Chest.h"
#include "../../core/Scene.h"
#include <raymath.h>

Chest::Chest()
	: Entity(0, 0, 32, 32, EntityID::Chest)
{}

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
}

void Chest::Draw()
{
	texture.DrawTile(aabb.min.x, aabb.min.y, frame);
}

void Chest::Update(__int64 tick)
{

	bool checkForPlayer = false;
	auto players = SceneManager::GetPlayers();
	Entity* client;

	for (auto player : players) {
		Vector2 pos = player->getPos();
		float d = Vector2Distance(aabb.min, pos);
		if (d < 48) {
			checkForPlayer = true;
			client = player;
			break;
		}
	}

	if (!isPlayerNear && checkForPlayer) {
		isPlayerNear = true;
		SceneManager::current->cameraZoom.Notify();
	}

	if (!checkForPlayer && isPlayerNear) {
		SceneManager::current->cameraZoom.Notify();
		isPlayerNear = false;
	}
}

void Chest::OnEvent(Event* event)
{
}
