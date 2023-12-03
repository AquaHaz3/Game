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
	ITEM_ENTITIY = 4,
	PLAYER = 5

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
		isHavePlayer = true;
		player_x = x;
		player_y = y;
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

private:

	SceneFile(const std::string& name) {
		this->path = "data\\"+name;
	}
	
	SceneFile() {
		this->path = "";
		objects = std::map<uint32_t, PrototypeGameObject*>();
		max_id = 0;
	}

	~SceneFile() {
		for (int i = 0; i < max_id; i++) {
			PrototypeGameObject* obj = objects[i];
			if (obj != 0) {
				delete obj;
			}
		}
		objects.clear();
	}

	std::string path;

private:

	std::map<uint32_t, PrototypeGameObject*> objects;
	int player_x;
	int player_y;

	bool isHavePlayer;

	int max_id = 0;

};
