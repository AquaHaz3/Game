#include "Sprite.h"

Texture2D* Sprite::NullTexture = 0;

Texture2D Sprite::LoadTextureFromResources(std::string texture_path)
{
	auto path = "resources/" + texture_path;
	Image image = LoadImage(path.c_str());
	Texture2D texture0 = LoadTextureFromImage(image);
	UnloadImage(image);

	image = LoadImageFromTexture(texture0);
	UnloadTexture(texture0);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);

	return texture;
}

Texture2D Sprite::GetNullTexture()
{
	if (NullTexture != 0) {
		return *NullTexture;
	}

	Image image = LoadImage("resources/null.png");
	Texture2D texture0 = LoadTextureFromImage(image);
	UnloadImage(image);

	image = LoadImageFromTexture(texture0);
	UnloadTexture(texture0);

	Texture2D tex = LoadTextureFromImage(image);
	UnloadImage(image);

	NullTexture = new Texture2D();
	NullTexture->id = tex.id;
	NullTexture->width = tex.width;
	NullTexture->height = tex.height;
	NullTexture->mipmaps = tex.mipmaps;
	NullTexture->format = tex.format;

	return *NullTexture;
}

std::map<std::string, Texture2D> SpriteLoader::textures = std::map<std::string, Texture2D>();

void SpriteLoader::LoadInGameSprites()
{
	if (textures.size() > 0) return;
	textures.emplace("player.png", Sprite::LoadTextureFromResources("player.png"));
	textures.emplace("inv_cell.png", Sprite::LoadTextureFromResources("inv_cell.png"));
	textures.emplace("inv_hand.png", Sprite::LoadTextureFromResources("inv_hand.png"));
	textures.emplace("ui_left.png", Sprite::LoadTextureFromResources("ui_left.png"));
	textures.emplace("ui_right.png", Sprite::LoadTextureFromResources("ui_right.png"));
}

Sprite SpriteLoader::GetSprite(std::string name)
{
	if (textures.count(name) > 0) {
		return Sprite(textures[name]);
	}
	else {
		printf("[SpriteLoader] [WARNING] Texture with name {%s} don't loaded", name.c_str());
		return Sprite(Sprite::GetNullTexture());
	}
}
