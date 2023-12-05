#pragma once

#include <string>
#include "Scene.h"
#include "../model/Player.h"
#include <map>

#define SCENE_FILE_EXTENSION ".scene"
#define STRUCT_FILE_EXTENSION ".struct"

enum class SceneObjectType {

	BACKGROUND = 0,
	WALL = 1,
	BLOCK = 2,
	ITEM_ENTITIY = 3,
	PLAYER = 4

};

struct PrototypeGameObject {

	uint32_t id;
	uint8_t type;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	uint32_t ord;
	uint8_t reserved_r;
	uint8_t reserved_g;
	uint8_t reserved_b;
	uint8_t reserved_a;

};

class SceneFile
{

public:

	Scene* BuildScene();
	void SaveScene(std::string name, int width, int height);

	void setPlayer(int x, int y) {
		isHavePlayer = (x >= 0);
		player_x = x;
		player_y = y;
	}

	Vector2 getPlayerPos() {
		return {(float)player_x, (float) player_y};
	}
	
	void addObject(PrototypeGameObject* object) {
		objects.emplace(max_id, object);
		max_id++;
	}

	void removeObject(int index) {
		auto obj = objects[index];
		if (obj != 0) {
			delete obj;
		}
		objects.erase(index);
	}

	void removeObjectInBox(AABB* box);

	SceneFile(const std::string& name) {
		this->path = "data\\" + name;
	}

	SceneFile() {
		this->path = "";
		objects = std::map<uint32_t, PrototypeGameObject*>();
		max_id = 0;
		player_x = -1;
		player_y = -1;
	}

	~SceneFile() {
		for (const auto& kv : objects) {
			PrototypeGameObject* obj = kv.second;
			if (obj != 0) {
				delete obj;
			}
		}
		objects.clear();
	}

	static GameObject* brushGameObjectFactory(PrototypeGameObject* brush);
	static std::string getTypeName(SceneObjectType type);
	static int getObjectDefaultID(SceneObjectType type);
	static int getObjectLastID(SceneObjectType type);

private:

	std::string path;

private:

	std::map<uint32_t, PrototypeGameObject*> objects;
	int player_x = -1;
	int player_y = -1;

	bool isHavePlayer;

	int max_id = 0;

};

