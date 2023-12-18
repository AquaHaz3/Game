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

	int camWidth;
	int camHeight;

	int width;
	int height;

	std::list<GameObject*> objects;

	std::list<Box2D*> boxes;
	std::list<Particle*> particles;

	bool isInit = false;

public:

	Scene();
	Scene(int width, int height);
	virtual ~Scene();

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
	virtual void OnPause(__int64 tick) { };
	virtual void PauseDraw() { };

	void moveCamera(float x, float y) {
		cam_x -= x;
		cam_y -= y;
		if (cam_x > 0) cam_x = 0;
		if (cam_y > 0) cam_y = 0;
		if (abs(cam_x) + camWidth > width) cam_x = -(width - camWidth);
		if (abs(cam_y) + camHeight > height) cam_y = -(height - camHeight);
	}

	void inline setCameraX(float x) {
		cam_x = x;
	}

	void inline setCameraY(float y) {
		cam_y = y;
	}

	void setCameraScale(float scale) { cam_scale = scale; }
	float getCameraScale() { return cam_scale; }

	/* 
	Перемещает камеру так, чтобы объект был виден 
	(А также центрирует, отн. borderW и borderH)
	@param box - объект, за которой последует камера
	@param borderW - достаточный оступ от горизонтального края камеры
	@param borderР - достаточный оступ от вертикального края камеры
	*/
	void bindCamera(AABB* box, int borderW, int borderH);

	Player* player;
	std::vector<Entity*>& getPlayerContainer();

	Morphism<float> cameraZoom;

	uint16_t notifications = 0;

protected:

	bool isDebugGridOn;
	Color background;

	std::vector<GameObject*> toRemove;
	std::vector<Entity*> playerContainer;

	bool isRenderTime;

	float cam_x;
	float cam_y;
	float cam_scale;

	bool isStatic;

	friend class SceneManager;

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
		if (current == nullptr) return;
		if (isPaused) {
			current->PauseDraw();
			return;
		}
		isDrawed = false;
		current->Draw();
		isDrawed = true;
	}

	static void inline UpdateCurrentScene(__int64 tick)
	{
		isReadyToPause = false;
		if (current == nullptr) return;
		if (isPaused) {
			current->OnPause(tick);
			return;
		}
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
	void ChangeSceneAndDispose(int index, Scene* construct);
	void StopAndExit();

	int AddScene(Scene* scene);

	int getCameraWidth() {
		return cameraWidth;
	}

	int getCameraHeight() {
		return cameraHeight;
	}

	void setDefaultCameraSize(int w, int h) {
		cameraWidth = w;
		cameraHeight = h;
	}

	bool ZoomCamera(bool isDouble) {

		bool a = (current->cam_scale == 1);

		if (isDouble) current->cam_scale = 2;
		else current->cam_scale = 1;

		return a && isDouble;
	}

	static Vector2 GetMouseOnWorld();
	static void ProjectToCamera(Vector2& pos);
	static void ProjectToCamera(Rectangle& rec);
	static void BeginCameraOverlay();
	static void EndCameraOverlay();
	static bool isSceneStatic();

	static void PlayerGameOver(Player* player);

	/* SIDE: BOTH */
	static std::vector<Entity*>& GetPlayers();

	static Scene* current;

private:

	SceneManager() 
	{
		current_index = 0;
		current = 0;
		isPaused = true;
		isReadyToExit = false;
		isReadyToChange = false;
		cameraWidth = 1024;
		cameraHeight = 640;
	}

	std::vector<SceneRef> scenes;
	int current_index;

	static bool isPaused;
	static bool isReadyToPause;
	static bool isDrawed;

	bool isReadyToChange;
	bool isReadyToExit;

	int cameraWidth;
	int cameraHeight;

	static bool isCameraOverlay;

	void pauseAll();
	void change();

};

