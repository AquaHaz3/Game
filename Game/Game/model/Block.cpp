#include "Block.h"

static Color NO_TINT = { 255,255,255,255 };

Block::Block(int x, int y, BlockID id)
	: Box2D(x, y, 32, 32) 
{
	this->id = (int)id;
}

Block::Block(int x, int y, int w, int h, BlockID id)
	: Box2D(x, y, w, h)
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
	/* Solid blocks: */
	addBlock(BlockID::WALL, "wall.png");
	addBlock(BlockID::DARK_BRICK, "dark_bricks.png");
	addBlock(BlockID::DARK_STONE, "dark_stone.png");
	addBlock(BlockID::BLOCK_TEST, "block.png");
	addBlock(BlockID::WOOD, "wood_corner.png");
	addBlock(BlockID::WOOD_X, "wood_wall_x.png");
	addBlock(BlockID::WOOD_Y, "wood_wall_y.png");
	/* Ground blocks: */
	addBlock(BlockID::GRAVEL, "gravel.png");
	addBlock(BlockID::COBBLESTONE, "cobblestone.png");
	addBlock(BlockID::FLOOR1, "floor.png");
	addBlock(BlockID::FLOOR2, "floor2.png");
	addBlock(BlockID::PLANKS, "planks.png");
	addBlock(BlockID::SANDSTONE, "sandstone.png");
	
}

void Block::addBlock(BlockID id, std::string tex_path)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
}
