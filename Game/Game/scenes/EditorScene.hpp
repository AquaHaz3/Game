#pragma once

#include "../core/Scene.h"
#include "../model/ui/ButtonUI.hpp"
#include "../core/SceneFile.h"
#include "../model/ui/CursorUI.hpp"
#include "../model/ui/EditorUI.hpp"

class EditorScene : public Scene {

public:

	EditorScene(int w, int h) : Scene(w, h)
	{
		brush = PrototypeGameObject();
		brush.type = (int)SceneObjectType::WALL;
		brush.id = 0;
		brush.ord = (int) BlockID::DARK_BRICK;
		context = SceneFile();
		isStatic = true;
		setDebugGrid(true);
	};

	virtual void OnStart() override
	{
		SceneFile last = SceneFile("base.scene");
		last.LoadSceneForEditor(this, &context);
		cursor = new CursorUI(true, RED,
			[this](char btn, int x, int y) { onClick(btn, x, y); },
			[this](char btn) { onSelect(btn); }
		);
		ui = new EditorUI(camWidth, &brush);
	};

	virtual void AfterDraw() override {
		Vector2 playerPos = context.getPlayerPos();
		if (playerPos.x >= 0) {
			SpriteLoader::GetSprite("player.png").DrawPro(playerPos.x, playerPos.y, 32, 32, 0, 0, 0);
		}
		cursor->Draw();
		ui->Draw();
		
	}

	virtual void AfterUpdate(__int64 tick) override {
		cursor->Update(tick);
		ui->Update(tick);

		if (tick % 3 != 0) return;

		if (IsKeyDown(KEY_W)) {
			SceneManager::current->moveCamera(0, -2);
		}
		if (IsKeyDown(KEY_S)) {
			SceneManager::current->moveCamera(0, 2);
		}
		if (IsKeyDown(KEY_A)) {
			SceneManager::current->moveCamera(-2, 0);
		}
		if (IsKeyDown(KEY_D)) {
			SceneManager::current->moveCamera(2, 0);
		}
		if (IsKeyReleased(KEY_ENTER)) {
			onSave();
		}
	}

	virtual void OnDispose() override {
		delete cursor;
		delete ui;
	}

private:

	void onClick(char btn, int x, int y) 
	{

		if (btn == MOUSE_BUTTON_RIGHT) {
			AABB eraser = {
			(float)x, (float)y,
			(float)(x + 32), (float)(y + 32)
			};
			removeObjectOnRenderScene(&eraser);
			context.removeObjectInBox(&eraser);
			return;
		}

		if (brush.type == (int)SceneObjectType::PLAYER) {
			context.setPlayer(x, y);
		}

		if (
			brush.type == (int)SceneObjectType::ITEM_ENTITIY ||
			brush.type == (int)SceneObjectType::MOB
		) {
			brush.x = x;
			brush.y = y;
			brush.w = 32;
			brush.h = 32;
			auto obj = SceneFile::brushGameObjectFactory(&brush);
			SceneManager::addObject(obj);
			auto save = new PrototypeGameObject();
			memcpy(save, &brush, sizeof(PrototypeGameObject));
			context.addObject(save);
		}
	}

	void onSelect(char btn) {

		if (brush.type == (int)SceneObjectType::PLAYER) return;
		if (brush.type == (int)SceneObjectType::ITEM_ENTITIY) return;
		if (brush.type == (int)SceneObjectType::MOB) return;

		brush.x = cursor->select.x; 
		brush.y = cursor->select.y; 
		brush.w = cursor->select.width;
		brush.h = cursor->select.height;

		if (brush.ord != 0 && btn == MOUSE_BUTTON_LEFT) {
			auto obj = SceneFile::brushGameObjectFactory(&brush);
			SceneManager::addObject(obj);
			auto save = new PrototypeGameObject();
			*save = brush;
			context.addObject(save);
		}
		else {
			AABB eraser = {
			(float)brush.x, (float)brush.y,
			(float)(brush.x + brush.w), (float)(brush.y + brush.h)
			};
			removeObjectOnRenderScene(&eraser);
			context.removeObjectInBox(&eraser);
		}
	}

	void onSave() 
	{
		printf("[EditorScene] Try to save...\n");
		context.SaveScene("base", width, height);
		printf("[EditorScene] saved...\n");
	}

	void removeObjectOnRenderScene(AABB* box) {
		
		for (auto obj : SceneManager::current->boxes) {
			if (UtilAABB::isOverlap(box, &obj->aabb)) {
				SceneManager::removeObject(obj);
			};
		}
	}

	PrototypeGameObject brush;
	SceneFile context;
	CursorUI* cursor;
	EditorUI* ui;

};