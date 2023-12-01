#include "Scene.h"
#include "../core/Debug.h"

static Color COLOR_GRID = {50,50,50,255};

Scene::Scene() 
{
	objects = std::list<GameObject*>();
	boxes = std::list<Box2D*>();
	toRemove = std::vector<GameObject*>();
	player = 0;
}

Scene::Scene(int width, int height)
{
	objects = std::list<GameObject*>();
	boxes = std::list<Box2D*>();
	toRemove = std::vector<GameObject*>();
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


	if (IsKeyPressed(KEY_SPACE)) {
		debug_util::switchDebugBoxes();
	}

	if (isDebugGridOn) {
		for (int i = 0; i < height/ 32; i++) {
			DrawLine(0, i*32, width, i*32, COLOR_GRID);
		}
		for (int i = 0; i < width/ 32; i++) {
			DrawLine(i*32, 0, i*32, height, COLOR_GRID);
		}
	}

	DrawText("Press <space> to show collision boxes", 10, 10, 10, WHITE);

	if (toRemove.size() > 0) {
		isRenderTime = false;
		return;
	}

	isRenderTime = true;

	for (auto obj : objects) {
		obj->Draw();
	}

	isRenderTime = false;
}

#include "AABB.h"

void Scene::Update(__int64 tick)
{

	for (auto obj : objects) {
		obj->Update(tick);
	}

	if (toRemove.size() > 0 && isRenderTime == false) {
		for (auto to_r : toRemove) 
		{
			objects.remove(to_r);
			if(to_r->flags & SOLID_OBJECT) boxes.remove((Box2D*) to_r);
			delete to_r;
		}
		toRemove.clear();
		//toRemove.shrink_to_fit();
	}
}

void Scene::setDebugGrid(bool isActive)
{
	isDebugGridOn = isActive;
}

void Scene::removeObject(GameObject* obj)
{
	toRemove.push_back(obj);
}

Scene::~Scene() 
{
	for (auto obj : objects) {
		delete obj;
	}
	objects.clear();
	player = 0;
};
