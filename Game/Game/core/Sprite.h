#pragma once

#include <raylib.h>
#include <string>
#include <vector>

class Sprite
{

public:

	Sprite() {
		texture = { 0,0,0,0 };
	};

	Sprite(std::string path)
	{
		texture = LoadTextureFromResources(path);
		tile = std::vector<Rectangle>();
	};

	void addTile(int x, int y, int w, int h) {
		Rectangle region{ (float) x, (float)y,(float)w, (float)h };
		tile.push_back(region);
	}

	void Draw(Vector2 pos) {
		DrawTextureV(texture, pos, WHITE);
	}

	void DrawPro(Vector2 pos, int w, int h, int tile_id) {
		Rectangle _pos = { pos.x, pos.y, w, h};
		DrawTexturePro(texture, tile[tile_id], _pos, {0, 0}, 0, WHITE);
	}

	Texture2D texture;
	std::vector<Rectangle> tile;

public:

	static Texture2D LoadTextureFromResources(std::string texture_path);

};

