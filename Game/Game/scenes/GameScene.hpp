#pragma once

#include "../core/Scene.h"
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

        printf("[SceneManager] Load 'GameScene' index 1\n");

        SceneManager::addObject(new Background(32, 32, 256, 288, BlockID::FLOOR1));
        SceneManager::addObject(new Background(32, 320, 256, 256, BlockID::GRAVEL));

        SceneManager::addObject(new Wall(32, 32, 288, 64, BlockID::DARK_BRICK));
        SceneManager::addObject(new Wall(32, 64, 64, 608, BlockID::DARK_BRICK));
        SceneManager::addObject(new Wall(32, 320, 288, 352, BlockID::DARK_BRICK));
        SceneManager::addObject(new Wall(256, 64, 288, 128, BlockID::DARK_BRICK));
        SceneManager::addObject(new Wall(256, 192, 288, 480, BlockID::DARK_BRICK));

        SceneManager::addObject(new Wall(32, 576, 288, 608, BlockID::DARK_BRICK));
        SceneManager::addObject(new Wall(256, 608, 288, 544, BlockID::DARK_BRICK));

        //current_scene->addObjectToScene(new Wall(256, 608, 288, 544, BlockID::DARK_BRICK));

        SceneManager::addObject(new ItemEntity(128, 160, 16, 22, ItemID::POTION_HEAL, RED));
        SceneManager::addObject(new ItemEntity(224, 288, 16, 22, ItemID::POTION_XP, BLUE));

        SceneManager::addObject(new ItemEntity(64, 548, 16, 22, ItemID::POTION_HEAL, RED));
        SceneManager::addObject(new ItemEntity(128, 548, 16, 22, ItemID::POTION_HEAL, RED));
        SceneManager::addObject(new ItemEntity(96, 548, 16, 22, ItemID::POTION_XP, BLUE));
        SceneManager::addObject(new ItemEntity(64, 352, 16, 22, ItemID::BOW));
        SceneManager::addObject(new ItemEntity(128, 352, 24, 24, ItemID::CROWN, ORANGE));
        SceneManager::addObject(new ItemEntity(128, 288, 24, 24, ItemID::CROWN, ORANGE));
        SceneManager::addObject(new ItemEntity(64, 288, 24, 24, ItemID::CROWN, ORANGE));

        SceneManager::addPlayer(new Player(96, 96));
	};

private:

	std::string name;

};