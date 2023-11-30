#include "Scene.h"

static Color COLOR_GRID = {50,50,50,255};

Scene::Scene() {
	objects = std::vector<GameObject*>();
	player = 0;
}

Scene::Scene(int width, int height)
{
	objects = std::vector<GameObject*>();
	this->width = width;
	this->height = height;
	background = DARKGRAY;
}

void Scene::addPlayerToScene(Player* player)
{
	this->player = player;
	objects.push_back(player);
}

void Scene::addObjectToScene(GameObject* obj)
{
	objects.push_back(obj);
	if (obj->flags & SOLID_OBJECT) boxes.push_back((Box2D*)obj);
}

void Scene::Draw()
{

	ClearBackground(background);

	if (isDebugGridOn) {
		for (int i = 0; i < height/ 32; i++) {
			DrawLine(0, i*32, width, i*32, COLOR_GRID);
		}
		for (int i = 0; i < width/ 32; i++) {
			DrawLine(i*32, 0, i*32, height, COLOR_GRID);
		}
	}

	for (auto obj : objects) {
		obj->Draw();
	}
}

#include "AABB.h"

void Scene::Update(__int64 tick)
{
	for (auto obj : objects) {
		obj->Update(tick);
	}
}

void Scene::setDebugGrid(bool isActive)
{
	isDebugGridOn = isActive;
}

Scene::~Scene() 
{
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] != nullptr) {
			delete objects[i];
			objects[i] = nullptr;
		}
	}
	player = 0;
};
