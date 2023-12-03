#include "Player.h"
#include "../core/Debug.h"
#include "../core/Scene.h"

#define MAX_PLAYER_HP 10
#define MAX_PLAYER_XP 200
#define INVENTORY_SIZE 9

static Color HP_BAR = { 230, 53, 80, 255 };
static Color HP_BAR2 = { 250, 73, 100, 255 };

static Color MAGIC_BLUE = { 40, 129, 250, 255 };
static Color MAGIC_BLUE2 = { 60, 149, 250, 255 };
static Color NO_TINT = { 255,255,255,255 };

#include <raymath.h>

Player::Player(int x, int y) : Entity(x, y, 20, 30, EntityType::Player)
{

	health = 7;
	speed = 1;
	texture = SpriteLoader::GetSprite("player.png");
	inventoryCell = SpriteLoader::GetSprite("inv_cell.png"); // текстура ячейки инвентаря
	inventoryHand = SpriteLoader::GetSprite("inv_hand.png"); // текстура ячейки инвентаря (рука)

	texture.addTile(32, 64, 32, 32); // Go to RIGHT
	texture.addTile(32, 0, 32, 32); // Go to DOWN
	texture.addTile(32, 32, 32, 32); // Go to LEFT
	texture.addTile(32, 96, 32, 32); // Go to UP

	texture.addTile(0, 64, 32, 32); // Go to RIGHT
	texture.addTile(0, 0, 32, 32); // Go to DOWN
	texture.addTile(0, 32, 32, 32); // Go to LEFT
	texture.addTile(0, 96, 32, 32); // Go to UP

	texture.addTile(64, 64, 32, 32); // Go to RIGHT
	texture.addTile(64, 0, 32, 32); // Go to DOWN
	texture.addTile(64, 32, 32, 32); // Go to LEFT
	texture.addTile(64, 96, 32, 32); // Go to UP

	if (w < 32) {
		x_offset = (32 - w) / 2; aabb.min.x = x;
	}
	if (h < 32) {
		y_offset = (32 - h) / 2; aabb.min.y = y;
	}

	inventory = std::vector<Item>(INVENTORY_SIZE);
	invFreeIndex = 1;

	inv_x = (SceneManager::current->width / 2) - ((32 * INVENTORY_SIZE) / 2);

	for (int i = 0; i < INVENTORY_SIZE; i++) {
		inventory.push_back(Item(ItemID::AIR, 0)); // Заполняем инвентарь пустотой
	}

	xp = 120;
	remindAboutXp = 0;
	bow_progress = 0;
	weapon = { 0,0,0,0,0 };
}

void Player::Draw()
{
	drawInventory();
	drawUI(); 

	if (bow_progress > 0) { // Рисуем направление натягивания лука, возле игрока
		Vector2 mouse = GetMousePosition();
		Vector2 center = { aabb.min.x + w / 2,aabb.min.y + h / 2 };
		float angle = Vector2Angle({center.x + 1, center.y}, mouse) / PI;
		Vector2 z = Vector2MoveTowards(center, mouse, bow_progress);
		DrawLineV(center, z, LIGHTGRAY);
		DrawCircleV(Vector2MoveTowards(center, mouse, bow_progress / 2), 2, LIGHTGRAY);
		DrawCircleV(Vector2MoveTowards(center, mouse, bow_progress / 4), 2, LIGHTGRAY);
		DrawTextPro(GetFontDefault(), ">", z, { 2,8 }, 180*angle, 20, 1, WHITE);
	}

	texture.DrawPro((int)aabb.min.x - x_offset, (int)aabb.min.y - y_offset, 32, 32, (int)direction + walk_tick * 4);
	Rectangle render = { aabb.min.x - 32 - x_offset, aabb.min.y - 32 - y_offset, 96, 96 };
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLinesEx(render, 1, PURPLE);
		DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, {250, 20, 20, 200});
	}
}

#include "Movement.h"
#include "../events/CollectItemEvent.hpp"

void Player::Update(__int64 tick)
{
	if (tick % 6 == 2) checkForAttack();
	if (tick % 3 != 0) return;
	walk_tick = (tick / 100) % 2 + 1;
	Vector2 pre = aabb.min;
	bool isMoved = Movement::EntityWASDControl(this);
	if (isMoved) {
		bool isCollided = false;
		for (auto solid : SceneManager::current->boxes) { // Проходим по всем твёрдым предметам
			if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
				if (solid->flags & ENTITY_OBJECT) {
					Entity* e = (Entity*) solid;
					if (e->type == EntityType::Item) { // Если столкнулись с предметом
						auto event = new CollectItemEvent((ItemEntity*)e);
						OnEvent(event);
						delete event;
						SceneManager::removeObject(e);
					}
				}
				isCollided = true;
				break;
			}
		}
		if (isCollided) setPos(pre.x, pre.y);
	}
	else {
		walk_tick = 0;
	}
}

#include "misc/TextParticle.h"

void Player::OnEvent(Event* event)
{
	if (event->uuid == CollectItemEvent::getClassUUID()) // При подборе предмета
	{
		CollectItemEvent* itemE = (CollectItemEvent*)event;
		if (itemE->id == ItemID::POTION_HEAL) { // При подборе зелья здоровья
			int hp = rand() % 3 + 1;
			health = std::min(health+hp, MAX_PLAYER_HP);
			auto about = "+"+std::to_string(hp); // +1 or +2 or +3
			SceneManager::addParticle(new TextParticle(aabb.max, about, 80, RED));
			return;
		}
		if (itemE->id == ItemID::POTION_XP) { // При подборе XP
			int _xp = ((rand() % 5) * 10) + 20;
			xp = std::min(xp + _xp, MAX_PLAYER_XP);
			auto about = "+" + std::to_string(_xp); // from +20 to +60
			SceneManager::addParticle(new TextParticle(aabb.max, about, 80, MAGIC_BLUE));
			return;
		}
		putToInventory((uint8_t) itemE->id); // помещаем подобранный предмет в инвентарь
	}
}

static char digits[20] = {
	'0', 0, '1', 0, '2', 0, '3', 0, '4', 0,
	'5', 0, '6', 0, '7', 0, '8', 0, '9', 0
};

void Player::drawUI()
{
	int hp = (int)health;
	DrawText("HP: ", 12, 8, 20, WHITE); // HP: 
	DrawText("xp: ", 12, 36, 20, WHITE); // xp: 
	DrawRectangle(60, 38, 200, 16, GRAY); // Серая полоса xp
	DrawRectangle(60, 38 + 8, xp, 8, MAGIC_BLUE); // Синяя(заполненая) полоса 
	DrawRectangle(60, 38, xp, 8, MAGIC_BLUE2); // Синяя(заполненая) полоса (осветлённая)
	for (int i = 0; i < MAX_PLAYER_HP; i++) { // Рисует жизни
		if (i < hp) {
			DrawRectangle(60 + i * 12, 18, 10, 8, HP_BAR); // Полосы здоровья
			DrawRectangle(60 + i * 12, 10, 10, 8, HP_BAR2); // Полосы здоровья (осветлённая часть)
		}
		else {
			DrawRectangle(60 + i * 12, 10, 10, 16, GRAY);  // Серые полосы здоровья (пустые)
		}
	}

	if ((remindAboutXp / 25) % 2 != 0) {
		DrawRectangleLinesEx({ 8,32,256,32 }, 2, MAGIC_BLUE2); // Напоминание о малом кол-ве XP 
	}
	if (remindAboutXp > 0) remindAboutXp--;
}

void Player::drawInventory() // рисует инвентарь
{

	Vector2 cellPos = { (float)inv_x, 0 };
	inventoryHand.Draw(cellPos);
	if (inventory[0].count > 0) { // Ячейка (руки)
		DrawTextureV(Item::textures[inventory[0].id], cellPos, NO_TINT);
	}

	for (int i = 1; i < INVENTORY_SIZE; i++) { // Рисовка ячейки
		cellPos.x += 32;
		inventoryCell.Draw(cellPos);
		int c = inventory[i].count;
		if (c > 0) {
			DrawTextureV(Item::textures[inventory[i].id], cellPos, NO_TINT);
		}
		if (c > 1 && c < 10) {
			DrawText(&digits[c*2], cellPos.x + 28, cellPos.y + 23, 10, BLACK);
			DrawText(&digits[c*2], cellPos.x + 27, cellPos.y + 22, 10, WHITE);
		}
		if (c >= 10) {
			DrawText(std::to_string(c).c_str(), cellPos.x + 28, cellPos.y + 23, 10, BLACK);
			DrawText(std::to_string(c).c_str(), cellPos.x + 27, cellPos.y + 22, 10, WHITE);
		}
	}
}

bool Player::putToInventory(uint8_t id) // помещает предмет в инвентарь
{
	for (int i = 1; i < INVENTORY_SIZE; i++) { // Случай когда уже предмет есть, надо кол-во++
		if (inventory[i].id == id) { 
			inventory[i].count++; 
			return true;
		}
	}
	if (inventory[0].count < 1 && Item::isWeaponItem(id)) // Если предмет оружие, кладём в руку
	{
		inventory[0].id = id;
		inventory[0].count = 1;
		weapon = Item::weapons[id]; // Устанавливаем текущее оружие
		return true;
	}
	if (invFreeIndex != INVENTORY_SIZE) { // Кладём в свободную ячейку, если ещё есть место
		inventory[invFreeIndex].id = id;
		inventory[invFreeIndex].count = 1;
		invFreeIndex++;
	}
	return false;
}

#include "entity/Arrow.h"

void Player::checkForAttack()
{
	// Если наше оружие лук, то натягиваем его
	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (WType) weapon.type == WType::BOWS)
	{
		bow_progress = std::min(bow_progress + 1, 100);
	}
	else {
		if (bow_progress > 20) { // Если натягивал лук 
			if (xp >= weapon.xp_cost) { // И если хватает xp на выстрел
				Vector2 mouse = GetMousePosition();
				Vector2 center = { aabb.min.x + w / 2,aabb.min.y + h / 2 };
				float angle = Vector2Angle({ center.x + 1, center.y }, mouse);
				SceneManager::addObject(
					new Arrow(center.x, center.y + 8, 0.5 + (1 * (bow_progress / 100.0f)), angle, 1, 0)
				);
				xp -= weapon.xp_cost; // Снимаем с игрока xp
			}
			else {
				if (remindAboutXp <= 1) remindAboutXp = 125; // Если нет xp, то напоминаем об этом
			}
		}
		bow_progress = 0;
	}
}
