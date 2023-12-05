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

	EditorUI(int width, PrototypeGameObject* brush)
	{
		this->width = width;
		this->brush = brush;
		left_arrow = SpriteLoader::GetSprite("ui_left.png");
		right_arrow = SpriteLoader::GetSprite("ui_right.png");
	};

	virtual void Draw() {

		rlPushMatrix();
		rlTranslatef(0, -height, 0);
		DrawRectangle(48, 0, width-96, 48, EditorBack);
		DrawRectangleLines(48, 0, width-96, 48, EditorLine);


		left_arrow.Draw({ 64, 8 });
		right_arrow.Draw({ 144, 8 });
		if (brush->type >= (int)SceneObjectType::BACKGROUND 
			&& brush->type <= (int)SceneObjectType::BLOCK) {
			DrawTexture(Block::textures[brush->ord], 104, 8, WHITE);
		}
		if (brush->type == (int) SceneObjectType::ITEM_ENTITIY) {
			DrawTexture(Item::textures[brush->ord], 104, 8, WHITE);
		}
		if (brush->type == (int) SceneObjectType::PLAYER) {
			SpriteLoader::GetSprite("player.png").DrawPro( 104, 8, 32, 32, 0, 0, 0);
		}

		BeginBlendMode(BLEND_SUBTRACT_COLORS);
		DrawRectangleLines(104, 8, 32, 32, WHITE);
		EndBlendMode();
		DrawText(aboutId.c_str(), 180, 8, 10, WHITE);
		left_arrow.Draw({ 256, 8 });
		right_arrow.Draw({ 448, 8 });
		DrawText(aboutType.c_str(), 304, 14, 20, WHITE);

		rlPopMatrix();
	}

	virtual void Update(__int64 tick)
	{
		if (tick % 3 == 2) {
			Vector2 mouse = GetMousePosition();
			int alpha = std::min((int) (255 - (200 - mouse.y * 2)), 255);
			EditorBack.a = alpha;
			EditorLine.a = alpha;
			height = std::max((int)(64 - mouse.y), 0);
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

	int width;
	int height;

	Sprite left_arrow;
	Sprite right_arrow;

	std::string aboutId;
	std::string aboutType;
	
	PrototypeGameObject* brush;

};