#include "Item.h"
#include "../core/Sprite.h"

std::vector<Texture2D> Item::textures = std::vector<Texture2D>(MAX_ITEM_ID);
std::vector<Weapon> Item::weapons = std::vector<Weapon>(MAX_ITEM_ID);

#define _weapon(type, level, damage, xp_cost, reserved) \
{(uint8_t) type, (uint8_t) level, damage, xp_cost, reserved}

Item::Item()
{
	id = 0;
	count = 0;
}

void Item::InitItems()
{
	addItem(ItemID::AIR);
	addItem(ItemID::POTION_HEAL, "potion_health.png");
	addItem(ItemID::POTION_XP, "potion_xp.png");
	addItem(ItemID::CROWN, "crown.png");
	addItem(ItemID::SPOTLIGHT, "light0.png");

	using enum ItemID;

	//                            Type           Rare            d  x  r
	addWeapon(BOW, _weapon(WType::BOWS, WLevel::DEFAULT, 5, 2, 1), "weapon/bow.png");
	addWeapon(SWORD, _weapon(WType::SWORDS, WLevel::DEFAULT, 5, 0, 1), "weapon/sword.png");
	addWeapon(FAST_BOW, _weapon(WType::BOWS, WLevel::RARE, 9, 7, 4), "weapon/fast_bow.png");
	addWeapon(BOW2, _weapon(WType::BOWS, WLevel::UNUSUAL, 7, 6, 1), "weapon/bow2.png");
	addWeapon(RED_BOW, _weapon(WType::BOWS, WLevel::LEGENDARY, 15, 10, 2), "weapon/red_bow.png");
	addWeapon(RED_WAND, _weapon(WType::EXTRA, WLevel::DEFAULT, 3, 3, 23), "weapon/magic_wand.png");
	addWeapon(GREEN_WAND, _weapon(WType::EXTRA, WLevel::UNUSUAL, 8, 5, 20), "weapon/stuff_green.png");
	addWeapon(ELECTRO_STAFF, _weapon(WType::EXTRA, WLevel::RARE, 5, 9, 30), "weapon/staff.png");
	addWeapon(GUN, _weapon(WType::EXTRA, WLevel::UNUSUAL, 5, 2, 17), "weapon/gun.png");
	addWeapon(RED_STAFF, _weapon(WType::EXTRA, WLevel::LEGENDARY, 8, 15, 30), "weapon/staff_2.png");
}

bool Item::isWeaponItem(uint8_t id) {
	return (weapons[id].type != 0);
}

void Item::addItem(ItemID id, std::string tex_path)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
}

void Item::addItem(ItemID id)
{
	textures[(int)id] = Sprite::GetNullTexture();
}

void Item::addWeapon(ItemID id, Weapon w, std::string tex_path)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
	weapons[(int)id] = w;
}
