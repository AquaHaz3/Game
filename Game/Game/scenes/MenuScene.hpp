#pragma once

#include "../core/Scene.h"
#include "../model/ui/ButtonUI.hpp"

static Color MenuButtonColor = { 40, 40, 40, 255 };

class MenuScene : public Scene {

public:

	MenuScene(int w, int h) : Scene(w, h) {};

	virtual void OnStart() override
	{
		SceneManager::addObject(
			new ButtonUI(width/2 - 100, 200, 200, 64, MenuButtonColor, "Start",
			[]() { // При нажатии 'Start'
				SceneManager::Instance()->ChangeScene(1);
			}
		));
		
		SceneManager::addObject(
			new ButtonUI(width / 2 - 100, 296, 200, 64, MenuButtonColor, "Scene Editor",
			[]() { // При нажатии 'Scene Editor'
				SceneManager::Instance()->ChangeScene(2);
			}
		));

		SceneManager::addObject(
			new ButtonUI(width / 2 - 100, 392, 200, 64, MenuButtonColor, "Exit Game",
			[]() { // При нажатии 'ExitGame'
				SceneManager::Instance()->StopAndExit();
			}
		));
	};

};