#include "ItemEntity.h"
#include "../core/Debug.h"

ItemEntity::ItemEntity(int x, int y, int w, int h, ItemID id)
	: Entity(x, y, w, h, EntityType::Item)
{
	if (w < 32) {
		x_offset = (32 - w) / 2;
		aabb.min.x = x + x_offset;
		aabb.max.x += x_offset;
	}
	if (h < 32) {
		y_offset = (32 - h) / 2;
		aabb.min.y = y + y_offset;
		aabb.max.y += y_offset;
	}
	this->id = (char)id; 
	this->tint = { 0,0,0,0 };
}

ItemEntity::ItemEntity(int x, int y, int w, int h, ItemID id, Color light)
	: Entity(x, y, w, h, EntityType::Item)
{
	if (w < 32) {
		x_offset = (32 - w) / 2;
		aabb.min.x = x + x_offset;
		aabb.max.x += x_offset;
	}
	if (h < 32) {
		y_offset = (32 - h) / 2;
		aabb.min.y = y + y_offset;
		aabb.max.y += y_offset;
	}
	this->id = (char)id;
	this->tint = light;
}

void ItemEntity::Draw()
{
	if (tint.a > 0) {
		DrawTexture(Item::textures[(int)ItemID::SPOTLIGHT], aabb.min.x - x_offset, aabb.min.y - y_offset, tint);
	}
	DrawTexture(Item::textures[this->id], aabb.min.x - x_offset, aabb.min.y - y_offset, WHITE);
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, RED);
	}
}
