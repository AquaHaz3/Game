#include "Item.h"
#include "../core/Sprite.h"

std::vector<Texture2D> Item::textures = std::vector<Texture2D>(MAX_ITEM_ID);

void Item::InitItems()
{
	addItem(ItemID::AIR);
	addItem(ItemID::POTION_HEAL, "potion_health.png");
	addItem(ItemID::POTION_XP, "potion_xp.png");
}

void Item::addItem(ItemID id, std::string tex_path)
{
	textures[(int)id] = Sprite::LoadTextureFromResources(tex_path);
}

void Item::addItem(ItemID id)
{
	textures[(int)id] = Sprite::GetNullTexture();
}
