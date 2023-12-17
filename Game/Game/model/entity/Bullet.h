#pragma once

#include "../Entity.h"
#include <vector>
#include "../Item.h"
#include "../ui/SoundUI.h"

enum class BulletID {

	RED_MAGIC = 0,
	PURPLE_MAGIC,
	GREEN_ENERGY,
	ELECTRO,
	GUN_BULLET,
	ELECTRO_2,
	ELECTRO_RED,
	RED_ENERGY,

};

struct BulletType {

	SpriteRef sprite;
	Sound* sound;
	bool isExplosive;

	BulletType() {};

	BulletType(
		const SpriteRef& sprite,
		bool isExplosive,
		const std::string& soundName
	) {
		this->sprite = sprite;
		this->isExplosive = isExplosive;
		sound = &SoundUI::GetSound(soundName);
	};
};

class Bullet : public Entity
{

public:

	Bullet();
	~Bullet();

	Bullet(int x, int y, BulletID id, float damage, float angle, int ricochet, Entity* owner);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

	float angle;
	float xSpeed;
	float ySpeed;

public:

	static void InitBullets();
	static std::vector<BulletType> bullet_types;

	Entity* owner;
	uint8_t id;
	Bullet* chain = nullptr;
	Bullet* parent = nullptr;

	bool isOwnedByPlayer;
	bool isExplosive();

	static void SpawnPlayerBullet(uint8_t weapon_id, int x, int y, float angle, Entity* owner);
	static void SpawnMobBullet(EntityID id, int x, int y, float angle, Entity* owner);

private:

	static void addBullet(BulletID id, std::string tex_path, bool isExplosive, const std::string& soundName);

	bool isMoving;
	BulletType* type;
	uint8_t ricochet;

protected:

	void addBulletToChain(Bullet* bullet) {
		this->chain = bullet;
		bullet->parent = this;
	}

};
