#include "ItemEntity.h"
#include "../core/Debug.h"

ItemEntity::ItemEntity(int x, int y, ItemID id)
	: Entity(x, y, 32, 32, EntityType::Item)
{
	this->id = (char)id;
}

void ItemEntity::Draw()
{
	DrawTexture(Item::textures[this->id], aabb.min.x, aabb.min.y, WHITE);
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, RED);
	}
}
