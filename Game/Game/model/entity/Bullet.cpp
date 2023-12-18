#include "Bullet.h"

#include "../../core/Debug.h"
#include "../../events/ProjectileHitEvent.hpp"
#include "../../core/Scene.h"

#include <raymath.h>

Bullet::Bullet() : Entity(0, 0, 0, 0)
{
	this->angle = 0;
	this->speed = 0;
	this->health = 0;

	this->xSpeed = 0;
	this->ySpeed = 0;

	isOwnedByPlayer = false;
	isMoving = false;
	flags = flags & (~SOLID_OBJECT); // Убираем 'твердость' (чтобы игрок мог проходить сквозь)
	this->owner = 0;
	id = 0;
	type = 0;
}

Bullet::~Bullet()
{
	if (this->parent != nullptr) {
		this->parent->chain = nullptr;
	}
	if (this->chain != nullptr) {
		this->chain->parent = nullptr;
	}
}

Bullet::Bullet(int x, int y, BulletID id, float damage, float angle, int ricochet, Entity* owner)
	: Entity(x, y, 3, 3)
{
	isOwnedByPlayer = (owner->flags & PLAYER_OBJECT);

	this->owner = owner;
	this->angle = (angle / PI) * 180;
	if (this->angle >= 360) this->angle -= 360;
	if (this->angle < 0) this->angle += 360;
	this->speed = 1;
	this->health = damage;

	this->xSpeed = (float)cos(angle) * speed;
	this->ySpeed = (float)sin(angle) * speed;
	isMoving = true;
	flags = flags & (~SOLID_OBJECT); // Убираем 'твердость' (чтобы игрок мог проходить сквозь)
	this->owner = owner;

	this->id = (uint8_t) id;
	type = &bullet_types[this->id];
	this->ricochet = ricochet;


	float sound_pitch = (80.0 + (rand() % 40)) / 100.0f;
	float volume = 1.0f;
	if (!isOwnedByPlayer) {
		sound_pitch = 1.6f;
		volume = 0.8f;
	}
	if (type->sound != 0) {
		SetSoundVolume(*type->sound, volume);
		SetSoundPitch(*type->sound, sound_pitch);
		PlaySound(*type->sound);
	}
}

void Bullet::Draw()
{
	type->sprite->DrawPro(aabb.min.x, aabb.min.y, 32, 32, 16, 16, angle);
	if (chain != nullptr) {
		if(id == (int)BulletID::ELECTRO)
			DrawLine(aabb.min.x, aabb.min.y, chain->aabb.min.x, chain->aabb.min.y, {10,240,255,100});
		else if(id == (int) BulletID::ELECTRO_2)
			DrawLine(aabb.min.x, aabb.min.y, chain->aabb.min.x, chain->aabb.min.y, { 255,10,255,130 });
		else
			DrawLine(aabb.min.x, aabb.min.y, chain->aabb.min.x, chain->aabb.min.y, { 255,10, 20, 200 });
	}
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLines(aabb.min.x, aabb.min.y, 3, 3, RED);
		//DrawLineEx({ aabb.min.x - xSpeed * 8, aabb.min.y - ySpeed * 8 }, { aabb.min.x + xSpeed * 8, aabb.min.y + ySpeed * 8 }, 3, GREEN);
	}
}

void Bullet::Update(__int64 tick)
{
	if (!isMoving) return;
	moveBy(xSpeed, ySpeed);

	using namespace UtilAABB;

	if (aabb.min.x < 0 || aabb.min.y < 0 ||
		aabb.min.x > SceneManager::current->width ||
		aabb.min.y > SceneManager::current->height) {
		SceneManager::removeObject(this);
		isMoving = false;
		return;
	}

	if (tick % 3 == 2) {
		auto players = SceneManager::GetPlayers();
		for (auto solid : SceneManager::current->boxes) { // Проверяем не столкнулась ли стрела с чем-то?
			if (solid == this) continue;
			if ((solid->flags & ENTITY_OBJECT) && !isOwnedByPlayer) continue;
			if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
				auto e = new ProjectileHitEvent(this);
				solid->OnEvent(e);
				bool phaseThrow = e->phase;
				delete e;
				if (phaseThrow) continue;
				if (ricochet <= 0 || (solid->flags & ENTITY_OBJECT)) {
					isMoving = false;
					SceneManager::removeObject(this);
				}
				else {
					Side side = getCollisionSide({ aabb.min.x - xSpeed * 8, aabb.min.y - ySpeed * 8}, { aabb.min.x + xSpeed * 8, aabb.min.y + ySpeed * 8}, &solid->aabb);
					if (side == Side::LEFT || side == Side::RIGHT) angle = 180 - angle;
					if (side == Side::TOP || side == Side::BOTTOM) angle = 360 - angle;
					if (side == Side::UNKNOWN) continue;
					ricochet--;
					if (this->angle >= 360) this->angle -= 360;
					if (this->angle < 0) this->angle += 360;
					float _angle = (angle / 180) * PI;
					this->xSpeed = (float)cos(_angle) * speed;
					this->ySpeed = (float)sin(_angle) * speed;
					moveBy(xSpeed, ySpeed);
					moveBy(xSpeed, ySpeed);
				}
			}
			if (this->chain != nullptr && tick % 27 == 2) {
				if (UtilAABB::isIntersectsBox(&solid->aabb, aabb.min, chain->aabb.min)) {
					float _health = health; health = rand() % 2 + 1 + (id == (int)BulletID::ELECTRO_RED);
					auto e = new ProjectileHitEvent(this);
					solid->OnEvent(e);
					delete e; health = _health;
				}
			}
		}
		if (!isOwnedByPlayer) {
			for (auto player : players) {
				if (player == owner) continue;
				if (UtilAABB::isOverlap(&aabb, &player->aabb)) {
					if (player->flags & ENTITY_OBJECT) {
						auto e = new ProjectileHitEvent(this);
						e->xSpeed = xSpeed;
						e->ySpeed = ySpeed;
						player->OnEvent(e);
						delete e;
						SceneManager::removeObject(this);
						return;
					}
					isMoving = false;
					return;
				}
				if (this->chain != nullptr && tick % 27 == 2) {
					if (UtilAABB::isIntersectsBox(&player->aabb, aabb.min, chain->aabb.min)) {
						float _health = health; health = 0.5;
						auto e = new ProjectileHitEvent(this);
						player->OnEvent(e);
						delete e; health = _health;
					}
				}
			}
		}
	}
	else {
		if (this->chain != nullptr) {
			float d = Vector2Distance(aabb.min, chain->aabb.min);
			if (d > 192 && !isOwnedByPlayer) {
				this->chain->parent = nullptr;
				this->chain = nullptr;
			}
		}
	}
}

bool Bullet::isExplosive() {
	if (type != 0)
		return type->isExplosive;
	else
		return false;
}

void Bullet::OnEvent(Event* event)
{
}

std::vector<BulletType> Bullet::bullet_types = std::vector<BulletType>(20);

void Bullet::InitBullets()
{
	addBullet(BulletID::RED_MAGIC, "weapon/bullet/red_magic.png", false, "magic");
	addBullet(BulletID::PURPLE_MAGIC, "weapon/bullet/purple_magic.png", false, "magic");
	addBullet(BulletID::GREEN_ENERGY, "weapon/bullet/green_energy.png", false, "magic");
	addBullet(BulletID::ELECTRO, "weapon/bullet/electro.png", false, "electro");
	addBullet(BulletID::ELECTRO_2, "weapon/bullet/electro2.png", false, "electro");
	addBullet(BulletID::ELECTRO_RED, "weapon/bullet/electro3.png", false, "electro");
	addBullet(BulletID::GUN_BULLET, "weapon/bullet/bullet.png", false, "gun");
	addBullet(BulletID::RED_ENERGY, "weapon/bullet/red_energy.png", false, "magic");
}

void Bullet::SpawnPlayerBullet(uint8_t weapon_id, int x, int y, float angle, Entity* owner)
{
	if (Item::weapons.size() <= weapon_id) return;
	if (weapon_id < 0) return;

	Weapon* weapon = &Item::weapons[weapon_id];
	if (weapon->type != (int)WType::EXTRA) {
		return;
	}

	using enum BulletID;

	
	if (weapon_id == (uint8_t) ItemID::RED_WAND) {
		SceneManager::addObject(new Bullet(x,y, PURPLE_MAGIC, weapon->damage, angle, 0, owner));
		char power = (rand() % 4 > 0);
		SceneManager::addObject(new Bullet(x,y, PURPLE_MAGIC, abs(weapon->damage - power), angle+0.12, 0, owner));
		SceneManager::addObject(new Bullet(x,y, PURPLE_MAGIC, abs(weapon->damage - power), angle-0.12, 0, owner));
	}	

	if (weapon_id == (uint8_t) ItemID::GREEN_WAND) {
		SceneManager::addObject(new Bullet(x,y, GREEN_ENERGY, weapon->damage, angle, 3, owner));
	}

	if (weapon_id == (uint8_t) ItemID::ELECTRO_STAFF) {
		auto main = new Bullet(x, y, ELECTRO, weapon->damage, angle - PI / 3, 3, owner);
		auto pair = new Bullet(x, y, ELECTRO, weapon->damage, angle + PI / 3, 3, owner);
		auto pair2 = new Bullet(x, y, ELECTRO, weapon->damage, angle, 1, owner);
		main->addBulletToChain(pair); pair->addBulletToChain(pair2); 
		SceneManager::addObject(main);
		SceneManager::addObject(pair); SceneManager::addObject(pair2);
	}

	if (weapon_id == (uint8_t) ItemID::RED_STAFF) {
		auto main = new Bullet(x, y, ELECTRO_2, weapon->damage, angle - PI / 4, 3, owner);
		auto pair = new Bullet(x, y, ELECTRO_2, weapon->damage, angle + PI / 4, 3, owner);
		auto pair2 = new Bullet(x, y, ELECTRO_2, weapon->damage, angle, 3, owner);
		main->addBulletToChain(pair); pair->addBulletToChain(pair2); 
		SceneManager::addObject(main);   
		SceneManager::addObject(pair); SceneManager::addObject(pair2);
	}

	if (weapon_id == (uint8_t) ItemID::GUN) {
		auto bullet = new Bullet(x, y, BulletID::GUN_BULLET, weapon->damage, angle, 1, owner);
		bullet->speed = 2;
		SceneManager::addObject(bullet);
	}

}

//#include "Mob.h"

void Bullet::SpawnMobBullet(EntityID id, int x, int y, float angle, Entity* owner)
{

	using enum BulletID;

	//Mob* mob = (Mob*)owner;

	if (id == EntityID::Dark) {
		SceneManager::addObject(new Bullet(x, y, RED_MAGIC, 1, angle, 0, owner));
		SceneManager::addObject(new Bullet(x, y, RED_MAGIC, 0.5, angle + 0.12, 0, owner));
		SceneManager::addObject(new Bullet(x, y, RED_MAGIC, 0.5, angle - 0.12, 0, owner));
	}
	
	if (id == EntityID::Ghost) {
		SceneManager::addObject(new Bullet(x, y, RED_ENERGY, 1, angle, 2, owner));
	}

	if (id == EntityID::Bat && rand() % 3 == 0) {
		SceneManager::addObject(new Bullet(x, y, RED_MAGIC, 0.5, angle, 0, owner));
	}

	if (id == EntityID::Agent) {
		auto main = new Bullet(x, y, ELECTRO_RED, 0.5, angle - PI / 3, 1, owner);
		auto pair = new Bullet(x, y, ELECTRO_RED, 0.5, angle + PI / 3, 1, owner);
		auto pair2 = new Bullet(x, y, ELECTRO_RED, 1, angle, 1, owner);
		main->addBulletToChain(pair); pair->addBulletToChain(pair2);
		SceneManager::addObject(main);
		SceneManager::addObject(pair); SceneManager::addObject(pair2);
	}

	if (id == EntityID::Glow) {
		if (rand() % 2 == 0) {
			SceneManager::addObject(new Bullet(x, y, RED_MAGIC, 1, angle - 0.07, 2, owner));
			SceneManager::addObject(new Bullet(x, y, RED_MAGIC, 0.5, angle + 0.07, 2, owner));
		}
		else {
			SceneManager::addObject(new Bullet(x, y, RED_ENERGY, 1, angle, 0, owner));
		}
	}
}

void Bullet::addBullet(BulletID id, std::string tex_path, bool isExplosive, const std::string& sound)
{
	auto ref = SpriteLoader::GetSprite(tex_path);
	bullet_types[(int)id] = BulletType(ref, isExplosive, sound);
}
