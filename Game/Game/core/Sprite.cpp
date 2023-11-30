#include "Sprite.h"

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

