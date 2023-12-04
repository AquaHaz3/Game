#pragma once

#include "../../core/GameObject.h"
#include <raylib.h>
#include "../../core/Sprite.h"
#include "../../core/SceneFile.h"

#include <functional>
#include <rlgl.h>

static Color EditorBack = { 100,100,100, 250 };
static Color EditorLine = { 70,70,70, 250 };

static inline void __between(int* v, int max, int min) {
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
			|| brush->type <= (int)SceneObjectType::BLOCK) {
			DrawTexture(Block::textures[brush->ord], 104, 8, WHITE);
		}

		BeginBlendMode(BLEND_SUBTRACT_COLORS);
		DrawRectangleLines(104, 8, 32, 32, EditorLine);
		EndBlendMode();
		DrawText(aboutBlock.c_str(), 180, 8, 10, WHITE);

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
			aboutBlock = "Id = " + std::to_string(brush->ord) + ";";
		}
		if (tick % 2 == 0) {
			float wheel = GetMouseWheelMove();
			if (wheel > 0) brush->ord++;
			if (wheel < 0) brush->ord--;
			if (abs(wheel) > 0.1) {
				if (brush->type >= (int)SceneObjectType::BACKGROUND) {
					__choose_ranges((int*)&brush->ord, (int)1, (int)BlockID::__lastBlock);
				}
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

	std::string aboutBlock;
	
	PrototypeGameObject* brush;

};