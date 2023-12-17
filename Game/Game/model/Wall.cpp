#include "Wall.h"
#include "../core/Debug.h"

#define DEBUG_WALL 0

static Color NO_TINT = { 255,255,255,255 };
static Color DEBUG_TINT = { 220, 0, 220, 100 };

static void __swap(int* a, int* b) {
	int c = *b;
	*b = *a;
	*a = c;
}

Wall::Wall(int x, int y, int x2, int y2, BlockID id)
	: Box2D(x, y, abs(x2-x), abs(y2-y))
{
	if (x2 < x) {
		this->aabb.min.x = x2;
		this->aabb.max.x = x2 + w;
	}
	if (y2 < y) {
		this->aabb.min.y = y2;
		this->aabb.max.y = y2 + h;
	}
	
	blocksW = (abs(x2 - x)) / BLOCK_WIDTH;
	blocksH = (abs(y2 - y)) / BLOCK_HEIGHT;
	this->id = (int)id;
}

void Wall::Draw()
{
	for (int i = 0; i < blocksH; i++) {
		for (int j = 0; j < blocksW; j++) {
#if DEBUG_WALL
			DrawRectangle(aabb.min.x + j * BLOCK_WIDTH, aabb.min.y + i * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT, DEBUG_TINT);
			//DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, GREEN);
			//DrawRectangleLines(aabb.min.x, aabb.min.y, aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y, GREEN);
			DrawLine(aabb.min.x, aabb.min.y, aabb.min.x, aabb.max.y, GREEN);
			DrawLine(aabb.min.x, aabb.min.y, aabb.max.x, aabb.min.y, GREEN);
			DrawLine(aabb.max.x, aabb.min.y, aabb.max.x, aabb.max.y, GREEN);
			DrawLine(aabb.max.x, aabb.max.y, aabb.min.x, aabb.max.y, GREEN);
#else
			DrawTexture(Block::textures[id], aabb.min.x + j * BLOCK_WIDTH, aabb.min.y + i * BLOCK_HEIGHT, NO_TINT);
#endif
		}
	}
}
