#include "Player.h"
#include "../core/Debug.h"
#include "../core/Scene.h"
#include "ui/SoundUI.h"

#define MAX_PLAYER_HP 10
#define MAX_PLAYER_XP 200
#define INVENTORY_SIZE 9

static Color HP_BAR = { 230, 53, 80, 255 };
static Color HP_BAR2 = { 250, 73, 100, 255 };

static Color MAGIC_BLUE = { 40, 129, 250, 255 };
static Color MAGIC_BLUE2 = { 60, 149, 250, 255 };
static Color NO_TINT = { 255,255,255,255 };

#include <raymath.h>

Player::Player() : Entity(0, 0, 32, 32, EntityID::Player)
{
	health = 0;
	speed = 0;
	isFrozen = false;
	weapon = { 0,0,0,0,0 };
	flags |= PLAYER_OBJECT;
}

Player::Player(int x, int y) : Entity(x, y, 20, 30, EntityID::Player)
{

	flags |= PLAYER_OBJECT;

	health = 7;
	speed = 1;
	texture = SpriteLoader::GetSprite("player.png");
	inventoryCell = SpriteLoader::GetSprite("inv_cell.png"); // текстура ячейки инвентаря
	inventoryHand = SpriteLoader::GetSprite("inv_hand.png"); // текстура ячейки инвентаря (рука)

	if (!texture->isTiled) {
		texture->addTile(32, 64, 32, 32); // Go to RIGHT
		texture->addTile(32, 0, 32, 32); // Go to DOWN
		texture->addTile(32, 32, 32, 32); // Go to LEFT
		texture->addTile(32, 96, 32, 32); // Go to UP

		texture->addTile(0, 64, 32, 32); // Go to RIGHT
		texture->addTile(0, 0, 32, 32); // Go to DOWN
		texture->addTile(0, 32, 32, 32); // Go to LEFT
		texture->addTile(0, 96, 32, 32); // Go to UP

		texture->addTile(64, 64, 32, 32); // Go to RIGHT
		texture->addTile(64, 0, 32, 32); // Go to DOWN
		texture->addTile(64, 32, 32, 32); // Go to LEFT
		texture->addTile(64, 96, 32, 32); // Go to UP
	}

	if (w < 32) x_offset = (32 - w) / 2; aabb.min.x = x;
	if (h < 32) y_offset = (32 - h) / 2; aabb.min.y = y;
	

	inventory = std::vector<Item>();
	invFreeIndex = 1;

	inv_x = (SceneManager::current->camWidth / 2) - ((32 * INVENTORY_SIZE) / 2);
	 
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		inventory.push_back(Item(ItemID::AIR, 0)); // Заполняем инвентарь пустотой
	}

	xp = 150;
	remindAboutXp = 0;
	weapon = { 0,0,0,0,0 };
	isFrozen = false;

}

void Player::Draw()
{

	if (bow_progress > 0) { // Рисуем направление натягивания лука, возле игрока
		Vector2 mouse = SceneManager::GetMouseOnWorld();
		Vector2 center = { aabb.min.x + w / 2,aabb.min.y + h / 2 };
		float angle = MyVector2Angle({center.x + 1, center.y}, mouse) / PI;
		Vector2 z = Vector2MoveTowards(center, mouse, bow_progress);
		DrawLineV(center, z, LIGHTGRAY);
		DrawCircleV(Vector2MoveTowards(center, mouse, bow_progress / 2), 2, LIGHTGRAY);
		DrawCircleV(Vector2MoveTowards(center, mouse, bow_progress / 4), 2, LIGHTGRAY);
		DrawTextPro(GetFontDefault(), ">", z, { 2,8 }, 180*angle, 20, 1, WHITE);
	}

	drawWeaponInHand(0);
	texture->DrawTile((int)aabb.min.x - x_offset, (int)aabb.min.y - y_offset, (int)direction + walk_tick * 4);
	drawWeaponInHand(1);

	if (debug_util::isDebugBoxes()) {
		Rectangle render = { aabb.min.x - 48 - x_offset, aabb.min.y - 48 - y_offset, 128, 128 };
		DrawRectangleLinesEx(render, 1, PURPLE);
		DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, {250, 20, 20, 200});
		std::string test = "(" + std::to_string((int)aabb.min.x) + ", " + std::to_string((int)aabb.min.y) + ")";
		DrawText(test.c_str(), aabb.max.x, aabb.max.y, 10, WHITE);
	}

	SceneManager::BeginCameraOverlay();
	drawInventory();
	drawUI();
	SceneManager::EndCameraOverlay();
}

#include "Movement.h"
#include "../events/CollectItemEvent.hpp"
#include "../events/ChestEvents.hpp"
#include "../events/ProjectileHitEvent.hpp"

static char digits[20] = {
	'0', 0, '1', 0, '2', 0, '3', 0, '4', 0,
	'5', 0, '6', 0, '7', 0, '8', 0, '9', 0
};

void Player::drawWeaponInHand(int layer) {

	using enum Direction;
	using enum WType;

	bool sword = weapon.type == (int)SWORDS;
	bool wand = weapon.type == (int)EXTRA;

	if (!sword && !wand) return;

	if (direction == UP && layer == 0) {
		Rectangle dest = { aabb.min.x + 8, aabb.min.y + 20, 24, 24 };
		float _angle = (float)-sword_progress;
		if (wand) _angle = (float)-gun_progress;
		DrawTexturePro(Item::textures[weaponID], { 32, 0, -32,32 }, dest, { 16,16 }, _angle, WHITE);
	}

	if (direction == LEFT && layer == 1) {
		Rectangle dest = { aabb.min.x + 8, aabb.min.y + 20, 24, 24 };
		float _angle = (float)-sword_progress;
		if (wand) _angle = (float)-gun_progress;
		DrawTexturePro(Item::textures[weaponID], { 32, 0, -32,32 }, dest, { 16,16 }, _angle, WHITE);
	}

	if ((direction == RIGHT || direction == DOWN) && layer == 1) {
		Rectangle dest = { aabb.min.x + 12, aabb.min.y + 24, 24, 24 };
		float _angle = (float)sword_progress;
		if (wand) _angle = (float)gun_progress;
		DrawTexturePro(Item::textures[weaponID], { 0,0,32,32 }, dest, { 8,20 }, _angle, WHITE);
	}

	if (layer == 1 && wand) {
		Vector2 mouse = SceneManager::GetMouseOnWorld();
		DrawCircleLines(mouse.x, mouse.y, 16, { 170, 170, 170, 70 });
	}
}

void Player::drawUI()
{

	int hp = (int)health;

	Vector2 textPos = { 12, 8 };
	Vector2 barsPos = { 60, 38 };
	SceneManager::ProjectToCamera(textPos); // Проекция на камеру
	SceneManager::ProjectToCamera(barsPos); // Проекция на камеру

	DrawText("HP: ", textPos.x, textPos.y, 20, WHITE); // HP: 
	DrawText("xp: ", textPos.x, textPos.y+28, 20, WHITE); // xp: 
	DrawRectangle(barsPos.x, barsPos.y, 200, 16, GRAY); // Серая полоса xp
	DrawRectangle(barsPos.x, barsPos.y + 8, xp, 8, MAGIC_BLUE); // Синяя(заполненая) полоса 
	DrawRectangle(barsPos.x, barsPos.y, xp, 8, MAGIC_BLUE2); // Синяя(заполненая) полоса (осветлённая)

	for (int i = 0; i < MAX_PLAYER_HP; i++) { // Рисует жизни
		if (i < hp) {
			DrawRectangle(barsPos.x + i * 12, barsPos.y - 20, 10, 8, HP_BAR); // Полосы здоровья
			DrawRectangle(barsPos.x + i * 12, barsPos.y - 28, 10, 8, HP_BAR2); // Полосы здоровья (осветлённая часть)
		}
		else {
			DrawRectangle(barsPos.x + i * 12, barsPos.y - 28, 10, 16, GRAY);  // Серые полосы здоровья (пустые)
		} 
	}

	if ((remindAboutXp / 25) % 2 != 0) {
		Rectangle reminderRect = { 8,32,256,32 };
		SceneManager::ProjectToCamera(reminderRect);
		DrawRectangleLinesEx(reminderRect, 2, MAGIC_BLUE2); // Напоминание о малом кол-ве XP 
	}
	if (remindAboutXp > 0) remindAboutXp--;
}

void Player::drawInventory() // рисует инвентарь
{

	Vector2 cellPos = { (float)inv_x, 0 };
	SceneManager::ProjectToCamera(cellPos);
	inventoryHand->Draw(cellPos);
	if (inventory[0].count > 0) { // Ячейка (руки)
		DrawTextureV(Item::textures[inventory[0].id], cellPos, NO_TINT);
	}

	int temp = cellPos.x;

	for (int i = 1; i < INVENTORY_SIZE; i++) { // Рисовка ячейки
		cellPos.x += 32;
		inventoryCell->Draw(cellPos);
		int c = inventory[i].count;
		if (c > 0) DrawTextureV(Item::textures[inventory[i].id], cellPos, NO_TINT);
	}

	cellPos.x = temp;

	for (int i = 1; i < INVENTORY_SIZE; i++) { // Рисовка кол-ва предметов (поверх)
		int c = inventory[i].count;
		cellPos.x += 32;
		if (c > 1 && c < 10) {
			DrawText(&digits[c * 2], cellPos.x + 28, cellPos.y + 23, 10, BLACK);
			DrawText(&digits[c * 2], cellPos.x + 27, cellPos.y + 22, 10, WHITE);
		}
		if (c >= 10) {
			DrawText(std::to_string(c).c_str(), cellPos.x + 28, cellPos.y + 23, 10, BLACK);
			DrawText(std::to_string(c).c_str(), cellPos.x + 27, cellPos.y + 22, 10, WHITE);
		}
	}
}

bool Player::putToInventory(uint8_t id) // помещает предмет в инвентарь
{
	SoundUI::PlayOnce("pickup");
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
		weaponID = id;
		return true;
	}
	if (invFreeIndex != INVENTORY_SIZE) { // Кладём в свободную ячейку, если ещё есть место
		inventory[invFreeIndex].id = id;
		inventory[invFreeIndex].count = 1;
		invFreeIndex++;
	}
	return false;
}

#include "misc/TextParticle.h"

void Player::OnEvent(Event* event)
{
	if (event->uuid == CollectItemEvent::getClassUUID()) // При подборе предмета
	{
		CollectItemEvent* itemE = (CollectItemEvent*)event;
		if (itemE->id == ItemID::POTION_HEAL) { // При подборе зелья здоровья
			int hp = rand() % 3 + 1;
			health = std::min(health + hp, MAX_PLAYER_HP);
			auto about = "+" + std::to_string(hp); // +1 or +2 or +3
			SceneManager::addParticle(new TextParticle(aabb.max, about, 80, RED));
			SoundUI::PlayOnce("potion");
			return;
		}
		if (itemE->id == ItemID::POTION_XP) { // При подборе XP
			int _xp = ((rand() % 5) * 10) + 30;
			xp = std::min(xp + _xp, MAX_PLAYER_XP);
			auto about = "+" + std::to_string(_xp); // from +30 to +70
			SceneManager::addParticle(new TextParticle(aabb.max, about, 80, MAGIC_BLUE));
			SoundUI::PlayOnce("potion");
			return;
		}
		putToInventory((uint8_t)itemE->id); // помещаем подобранный предмет в инвентарь
	}
	if (event->uuid == ChestOpenEvent::getClassUUID()) {
		isFrozen = true;
	}
	if (event->uuid == ChestDropEvent::getClassUUID()) {
		isFrozen = false;
	}
	if (event->uuid == ProjectileHitEvent::getClassUUID()) { // Игрок ловит снаряд
		auto hitEvent = (ProjectileHitEvent*)event;
		Vector2 from = { hitEvent->x - hitEvent->xSpeed * 8, hitEvent->y - hitEvent->ySpeed *8 };
		Vector2 to = { hitEvent->x + hitEvent->xSpeed * 8, hitEvent->y + hitEvent->ySpeed * 8 };
		using namespace UtilAABB;
		auto side = getCollisionSide(from, to, &aabb);
		if (side == Side::LEFT && direction == Direction::LEFT && sword_progress == -30) return;
		if (side == Side::RIGHT && direction == Direction::RIGHT && sword_progress == -30) return;
		if (side == Side::TOP && direction == Direction::UP && sword_progress == -30) return;
		if (side == Side::BOTTOM && direction == Direction::DOWN && sword_progress == -30) return;
		this->health -= hitEvent->damage;
	}
}

void Player::Update(__int64 tick)
{
	if (tick % 6 == 2) checkForAttack(tick);
	if (tick % 3 != 0) return;
	walk_tick = (tick / 100) % 2 + 1;
	Vector2 pre = aabb.min;

	bool isMoved = false;
	if (!isFrozen) {
		isMoved = Movement::EntityWASDControl(this);
		if (tick % 18 == 0) {
			Vector2 mouse = SceneManager::GetMouseOnWorld();
			Vector2 center = { aabb.min.x + w / 2,aabb.min.y + h / 2 };
			float angle = (MyVector2Angle({ center.x + 1, center.y }, mouse) / PI) * 180;
			if (angle <= 0) angle += 360.0;
			if (angle >= 300.0 || angle <= 60.0) direction = Direction::RIGHT;
			if (angle >= 120.0 && angle <= 240.0) direction = Direction::LEFT;
			if (angle > 60.0 && angle < 120.0) direction = Direction::DOWN;
			if (angle > 240.0 && angle < 300.0) direction = Direction::UP;
		}
	}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		if (sword_progress == 0) sword_progress = 60;
	}

	int key = GetCharPressed();

	if ((key >= '2') && (key <= '9'))
	{
		char index = key - '1';
		if (Item::isWeaponItem(inventory[index].id)) {
			Item swap = inventory[index];
			inventory[index] = inventory[0];
			inventory[0] = swap;
			weaponID = swap.id;
			weapon = Item::weapons[swap.id];
		}
	}

	if (IsKeyDown(KEY_Z)) {
		auto state = SceneManager::current->cameraZoom.Notify();
		if (state == MORPHISM_STATE_ACTION) {
			//SceneManager::current->setCameraScale(2);

			//SceneManager::current->setCameraScale(1);
		}
	}

	if (sword_progress > 0) sword_progress--;

	if (isMoved)
	{

		bool isCollided = false;
		for (auto solid : SceneManager::current->boxes) { // Проходим по всем твёрдым предметам
			if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
				if (solid->flags & ENTITY_OBJECT) {
					Entity* e = (Entity*)solid;
					if (e->type == EntityID::Item) { // Если столкнулись с предметом
						auto event = new CollectItemEvent((ItemEntity*)e);
						OnEvent(event);
						delete event;
						e->flags ^= ENTITY_OBJECT;
						SceneManager::removeObject(e);
					}
					continue;
				}
				isCollided = true;
				break;
			}
		}
		if (isCollided) setPos(pre.x, pre.y);
		if (aabb.max.x >= SceneManager::current->width) setPos(pre.x, aabb.min.y);
		if (aabb.max.y >= SceneManager::current->height) setPos(aabb.min.x, pre.y);
		if (aabb.min.x <= 0) setPos(pre.x, aabb.min.y);
		if (aabb.min.y <= 0) setPos(aabb.min.x, pre.y);

		SceneManager::current->bindCamera(&aabb, 320, 196);

		SoundUI::Play("walk_stone");

	}
	else {
		walk_tick = 0;
		SoundUI::Stop("walk_stone");
	}
	if (tick % 2700 == 0 || (tick % 900 == 0 && xp < 5)) {
		xp = std::min(xp + 1, MAX_PLAYER_XP);
	}

	if (health <= 0) {
		SceneManager::PlayerGameOver(this);
	}
}


#include "entity/Arrow.h"
#include "entity/Bullet.h"

void Player::checkForAttack(__int64 tick)
{
	// Если наше оружие лук, то натягиваем его
	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (WType) weapon.type == WType::BOWS)
	{
		bow_progress = std::min(bow_progress + weapon.reserved, 100);
	}
	else {
		if (bow_progress > 20) { // Если натягивал лук 
			if (xp >= weapon.xp_cost) { // И если хватает xp на выстрел
				Vector2 mouse = SceneManager::GetMouseOnWorld();
				Vector2 center = { aabb.min.x + w / 2,aabb.min.y + h / 2 };
				float angle = MyVector2Angle({ center.x + 1, center.y }, mouse);
				float damage = weapon.damage * (bow_progress / 100.0f);
				SceneManager::addObject(
					new Arrow(center.x, center.y + 8, 0.5 + (1 * (bow_progress / 100.0f)), 
						angle, &weapon, this)
				);
				SoundUI::PlayOnce("bow");
				xp -= weapon.xp_cost; // Снимаем с игрока xp
			}
			else {
				if (remindAboutXp <= 1) remindAboutXp = 125; // Если нет xp, то напоминаем об этом
			}
		}
		bow_progress = 0;
	}
	if ((WType)weapon.type == WType::EXTRA && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		if (gun_progress == 0 && xp >= weapon.xp_cost) {
			Vector2 mouse = SceneManager::GetMouseOnWorld();
			Vector2 center = { aabb.min.x + w / 2,aabb.min.y + h / 2 };
			float angle = MyVector2Angle({ center.x + 1, center.y }, mouse);
			Bullet::SpawnPlayerBullet(weaponID, center.x, center.y + 8, angle, this);
			gun_progress = weapon.reserved;
			xp -= weapon.xp_cost;
		}
	}
	if (gun_progress > 0) {
		gun_progress--;
	}
		
}
