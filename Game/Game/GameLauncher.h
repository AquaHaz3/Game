#pragma once

#include "core/Scene.h"
#include <vector>

class GameLauncher
{
public:

	GameLauncher();

	void Launch();

private:

	Scene current_scene;

	void draw();
	void update();
		
};

