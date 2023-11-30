#pragma once

#include <vector>
#include "GameObject.h"
#include "Box2D.h"

class Scene
{

public:

	int width;
	int height;

	std::vector<GameObject*> objects;
	std::vector<Box2D*> boxes;

public:

	Scene();
	Scene(int width, int height);
	~Scene();

	void addObjectToScene(GameObject* obj);

private:

};

