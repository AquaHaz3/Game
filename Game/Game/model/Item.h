#pragma once

#include <vector>
#include <raylib.h>
#include <string>

#define MAX_ITEM_ID 255

enum class ItemID
{
	AIR = 0,
	POTION_HEAL = 1,
	POTION_XP = 2,

	SPOTLIGHT = 200,
};

class Item
{

public:

	uint8_t id;
	int damage;
	short count;

public:

	Item() {};

	Item(ItemID id, int damage, int count) 
	{
		this->id = (int) id;
		this->damage = damage;
		this->count = count;
	}

public:

	static void InitItems();

protected:

	friend class ItemEntity;

	static std::vector<Texture2D> textures;

	static void addItem(ItemID id, std::string tex_path);
	static void addItem(ItemID id);


};

