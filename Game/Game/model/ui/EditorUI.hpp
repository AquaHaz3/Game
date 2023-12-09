#pragma once

#include "../../core/GameObject.h"
#include <raylib.h>
#include "../../core/Sprite.h"
#include "../../core/SceneFile.h"

#include <functional>
#include <rlgl.h>

static Color EditorBack = { 100,100,100, 250 };
static Color EditorLine = { 70,70,70, 250 };

static inline void __between(int* v, int min, int max) {
	*v = std::min(std::max(*v, min), max);
}

static void __choose_ranges(int* v, int from, int to) 
{
	if (*v < from) *v = to - 1;
	if (*v >= to) *v = from;
}

class EditorUI : public GameObject
{

public:

	EditorUI(int camWidth, PrototypeGameObject* brush)
	{
		this->camWidth = camWidth;
		this->brush = brush;
		left_arrow = SpriteLoader::GetSprite("ui_left.png");
		right_arrow = SpriteLoader::GetSprite("ui_right.png");
	};

	virtual void Draw() {

		Vector2 editBar = { 48, 0 };
		Vector2 toolPos = { 64, 8 };
		SceneManager::ProjectToCamera(editBar);
		SceneManager::ProjectToCamera(toolPos);

		rlPushMatrix();
		rlTranslatef(0, -camHeight, 0);
		DrawRectangle(editBar.x, editBar.y, camWidth-96, 48, EditorBack);
		DrawRectangleLines(editBar.x, editBar.y, camWidth-96, 48, EditorLine);


		left_arrow.Draw({ toolPos.x, toolPos.y }); toolPos.x += 80;
		right_arrow.Draw({ toolPos.x, toolPos.y }); toolPos.x -= 40;

		if (brush->type >= (int)SceneObjectType::BACKGROUND 
			&& brush->type <= (int)SceneObjectType::BLOCK) {
			DrawTexture(Block::textures[brush->ord], toolPos.x, toolPos.y, WHITE);
		}

		if (brush->type == (int) SceneObjectType::ITEM_ENTITIY) {
			DrawTexture(Item::textures[brush->ord], toolPos.x, toolPos.y, WHITE);
		}
		if (brush->type == (int) SceneObjectType::PLAYER) {
			SpriteLoader::GetSprite("player.png").DrawPro(toolPos.x, toolPos.y, 32, 32, 0, 0, 0);
		}

		BeginBlendMode(BLEND_SUBTRACT_COLORS);
		DrawRectangleLines(toolPos.x, toolPos.y, 32, 32, WHITE);
		EndBlendMode();
		toolPos.x += 76;
		DrawText(aboutId.c_str(), toolPos.x, toolPos.y, 10, WHITE);
		toolPos.x += 76;
		left_arrow.Draw({ toolPos.x, toolPos.y }); toolPos.x += 192;
		right_arrow.Draw({ toolPos.x, toolPos.y }); toolPos.x -= 144;
		DrawText(aboutType.c_str(), toolPos.x, toolPos.y + 6, 20, WHITE);

		rlPopMatrix();
	}

	virtual void Update(__int64 tick)
	{
		if (tick % 3 == 2) {
			Vector2 mouse = GetMousePosition();
			int alpha = std::min((int) (255 - (200 - mouse.y * 2)), 255);
			EditorBack.a = alpha;
			EditorLine.a = alpha;
			camHeight = std::max((int)(64 - mouse.y), 0);
		}
		if (tick % 3 == 1) {
			aboutId = "Id = " + std::to_string(brush->ord);
			aboutType = SceneFile::getTypeName((SceneObjectType) brush->type);
		}
		if (tick % 3 == 0) {
			float wheel = GetMouseWheelMove();
			if (wheel > 0) brush->ord++;
			if (wheel < 0) brush->ord--;
			if (abs(wheel) > 0.1) {
				int defaultId = SceneFile::getObjectDefaultID((SceneObjectType)brush->type);
				int lastId = SceneFile::getObjectLastID((SceneObjectType)brush->type);
				__choose_ranges((int*)&brush->ord, defaultId, lastId);
			}
		}
		if (tick % 4 == 0) {
			int temp = brush->type;
			if (IsKeyPressed(KEY_RIGHT) && brush->type < (int) SceneObjectType::PLAYER) brush->type++;
			if (IsKeyPressed(KEY_LEFT) && brush->type > (int) SceneObjectType::BACKGROUND) brush->type--;
			if (brush->type != temp) {
				brush->ord = SceneFile::getObjectDefaultID((SceneObjectType)brush->type);
			}
		}
	}

	virtual void OnEvent(Event* event) {

	};

private:

	int camWidth;
	int camHeight;

	Sprite left_arrow;
	Sprite right_arrow;

	std::string aboutId;
	std::string aboutType;
	
	PrototypeGameObject* brush;

};