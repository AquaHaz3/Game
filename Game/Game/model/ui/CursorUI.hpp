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
		this->color = { 0,0,0,0 };
		pos = { 0, 0, 32, 32 };
		posData = "";
		selectionData = "";
		selectionDelay = 0;
		this->onClick = 0;
		this->onSelect = 0;
		isSelection = 0;
		select = { 0,0,0,0 };
	};

	CursorUI(bool isGrid, Color color,
		std::function<void(char btn, int x, int y)> onClick, std::function<void(char btn)> onSelect
	) {
		this->color = color;
		pos = { 0, 0, 32, 32 };
		this->isGrid = isGrid;
		posData = "";
		selectionData = "";
		selectionDelay = -45;
		this->onClick = onClick;
		this->onSelect = onSelect;
		isSelection = 0;
		select = { 0,0,0,0 };
	};

	virtual void Draw() {

		DrawRectangleLinesEx(pos, 1, color);

		if (isSelection == 1) {
			DrawRectangleLinesEx(select, 1, ORANGE);
			DrawRectangleRec(select, { 255, 161, 0, 60 });
			DrawText(selectionData.c_str(), pos.x + 32, pos.y + 32, 10, WHITE);
		}

		if (isSelection == 2) {
			DrawRectangleLinesEx(select, 1, RED);
			DrawRectangleRec(select, {230, 41, 55, 60 });
		}
		
		if (isSelection == 0) {
			DrawText(posData.c_str(), pos.x + 32, pos.y + 32, 10, WHITE);
		}
		else {
			//DrawText(selectionData.c_str(), pos.x + 32, pos.y + 32, 10, WHITE);
		}

	}

	virtual void Update(__int64 tick) 
	{
		Vector2 mouse = SceneManager::GetMouseOnWorld();
		if (tick % 2 == 0) {
			posData = "(" + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y) + ")";
			selectionData = "(" + std::to_string((int)select.width/32) + ", " + std::to_string((int)select.height / 32) + ")";
		}
		if (isGrid)
		{
			pos.x = (int(mouse.x / 32)) * 32;
			pos.y = (int(mouse.y / 32)) * 32;

			bool isLeft = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
			bool isRight = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

			if (isLeft ^ isRight) {
				
				if (selectionDelay == 1) {
					startSelectPos.x = pos.x;
					startSelectPos.y = pos.y;
					if(isLeft) onClick(MOUSE_BUTTON_LEFT, pos.x, pos.y);
					else onClick(MOUSE_BUTTON_RIGHT, pos.x, pos.y);
				}
				if(selectionDelay < 55) selectionDelay++;
				if (selectionDelay > 50 && selectionDelay < 60) {
					isSelection = 1 + (char)isRight;
					selectionDelay = 61;
				}
			}

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				if(isSelection > 0)
					onSelect(MOUSE_BUTTON_LEFT);
				isSelection = 0;
				selectionDelay = 0;
			}

			if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
				if (isSelection > 0)
					onSelect(MOUSE_BUTTON_RIGHT);
				isSelection = 0;
				selectionDelay = 0;
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
	char isSelection;
	Vector2 startSelectPos;
	int selectionDelay;

	std::string posData;
	std::string selectionData;

	std::function<void(char btn, int x, int y)> onClick;
	std::function<void(char button)> onSelect;

};