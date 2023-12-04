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
	isInit = false;
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
	isInit = false;
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

	BeforeDraw();

	isRenderTime = true;

	for (auto obj : objects) {
		obj->Draw();
	}

	AfterDraw();

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
			if(to_r->finger_print == 0b1100110)
				delete to_r;
		}
		toRemove.clear();
		//toRemove.shrink_to_fit();
	}
}

void Scene::Dispose()
{
	for (auto obj : objects) {
		if (obj != player.get()) {
			delete obj;
		}
	}
	objects.clear();
	particles.clear();
	boxes.clear();
	toRemove.clear();
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
	Dispose();
	printf("[Game] [Scene] Dispose: End!");
};

#include <thread>

SceneManager* SceneManager::instance = nullptr;
bool SceneManager::isPaused = false;
bool SceneManager::isDrawed = false;
bool SceneManager::isReadyToPause = false;
Scene* SceneManager::current = nullptr;

SceneManager* SceneManager::Instance()
{
	if (instance == nullptr) {
		instance = new SceneManager();
	}
	return instance;
}

void SceneManager::addObject(GameObject* object)
{
	if (current == nullptr) return;
	current->addObjectToScene(object);
}

void SceneManager::addPlayer(Player* player)
{
	if (current == nullptr) return;
	current->addPlayerToScene(player);
}

void SceneManager::removeObject(GameObject* object)
{
	if (current == nullptr) return;
	current->removeObject(object);
}

void SceneManager::addParticle(Particle* object)
{
	if (current == nullptr) return;
	current->addParticle(object);
}

void SceneManager::Pause()
{
	isReadyToPause = true;
}

void SceneManager::Resume()
{
	if (!isPaused) return;
	isPaused = false;
	printf("[Game] [SceneManager] Resume!\n");
}

void SceneManager::Start()
{
	if (current == nullptr) return;
	if (!current->isInit) current->OnStart();
	isPaused = false;
	isReadyToChange = false;
}

void SceneManager::ChangeScene(int index)
{
	if (scenes[index].get() != 0) {
		Pause();
		isReadyToChange = true;
		current_index = index;
	}
}

void SceneManager::StopAndExit()
{
	Pause();
	isReadyToExit = true;
}

int SceneManager::AddScene(Scene* scene)
{
	scenes.push_back(std::shared_ptr<Scene>(scene));
	return scenes.size() - 1;
}

void SceneManager::pauseAll()
{
	if (isPaused) return;
	isPaused = true;
	auto tick_delay = std::chrono::milliseconds(1);
	for (int i = 0; i < 8000; i++) {
		std::this_thread::sleep_for(tick_delay);
		if (i % 1000) printf("[%d && %d]\n", isDrawed, isReadyToPause);
		if (isDrawed && isReadyToPause) break;
	}
	if ((!isDrawed) || (!isReadyToPause)) {
		debug_util::panicf("[ERROR] Scene [render or update] tick was too long > 8 seconds\n");
	}
	printf("[Game] [SceneManager] Pause!\n");

	if (isReadyToChange) change();
	if (isReadyToExit) exit(0);
}

void SceneManager::change()
{
	current->Dispose();
	current = scenes[current_index].get();
	if (!current->isInit) current->OnStart();
	isReadyToChange = false;
	Resume();
}

