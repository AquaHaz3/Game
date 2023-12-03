#pragma once

#include "../core/Box2D.h"
#include "../core/Sprite.h"
#include <vector>

#define MAX_BLOCK_ID 255

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32

enum class BlockID /* NAME = ID */
{

	AIR = 0,
	WALL = 1,
	DARK_BRICK = 2,
	DARK_STONE = 3,
	BLOCK_TEST = 4,

	GRAVEL = 100,
	COBBLESTONE = 101,
	FLOOR1 = 102,
	FLOOR2 = 103,

};

class Block : public Box2D
{

public:

	char id;

public:

	Block() {};
	Block(int x, int y, BlockID id);
	Block(int x, int y, int w, int h, BlockID id);

	virtual void Draw();

	virtual void Update(__int64 tick) { }

	virtual void OnEvent(Event* event) {};

public:

	static void InitBlocks();

protected:

	static std::vector<Texture2D> textures;

	static void addBlock(BlockID id, std::string tex_path);

	friend class Wall;  // 
	friend class Background;  // 

};

