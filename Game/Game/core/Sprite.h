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

	Sprite(Texture2D texture)
	{
		this->texture = texture;
		tile = std::vector<Rectangle>();
	};

	void addTile(int x, int y, int w, int h) {
		Rectangle region{ (float) x, (float)y,(float)w, (float)h };
		tile.push_back(region);
	}

	void Draw(Vector2 pos) {
		DrawTextureV(texture, pos, WHITE);
	}

	void DrawTile(int x, int y, int tile_id) {
		DrawTextureRec(texture, tile[tile_id], { (float)x, (float)y }, WHITE);
	}

	void DrawPro(int x, int y, int w, int h, float oX, float oY, float angle) {
		Rectangle _pos = { (float)x, (float)y, (float)w, (float)h};
		DrawTexturePro(texture, {0,0,32,32}, _pos, {oX, oY}, angle, WHITE);
	}

	Texture2D texture;
	std::vector<Rectangle> tile;

public:

	static Texture2D LoadTextureFromResources(std::string texture_path);
	static Texture2D GetNullTexture();

	static Texture2D* NullTexture;

};

#include <map>

class SpriteLoader {

public:

	static void LoadInGameSprites();
	static Sprite GetSprite(std::string name);

private:

	static std::map<std::string, Texture2D> textures;

};

