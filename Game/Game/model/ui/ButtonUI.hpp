#pragma once

#include "../../core/GameObject.h"
#include "../../core/Box2D.h"
#include <raylib.h>
#include <string>
#include <functional>

class ButtonUI : public Box2D {

public:

	ButtonUI(
		int x, int y, int w, int h,
		Color color, std::string text, std::function<void()> callback
	)
		: Box2D(x, y, w, h)
	{
		this->color = color;
		this->text = text;
		this->callback = callback;

		textX = x + w / 2 - (((int)text.size()) * 10 / 2) - 5;
		textY = y + h / 2 - 10;
	}

	virtual void Draw() {
		DrawRectangle(aabb.min.x, aabb.min.y, w, h, color);
		DrawText(text.c_str(), textX, textY, 20, WHITE);
	}

	virtual void Update(__int64 tick) {

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isPressed) {
			Vector2 mouse = GetMousePosition();
			if (UtilAABB::isPointOn(mouse, &aabb)) {
				callback();
			};
			isPressed = true;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			isPressed = false;
		}

	}

	virtual void OnEvent(Event* event) {};

private:

	Color color;
	std::string text;
	std::function<void()> callback;

	int textX;
	int textY;

	bool isPressed = false;

};