#pragma once

#include "../core/Scene.h"
#include "../core/SceneFile.h"
#include "model/Player.h"
#include "model/Block.h"
#include "model/Item.h"
#include "model/entity/Arrow.h"
#include "model/Wall.h"
#include "model/ItemEntity.h"
#include "model/Background.h"
#include <string>

class GameScene : public Scene {

public:

	GameScene(int w, int h, std::string name) : Scene(w, h)
	{
		this->name = name;
	}

	virtual void OnStart() override
	{

        printf("[SceneManager] Load 'GameScene'\n");

        SceneFile file = SceneFile(name); // Берём сцену из сохранённого файла ( ..\data\base.scene )
        file.InitScene(this);
       
	};

	virtual void AfterUpdate(__int64 tick) override {
		if (tick % 5 == 0) {
			cameraZoom.Update();
		}
	}

private:

	std::string name;

};