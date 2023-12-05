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
		brush.type = (int)SceneObjectType::WALL;
		brush.id = 0;
		brush.ord = (int) BlockID::DARK_BRICK;
		context = SceneFile();
		setDebugGrid(true);
	};

	virtual void OnStart() override
	{
		cursor = new CursorUI(true, RED,
			[this](char btn, int x, int y) { onClick(btn, x, y); },
			[this](char btn) { onSelect(btn); }
		);
		ui = new EditorUI(width, &brush);
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
		
	}

	virtual void OnDispose() override {
		delete cursor;
		delete ui;
	}

private:

	void onClick(char btn, int x, int y) 
	{
		if (brush.type == (int)SceneObjectType::PLAYER) {
			context.setPlayer(x, y);
		}
	}

	void onSelect(char btn) {

		if (brush.type == (int)SceneObjectType::PLAYER) return;
		if (brush.type == (int)SceneObjectType::ITEM_ENTITIY) return;

		brush.x = cursor->select.x; 
		brush.y = cursor->select.y; 
		brush.w = cursor->select.width;
		brush.h = cursor->select.height;

		if (brush.ord != 0 && btn == MOUSE_BUTTON_LEFT) {
			auto obj = SceneFile::brushGameObjectFactory(&brush);
			SceneManager::addObject(obj);
			auto save = new PrototypeGameObject();
			memcpy(save, &brush, sizeof(PrototypeGameObject));
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