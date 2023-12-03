#include "Scene.h"
#include "../core/Debug.h"

static Color COLOR_GRID = {50,50,50,255};

Scene::Scene() 
{
	objects = std::list<GameObject*>();
	boxes = std::list<Box2D*>();
	toRemove = std::vector<GameObject*>();
	particles = std::list<Particle*>();
	player = 0;
}

Scene::Scene(int width, int height)
{
	objects = std::list<GameObject*>();
	boxes = std::list<Box2D*>();
	toRemove = std::vector<GameObject*>();
	particles = std::list<Particle*>();
	this->width = width;
	this->height = height;
	background = DARKGRAY;
}

void Scene::addPlayerToScene(Player* player)
{
	this->player = std::shared_ptr<Player>(player);
	objects.push_back(player);
}

void Scene::addObjectToScene(GameObject* obj)
{
	objects.push_back(obj);
	if (obj->flags & SOLID_OBJECT) boxes.push_back((Box2D*)obj);
}

void Scene::Draw()
{

	if (toRemove.size() > 0) {
		isRenderTime = false;
		return;
	}

	ClearBackground(background);


	if (IsKeyPressed(KEY_SPACE)) {
		debug_util::switchDebugBoxes();
	}

	if (isDebugGridOn) {
		for (int i = 0; i < height/ 32; i++) {
			DrawLine(0, i*32, width, i*32, COLOR_GRID);
		}
		for (int i = 0; i < width/ 32; i++) {
			DrawLine(i*32, 0, i*32, height, COLOR_GRID);
		}
	}

	if (debug_util::isDebugBoxes()) {
		auto about_particles = "Particles: " + std::to_string(particles.size());
		auto about_objects = "Objects: " + std::to_string(objects.size());
		auto about_boxes = "Boxes(Solid): " + std::to_string(boxes.size());
		auto fps = "FPS: " + std::to_string(GetFPS());
		DrawText(about_objects.c_str(), width - 198, 10, 20, WHITE);
		DrawText(about_boxes.c_str(), width - 198, 34, 20, WHITE);
		DrawText(about_particles.c_str(), width - 198, 58, 20, WHITE);
		DrawText(fps.c_str(), width - 198, 82, 20, WHITE);

	}

	isRenderTime = true;

	for (auto obj : objects) {
		obj->Draw();
	}

	isRenderTime = false;
}

#include "AABB.h"

void Scene::Update(__int64 tick)
{

	for (auto obj : objects) {
		obj->Update(tick);
	}

	for (auto p : particles) {
		if (p->isAlive == false) toRemove.push_back(p);
	}

	if (toRemove.size() > 0 && isRenderTime == false) {
		for (auto to_r : toRemove) 
		{
			objects.remove(to_r);
			if(to_r->flags & SOLID_OBJECT) boxes.remove((Box2D*) to_r);
			if(to_r->flags & PARTICLE_OBJECT) particles.remove((Particle*) to_r);
			delete to_r;
		}
		toRemove.clear();
		//toRemove.shrink_to_fit();
	}
}

void Scene::setDebugGrid(bool isActive)
{
	isDebugGridOn = isActive;
}

void Scene::removeObject(GameObject* obj)
{
	toRemove.push_back(obj);
}

void Scene::addParticle(Particle* particle)
{
	if (particles.size() > 50) {
		delete particle;
		return;
	}
	particles.push_back(particle);
	objects.push_back(particle);
}

Scene::~Scene() 
{
	printf("[Game] [Scene] Dispose: Start!");
	for (auto obj : objects) {
		if (obj != player.get()) {
			delete obj;
		}
	}
	objects.clear();
	printf("[Game] [Scene] Dispose: End!");
};
