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
	PLAYER = 4,
	MOB = 5

};

struct PrototypeGameObject {

	uint32_t id;
	uint8_t type;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	uint16_t ord;
	Color color;

	PrototypeGameObject() {
		x = 0; y = 0; w = 0; h = 0;
		ord = 0; id = 0; type = 0;
		color = { 0,0,0,0 };
	}

	PrototypeGameObject(int x, int y, int w, int h, int ord, int type, Color color) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->ord = ord;
		this->type = type;
		this->color = color;
		this->id = 0;
	}

};

/*
    Записывает или считывает сцену из файла (*.scene)
*/

class SceneFile
{

public:

	void InitScene(Scene* scene);
	void SaveScene(std::string name, int camWidth, int camHeight);

	void LoadSceneForEditor(Scene* sceneEditor, SceneFile* context);

	void setPlayer(int x, int y) {
		isHavePlayer = (x >= 0);
		player_x = x;
		player_y = y;
	}

	Vector2 getPlayerPos() {
		return {(float)player_x, (float) player_y};
	}
	
	void addObject(PrototypeGameObject* object) {
		object->id = max_id;
		if (object->type == (int)SceneObjectType::BACKGROUND) {
			backgroundObjects.emplace(max_id, object);
		}
		else {
			objects.emplace(max_id, object);
		}
		max_id++;
	}

	void removeObject(int index) {
		if (backgroundObjects.count(index) > 0) {
			auto obj = backgroundObjects[index];
			if (obj != 0) delete obj;
			backgroundObjects.erase(index);
			return;
		}
		auto obj = objects[index];
		if (obj != 0) delete obj;
		objects.erase(index);
	}

	void removeObjectInBox(AABB* box);

	SceneFile(const std::string& name) {
		this->path = "data\\" + name;
	}

	SceneFile() {
		this->path = "";
		objects = std::map<uint32_t, PrototypeGameObject*>();
		backgroundObjects = std::map<uint32_t, PrototypeGameObject*>();
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
		for (const auto& kv : backgroundObjects) {
			PrototypeGameObject* obj = kv.second;
			if (obj != 0) {
				delete obj;
			}
		}
		path.~basic_string();
		printf("[Memory] delete SceneFile\n");
		objects.clear();
		backgroundObjects.clear();
	}

	static GameObject* brushGameObjectFactory(PrototypeGameObject* brush);
	static std::string getTypeName(SceneObjectType type);
	static int getObjectDefaultID(SceneObjectType type);
	static int getObjectLastID(SceneObjectType type);

private:

	std::string path;

private:

	std::map<uint32_t, PrototypeGameObject*> objects;
	std::map<uint32_t, PrototypeGameObject*> backgroundObjects;
	int player_x = -1;
	int player_y = -1;

	bool isHavePlayer;

	int max_id = 0;

};

