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

#include "../events/SceneStartEvent.hpp"
#include "../events/EventBus.hpp"

class GameScene : public Scene {

public:

	GameScene(int w, int h, std::string name) : Scene(w, h)
	{
		this->name = name;
		printf("[GameScene] Construct!\n");
	}

	~GameScene() {
		printf("[GameScene] Deconstruct!\n");
	}

	virtual void OnStart() override
	{

        printf("[SceneManager] Load 'GameScene'\n");

        SceneFile file = SceneFile(name); // Берём сцену из сохранённого файла ( ..\data\base.scene )
        file.InitScene(this);

		cam_x = 0;
		cam_y = 0;

		auto startEvent = new SceneStartEvent(1);
		EventBus::getInstance()->post(startEvent);
		delete startEvent;

		isRoomChange = false;
       
	};

	virtual void AfterUpdate(__int64 tick) override {

		static int testRoom = 0;

		if (tick % 3 == 0) {
			cameraZoom.Update();
		}
		if (isGameOver) {
			SceneManager::Instance()->ChangeScene(0);
		}
		if (name == "test.scene" && tick % 100 == 0) {
			if (player != nullptr) {
				if (player->xp < 100) {
				player->xp = std::min(player->xp + 2, 200);
				}
			}
		}
		if (IsKeyDown(KEY_T) && tick % 5 == 0 && testRoom == 0) {
			isRoomChange = true;
			if (name == "base.scene") { name = "test.scene"; }
			else { name = "base.scene"; }
			SceneManager::Instance()->ChangeScene(1);
			testRoom = 200;
		}
		if (testRoom > 0) testRoom--;
	}

	virtual void OnDispose() override {
		isGameOver = false;
		for (auto obj : objects) {
			if (obj != player) {
				delete obj;
			}
		}
		printf("[Memory] Delete %d objects\n\n", (int)objects.size());
		objects.clear();
		particles.clear();
		boxes.clear();
		toRemove.clear();
		if (player != nullptr) {
			delete player;
			player = nullptr;
		}
	}

	virtual void OnPause(__int64 tick) override {

		static int enter = 0;

		if (IsKeyPressed(KEY_ENTER) && enter == 0) {
			enter = 100;
			isGameOver = true;
			printf("Try to go to menu scene...\n");
			SceneManager::Instance()->Resume();
		}

		if(tick % 4 == 0 && enter > 0) enter--;

	}

	virtual void PauseDraw() override {
		if (isRoomChange) return;
		DrawRectangle(camWidth / 2 - 128, camHeight / 2 - 32, 256, 64, { 50, 50, 50, 90 });
		DrawText("GAME OVER", camWidth / 2 - 60, camHeight / 2 - 8, 20, WHITE);
		DrawText("Press 'Enter' to continue", camWidth / 2 - 60, camHeight / 2 + 48, 10, WHITE);
	}

private:

	std::string name;

	bool isGameOver = false;
	bool isRoomChange = false;

};