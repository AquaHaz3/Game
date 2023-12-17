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
std::map<std::string, SpriteRef> SpriteLoader::loadedSprites = std::map<std::string, SpriteRef>();

void SpriteLoader::LoadInGameSprites()
{
	if (textures.size() > 0) return;
	textures.emplace("player.png", Sprite::LoadTextureFromResources("player.png"));
	textures.emplace("inv_cell.png", Sprite::LoadTextureFromResources("inv_cell.png"));
	textures.emplace("inv_hand.png", Sprite::LoadTextureFromResources("inv_hand.png"));
	textures.emplace("ui_left.png", Sprite::LoadTextureFromResources("ui_left.png"));
	textures.emplace("ui_right.png", Sprite::LoadTextureFromResources("ui_right.png"));
	textures.emplace("arrow.png", Sprite::LoadTextureFromResources("arrow.png"));
	textures.emplace("arrow2.png", Sprite::LoadTextureFromResources("arrow2.png"));
}

SpriteRef SpriteLoader::GetSprite(std::string name)
{

	if (loadedSprites.count(name) > 0) return loadedSprites[name];

	if (textures.count(name) > 0) {
		auto spriteRef = std::shared_ptr<Sprite>(new Sprite(textures[name]));
		loadedSprites.emplace(name, spriteRef);
		return spriteRef;
	}
	else {
		auto spriteRef = std::shared_ptr<Sprite>(new Sprite(Sprite::LoadTextureFromResources(name)));
		loadedSprites.emplace(name, spriteRef);
		return spriteRef;
	}
}
