#pragma once

#include "../core/Box2D.h"
#include "../core/Sprite.h"
#include <vector>

#define MAX_BLOCK_ID 255

enum class BlockID
{
	WALL = 0
};

class Block : public Box2D
{

public:

	char id;

public:

	Block() {};
	Block(int x, int y, BlockID id);

	virtual void Draw();

	virtual void Update(__int64 tick) { }

	virtual void OnEvent(Event* event) {};

public:

	static void InitBlocks();

protected:

	static std::vector<Texture2D> textures;

	static void addBlock(BlockID id, std::string tex_path);

};

