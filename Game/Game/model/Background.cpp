#include "Background.h"

static Color NO_TINT = { 255,255,255,255 };

Background::Background(int x, int y, int camWidth, int camHeight, BlockID id)
{
	this->x = x;
	this->y = y;

	blocksW = camWidth / BLOCK_WIDTH;
	blocksH = camHeight / BLOCK_HEIGHT;

	this->id = (int)id;
}

void Background::Draw()
{
	for (int i = 0; i < blocksH; i++) {
		for (int j = 0; j < blocksW; j++) {
			DrawTexture(Block::textures[id], x + j * BLOCK_WIDTH, y + i * BLOCK_HEIGHT, NO_TINT);
		}
	}
}