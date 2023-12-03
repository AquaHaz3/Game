#pragma once

#include <vector>
#include <list>
#include "GameObject.h"
#include "Box2D.h"
#include "../model/Player.h"
#include "Particle.hpp"

class Scene
{

public:

	int width;
	int height;

	std::list<GameObject*> objects;
	std::list<Box2D*> boxes;
	std::list<Particle*> particles;

public:

	Scene();
	Scene(int width, int height);
	~Scene();

	void addPlayerToScene(Player* player);
	void addObjectToScene(GameObject* obj);
	void setDebugGrid(bool isActive);

	void removeObject(GameObject* obj);

	void addParticle(Particle* particle);
	
	void Draw();
	void Update(__int64 tick);

	PlayerRef player;

private:

	bool isDebugGridOn;
	Color background;

	std::vector<GameObject*> toRemove;

	bool isRenderTime;

};

