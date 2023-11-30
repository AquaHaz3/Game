#pragma once

#include "core/Scene.h"
#include <vector>
#include <memory>

typedef std::shared_ptr<Scene> SceneRef;

class GameLauncher
{
public:

	GameLauncher();

	void Launch();
	void OnStart();

	void draw();
	void update();

	bool isGameAlive;
	static SceneRef current_scene;

private:
	

};

