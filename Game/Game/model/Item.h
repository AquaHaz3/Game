#pragma once

#include <vector>
#include <raylib.h>
#include <string>

#define MAX_ITEM_ID 255

enum class ItemID
{
	AIR = 0,
	SPOTLIGHT = 254,
	POTION_HEAL = 1,
	POTION_XP = 2,
	CROWN,
	BOW,
	SWORD,
	FAST_BOW,
	BOW2,
	RED_BOW,

	__lastItem
};

enum class WType { // тип оружия
	SWORDS = 1,
	BOWS = 2,
	EXTRA = 3
};

enum class WLevel // уровни оружия (по редкости)
{ 
	DEFAULT, // обычный
	UNUSUAL, // необычный
	RARE, // редкий
	LEGENDARY // легенадрный
};

struct Weapon { // Структура оружия (5 байт)

	uint8_t type; // тип оружия (мечи, луки, специальные)
	uint8_t level; // уровень оружия (обычый, необычный, редкий, легендарный)
	uint8_t damage; // урон наносимый оружимем
	uint8_t xp_cost; // сколько xp стоит один выстрел
	uint8_t reserved; // зарезирвировано (доп. харакетристика, например: дальность лука)

};

class Item
{

public:

	uint8_t id;
	short count;

public:

	Item();;

	Item(ItemID id, int count) 
	{
		this->id = (int) id;
		this->count = count;
	}

public:

	static void InitItems();
	static bool isWeaponItem(uint8_t id);

protected:

	friend class ItemEntity;
	friend class Player;
	friend class EditorUI;

	static std::vector<Texture2D> textures;
	static std::vector<Weapon> weapons;

	/* Добавить предмет в список 
	@param id - ID; tex_path - название текстурки */
	static void addItem(ItemID id, std::string tex_path);
	/* Добавить оружие в список
	@param id - ID; w - структура оружия; tex_path - название текстурки */
	static void addWeapon(ItemID id, Weapon w, std::string tex_path);

	static void addItem(ItemID id);


};

