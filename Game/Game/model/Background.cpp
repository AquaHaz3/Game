#include "Background.h"

static Color NO_TINT = { 255,255,255,255 };
static Color COBBLE_COLOR = { 67, 67, 75,255 };

Background::Background()
{
	id = (int)BlockID::WALL;
	this->x = 0;
	this->y = 0;
	blocksW = 32;
	blocksH = 32;
}

Background::Background(int x, int y, int camWidth, int camHeight, BlockID id)
{
	this->x = x;
	this->y = y;

	blocksW = camWidth / BLOCK_WIDTH;
	blocksH = camHeight / BLOCK_HEIGHT;

	this->id = (int)id;
}

#include <thread>

void Background::Draw()
{
	uint32_t s = 0xababab;
	Color stack = { 0,0,0,255 };
	if(id == 101) {
		for (int i = 0; i < blocksH * 4; i++) {
			for (int j = 0; j < blocksW * 4; j++) {
				stack.r = 67 + (s) % 8;
				stack.g = 67 + (s) % 8;
				stack.b = 71 + (s) % 8;
				s = ((i * (j+1)) + s * 4);
				DrawRectangle(x + j * 8, y + i * 8, 8, 8, stack);
			}
		}
	}
	else {
		for (int i = 0; i < blocksH; i++) {
			for (int j = 0; j < blocksW; j++) {
				DrawTexture(Block::textures[id], x + j * BLOCK_WIDTH, y + i * BLOCK_HEIGHT, NO_TINT);
			}
		}
	}
}