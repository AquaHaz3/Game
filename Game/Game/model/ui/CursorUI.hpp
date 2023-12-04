#pragma once

#include "../../core/GameObject.h"
#include <raylib.h>
#include "../../core/Sprite.h"

#include <functional>

class CursorUI : public GameObject
{

public:

	CursorUI()
	{
		this->isGrid = false;
	};

	CursorUI(bool isGrid, Color color,
		std::function<void(char btn)> onClick, std::function<void(char btn)> onSelect
	) {
		this->color = color;
		pos = { 0, 0, 32, 32 };
		this->isGrid = isGrid;
		posData = "";
		selectionData = "";
		selectionDelay = -10;
		this->onClick = onClick;
		this->onSelect = onSelect;
	};

	virtual void Draw() {

		DrawRectangleLinesEx(pos, 1, color);
		if (isSelection) {
			DrawRectangleLinesEx(select, 1, ORANGE);
			DrawRectangleRec(select, { 255, 161, 0, 60 });
		}
		
		if (isSelection == false) {
			DrawText(posData.c_str(), pos.x + 32, pos.y + 32, 10, WHITE);
		}
		else {
			//DrawText(selectionData.c_str(), pos.x + 32, pos.y + 32, 10, WHITE);
		}

	}

	virtual void Update(__int64 tick) 
	{
		Vector2 mouse = GetMousePosition();
		if (tick % 2 == 0) {
			posData = "(" + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y) + ")";
			selectionData = "";
		}
		if (isGrid)
		{
			pos.x = (int(mouse.x / 32)) * 32;
			pos.y = (int(mouse.y / 32)) * 32;

			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				if (selectionDelay >= 1 && selectionDelay < 3) {
					startSelectPos.x = pos.x;
					startSelectPos.y = pos.y;
				}
				if(selectionDelay < 55) selectionDelay++;
				if (selectionDelay > 50 && selectionDelay < 60) {
					isSelection = true;
					selectionDelay = 61;
				}
			}

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				isSelection = false;
				selectionDelay = 0;
				onSelect(MOUSE_BUTTON_LEFT);
			}

			if (isSelection) {
				if (pos.x < startSelectPos.x) {
					select.x = pos.x;
					select.width = abs(startSelectPos.x - pos.x + 32);
				}
				else {
					select.x = startSelectPos.x;
					select.width = abs(startSelectPos.x - pos.x - 32);
				}
				if (pos.y < startSelectPos.y) {
					select.y = pos.y;
					select.height = abs(startSelectPos.y - pos.y + 32);
				}
				else {
					select.y = startSelectPos.y;
					select.height = abs(startSelectPos.y - pos.y - 32);
				}
			}
		}
		else {
			memcpy(&pos.x, &mouse.x, sizeof(float) * 2);
		}
	}

	virtual void OnEvent(Event* event) {
	
	};

	Color color;
	Rectangle select;
	Rectangle pos;

private:

	bool isGrid;
	bool isSelection;
	Vector2 startSelectPos;
	int selectionDelay;

	std::string posData;
	std::string selectionData;

	std::function<void(char button)> onClick;
	std::function<void(char button)> onSelect;

};