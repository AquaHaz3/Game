#pragma once

#include "../core/Box2D.h"
#include "../core/Sprite.h"

enum class EntityID {

	Unknown = 0,
	Player = 1,
	Item = 2,
	Ghost = 3,
	Bat = 4,

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

	EntityModel() {}
	EntityModel(EntityID id, short hp)
	{
		this->id = id;
		health = hp;
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

	static void addEntity(EntityID id, std::string tex_path, short health);

public:

	static void InitEntities();

};

