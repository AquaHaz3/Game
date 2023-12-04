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
		current_object.ord = (int) BlockID::FLOOR2;
		context = SceneFile();
		setDebugGrid(true);
	};

	virtual void OnStart() override
	{
		cursor = new CursorUI(true, RED,
			[this](char btn) { onClick(btn); },
			[this](char btn) { onSelect(btn); }
		);
		SceneManager::addObject(cursor);
	};

private:

	void onClick(char btn) {

	}

	void onSelect(char btn) {
		SceneManager::addObject(new Background(
			(int)cursor->select.x, (int)cursor->select.y, 
			(int) cursor->select.width, (int) cursor->select.height,
			(BlockID)current_object.ord
		));
	}

	PrototypeGameObject current_object;
	SceneFile context;
	CursorUI* cursor;

};