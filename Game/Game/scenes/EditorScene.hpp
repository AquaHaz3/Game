#pragma once

#include "../core/Scene.h"
#include "../model/ui/ButtonUI.hpp"
#include "../core/SceneFile.h"
#include "../model/ui/CursorUI.hpp"

class EditorScene : public Scene {

public:

	EditorScene(int w, int h) : Scene(w, h)
	{
		current_object.type = (int)SceneObjectType::WALL;
		current_object.id = 0;
		current_object.ord = (int) BlockID::DARK_BRICK;
		context = SceneFile();
		setDebugGrid(true);
	};

	virtual void OnStart() override
	{
		cursor = new CursorUI(true, RED);
		SceneManager::addObject(cursor);
	};

private:

	PrototypeGameObject current_object;
	SceneFile context;
	CursorUI* cursor;

};