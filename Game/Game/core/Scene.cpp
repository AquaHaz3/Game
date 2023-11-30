#include "Scene.h"

Scene::Scene() {
	objects = std::vector<GameObject*>();
}

Scene::Scene(int width, int height)
{
	objects = std::vector<GameObject*>();
	this->width = width;
	this->height = height;
}

void Scene::addObjectToScene(GameObject* obj)
{
	objects.push_back(obj);
	if (obj->flags & SOLID_OBJECT) boxes.push_back((Box2D*)obj);
}

Scene::~Scene() 
{
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] != nullptr) {
			delete objects[i];
			objects[i] = nullptr;
		}
	}
};
