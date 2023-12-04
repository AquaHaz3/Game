#pragma once

#include <vector>
#include <list>
#include "GameObject.h"
#include "Box2D.h"
#include "../model/Player.h"
#include "Particle.hpp"
#include <memory>

class Scene
{

public:

	int width;
	int height;

	std::list<GameObject*> objects;
	std::list<Box2D*> boxes;
	std::list<Particle*> particles;

	bool isInit = false;

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
	void Dispose();

	virtual void OnStart() { isInit = true; };
	virtual void AfterDraw() { };
	virtual void AfterUpdate(__int64 tick) { };
	virtual void OnDispose() { };

	PlayerRef player;

private:

	bool isDebugGridOn;
	Color background;

	std::vector<GameObject*> toRemove;

	bool isRenderTime;

};

typedef std::shared_ptr<Scene> SceneRef;

class SceneManager
{

public:

	static SceneManager* Instance();
	static SceneRef GetCurrent();

	static SceneManager* instance;

	static void addObject(GameObject* object);
	static void addPlayer(Player* player);
	static void removeObject(GameObject* object);
	static void addParticle(Particle* object);

	static void inline DrawCurrentScene() 
	{
		if (current == nullptr || isPaused) return;
		isDrawed = false;
		current->Draw();
		isDrawed = true;
	}

	static void inline UpdateCurrentScene(__int64 tick)
	{
		isReadyToPause = false;
		if (current == nullptr || isPaused) return;
		current->Update(tick);
		if (isReadyToPause) Instance()->pauseAll();
	}

public:

	void setStartScene(SceneRef& startScene) {
		current = startScene.get();
		scenes.push_back(startScene);
	};

	void Pause();
	void Resume();
	void Start();
	void ChangeScene(int index);
	void StopAndExit();
	int AddScene(Scene* scene);

	static Scene* current;

private:

	SceneManager() 
	{
		current_index = 0;
		current = 0;
		isPaused = true;
	}

	std::vector<SceneRef> scenes;
	int current_index;

	static bool isPaused;
	static bool isReadyToPause;
	static bool isDrawed;

	bool isReadyToChange;
	bool isReadyToExit;

	void pauseAll();
	void change();

};

