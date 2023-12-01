#include "Block.h"

static Color NO_TINT = { 255,255,255,255 };

Block::Block(int x, int y, BlockID id)
	: Box2D(x, y, 32, 32) 
{
	this->id = (int)id;
}

void Block::Draw()
{
	DrawTexture(textures[id], (int)aabb.min.x, (int)aabb.min.y, NO_TINT);
}

std::vector<Texture2D> Block::textures = std::vector<Texture2D>(MAX_BLOCK_ID);

void Block::InitBlocks()
{
	addBlock(BlockID::WALL, "wall.png");
}

void Block::addBlock(BlockID id, std::string tex_path)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
}
