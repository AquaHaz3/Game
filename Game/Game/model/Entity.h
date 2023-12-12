#pragma once

#include "../core/Box2D.h"
#include "../core/Sprite.h"

enum class EntityID {

	Unknown = 0,
	Player = 1,
	Item = 2,
	Ghost = 3,
	Bat = 4,
	Dark,

	__lastEntity
};

enum class Direction {
	RIGHT = 0,
	DOWN = 1,
	LEFT = 2,
	UP = 3,
};

struct EntityModel {

	EntityID id;
	short health;
	bool isDistanceBattle;
	bool solid;
	uint16_t detectRadius;
	uint16_t idleRadius;

	EntityModel() {}
	EntityModel(EntityID id, short hp, bool isDistance, bool solid, uint16_t detectR, uint16_t idleR)
	{
		this->id = id;
		health = hp;
		isDistanceBattle = isDistance;
		this->solid = solid;
		this->detectRadius = detectR;
		this->idleRadius = idleR;
	}

};

class Entity : public Box2D
{

public:

	int health;
	float speed;
	EntityID type;


public:

	Entity(int x, int y, int w, int h);
	Entity(int x, int y, int w, int h, EntityID type);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

protected:

	friend class EditorUI;

	Sprite texture;

	static std::vector<Texture2D> textures;
	static std::vector<EntityModel> models;

	static void addEntity(
		EntityID id, std::string tex_path,
		short hp, bool isDistance, bool solid, uint16_t detectR, uint16_t idleR);

public:

	static void InitEntities();

};

