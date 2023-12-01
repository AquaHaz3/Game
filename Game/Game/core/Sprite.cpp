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


