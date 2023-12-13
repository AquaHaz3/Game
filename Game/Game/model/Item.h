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

enum class WType { // ��� ������
	SWORDS = 1,
	BOWS = 2,
	EXTRA = 3
};

enum class WLevel // ������ ������ (�� ��������)
{ 
	DEFAULT, // �������
	UNUSUAL, // ���������
	RARE, // ������
	LEGENDARY // �����������
};

struct Weapon { // ��������� ������ (5 ����)

	uint8_t type; // ��� ������ (����, ����, �����������)
	uint8_t level; // ������� ������ (������, ���������, ������, �����������)
	uint8_t damage; // ���� ��������� ��������
	uint8_t xp_cost; // ������� xp ����� ���� �������
	uint8_t reserved; // ��������������� (���. ��������������, ��������: ��������� ����)

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

	/* �������� ������� � ������ 
	@param id - ID; tex_path - �������� ��������� */
	static void addItem(ItemID id, std::string tex_path);
	/* �������� ������ � ������
	@param id - ID; w - ��������� ������; tex_path - �������� ��������� */
	static void addWeapon(ItemID id, Weapon w, std::string tex_path);

	static void addItem(ItemID id);


};

