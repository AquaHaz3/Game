#include "Item.h"
#include "../core/Sprite.h"

std::vector<Texture2D> Item::textures = std::vector<Texture2D>(MAX_ITEM_ID);
std::vector<Weapon> Item::weapons = std::vector<Weapon>(MAX_ITEM_ID);

#define _weapon(type, level, damage, xp_cost, reserved) \
{(uint8_t) type, (uint8_t) level, damage, xp_cost, reserved}

void Item::InitItems()
{
	addItem(ItemID::AIR);
	addItem(ItemID::POTION_HEAL, "potion_health.png");
	addItem(ItemID::POTION_XP, "potion_xp.png");
	addItem(ItemID::CROWN, "crown.png");
	addItem(ItemID::SPOTLIGHT, "light0.png");

	//                             Type         Rare             d  x  r
	addWeapon(ItemID::BOW, _weapon(WType::BOWS, WLevel::DEFAULT, 5, 5, 4), "weapon/bow.png");
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
