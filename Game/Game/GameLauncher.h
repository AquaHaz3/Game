#pragma once

#include "core/Scene.h"
#include <vector>
#include <memory>


class GameLauncher
{
public:

	GameLauncher();

	void Launch();
	void OnStart();

	void draw();
	void update();

	bool isGameAlive;

private:
	

};

