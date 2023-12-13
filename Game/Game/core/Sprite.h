#pragma once

#include <raylib.h>
#include <string>
#include <vector>

class Sprite
{

public:

	Sprite() {
		texture = { 0,0,0,0 };
		tile = std::vector<Rectangle*>();
		isTiled = false;
	};

	Sprite(std::string path)
	{
		texture = LoadTextureFromResources(path);
		tile = std::vector<Rectangle*>();
		isTiled = false;
	};

	Sprite(Texture2D texture)
	{
		this->texture = texture;
		tile = std::vector<Rectangle*>();
		isTiled = false;
	};

	~Sprite() {
		printf("Sprite unload!\n");
		for (int i = 0; i < tile.size(); i++) {
			if(tile[i] != 0) delete tile[i];
			tile[i] = 0;
		}
		tile.clear();
	}

	void addTile(int x, int y, int w, int h) {
		Rectangle* region = new Rectangle();
		region->x = (float)x;
		region->y = (float)y;
		region->width = (float)w;
		region->height = (float)h;
		tile.push_back(region);
		isTiled = true;
	}

	void Draw(Vector2 pos) {
		DrawTextureV(texture, pos, WHITE);
	}

	void DrawTile(int x, int y, int tile_id) {
		DrawTextureRec(texture, *tile[tile_id], { (float)x, (float)y }, WHITE);
	}

	void DrawTileTint(int x, int y, int tile_id, Color color) {
		DrawTextureRec(texture, *tile[tile_id], { (float)x, (float)y }, color);
	}

	void DrawPro(int x, int y, int w, int h, float oX, float oY, float angle) {
		Rectangle _pos = { (float)x, (float)y, (float)w, (float)h};
		DrawTexturePro(texture, {0,0,32,32}, _pos, {oX, oY}, angle, WHITE);
	}

	Texture2D texture;
	std::vector<Rectangle*> tile = std::vector<Rectangle*>();

public:

	static Texture2D LoadTextureFromResources(std::string texture_path);
	static Texture2D GetNullTexture();

	static Texture2D* NullTexture;

	bool isTiled = false;

};

#include <map>
#include <memory>

typedef std::shared_ptr<Sprite> SpriteRef;

class SpriteLoader {

public:

	static void LoadInGameSprites();
	static SpriteRef GetSprite(std::string name);

private:

	static std::map<std::string, Texture2D> textures;
	static std::map<std::string, SpriteRef> loadedSprites;

};

