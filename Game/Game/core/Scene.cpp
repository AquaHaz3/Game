#include "Scene.h"
#include "../core/Debug.h"


static Color COLOR_GRID = {50,50,50,255};

Scene::Scene() 
{
	objects = std::list<GameObject*>();
	boxes = std::list<Box2D*>();
	toRemove = std::vector<GameObject*>();
	particles = std::list<Particle*>();
	playerContainer = std::vector<Entity*>();
	player = 0;
	isInit = false;
	cam_scale = 1;
	cameraZoom = Morphism<float>(&cam_scale, 1.5f, 1.0f, 0.03125f, [](){});
}

Scene::Scene(int width, int height)
{
	objects = std::list<GameObject*>();
	boxes = std::list<Box2D*>();
	toRemove = std::vector<GameObject*>();
	particles = std::list<Particle*>();
	playerContainer = std::vector<Entity*>();
	this->camWidth = SceneManager::Instance()->getCameraWidth();
	this->camHeight = SceneManager::Instance()->getCameraHeight();
	this->width = width;
	this->height = height;
	player = 0;
	isStatic = false;
	background = {60,60,65,255};
	isInit = false;
	cam_scale = 1;
	cameraZoom = Morphism<float>(&cam_scale, 1.5f, 1.0f, 0.03125f,
		[this]() {
			SceneManager::current->bindCamera(&player->aabb, 320, 196);
		}
	);
}

void Scene::addPlayerToScene(Player* player)
{
	if (this->player != nullptr) return;
	this->player = player;
	objects.push_back(player);
	playerContainer.push_back(player);
}

void Scene::addObjectToScene(GameObject* obj)
{
	if (obj == 0) return;
	objects.push_back(obj);
	if (obj->flags & SOLID_OBJECT) boxes.push_back((Box2D*)obj);
}

#include <rlgl.h>

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

	rlPushMatrix();
	rlScalef(cam_scale, cam_scale, 1);
	rlTranslatef(cam_x, cam_y, 0);

	int from_i = (int)(-cam_y) / 32;
	int from_j = (int)(-cam_x) / 32;

	if (isDebugGridOn) {
		for (int i = from_i; i < height / 32 + from_i; i++)
			DrawLine(0, i * 32, width, i * 32, COLOR_GRID);
		for (int j = from_j; j < width / 32 + from_j; j++)
			DrawLine(j * 32, 0, j * 32, height, COLOR_GRID);
	}

	isRenderTime = true;

	for (auto obj : objects) {
		if (obj->flags & SOLID_OBJECT) {
			Box2D* box = (Box2D*)obj;
			if (
				box->aabb.max.x + cam_x > 0
				&& box->aabb.max.y + cam_y > 0
				&& box->aabb.min.x + cam_x <= camWidth
				&& box->aabb.min.y + cam_y <= camHeight
				) {
				obj->Draw();
			}
		}
		else {
			obj->Draw();
		}
	}

	AfterDraw();

	rlPopMatrix();

	if (debug_util::isDebugBoxes()) {
		//DrawRectangle(-cam_x, -cam_y, camWidth, camHeight, { 0, 248, 88, 60 });
		auto about_particles = "Particles: " + std::to_string(particles.size());
		auto about_objects = "Objects: " + std::to_string(objects.size());
		auto about_boxes = "Boxes(Solid): " + std::to_string(boxes.size());
		auto fps = "FPS: " + std::to_string(GetFPS());
		auto camera = "Camera: {" + std::to_string((int)cam_x) + ", " + std::to_string((int)cam_y) + "}";
		DrawText(about_objects.c_str(), camWidth - 198, 10, 20, WHITE);
		DrawText(about_boxes.c_str(), camWidth - 198, 34, 20, WHITE);
		DrawText(about_particles.c_str(), camWidth - 198, 58, 20, WHITE);
		DrawText(fps.c_str(), camWidth - 198, 82, 20, WHITE);
		DrawText(camera.c_str(), camWidth - 198, 106, 10, WHITE);
		DrawText(RAYLIB_VERSION, camWidth - 198, 118, 10, WHITE);
	}

	isRenderTime = false;
}

#include "AABB.h"

void Scene::Update(__int64 tick)
{

	notifications = 0;

	for (auto obj : objects) {
		if (obj->flags & SOLID_OBJECT) {
			Box2D* box = (Box2D*)obj;
			if (
				box->aabb.max.x + cam_x > -64
				&& box->aabb.max.y + cam_y > -64
				&& box->aabb.min.x + cam_x <= camWidth + 64
				&& box->aabb.min.y + cam_y <= camHeight + 64
				) {
				obj->Update(tick);
			}
		}
		else {
			obj->Update(tick);
		}
	}

	for (auto p : particles) {
		if (p->isAlive == false) toRemove.push_back(p);
	}

	AfterUpdate(tick);

	if (toRemove.size() > 0 && isRenderTime == false) {
		for (auto to_r : toRemove) 
		{
			objects.remove(to_r);
			if(to_r->flags & SOLID_OBJECT) boxes.remove((Box2D*) to_r);
			if(to_r->flags & PARTICLE_OBJECT) particles.remove((Particle*) to_r);
			if (to_r == player) {
				player = nullptr;
			}
			if (to_r->object_id == 0b11001100) {
				delete to_r;
			}
		}
		toRemove.clear();
		//toRemove.shrink_to_fit();
	}
}

void Scene::Dispose()
{
	OnDispose();
	for (auto obj : objects) {
		if (obj != player) {
			delete obj;
		}
	}
	objects.clear();
	particles.clear();
	boxes.clear();
	toRemove.clear();
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}
}

void Scene::bindCamera(AABB* box, int borderWidth, int borderHeight)
{

	int camWidth = SceneManager::current->camWidth / SceneManager::current->cam_scale;
	int camHeight = SceneManager::current->camHeight / SceneManager::current->cam_scale;
	int borderW = borderWidth / SceneManager::current->cam_scale;
	int borderH = borderHeight / SceneManager::current->cam_scale;

	int dx = box->max.x - (camWidth - borderW);
	int ddx = cam_x + box->min.x - borderW;
	if(abs(cam_x) < dx) cam_x = -dx;
	if (ddx < 0 && ddx >= cam_x) cam_x = cam_x - ddx;

	int dy = box->max.y - (camHeight - borderH);
	int ddy = cam_y + box->min.y - borderH;
	if (abs(cam_y) < dy) cam_y = -dy;
	if (ddy < 0 && ddy >= cam_y) cam_y = cam_y - ddy;
}

std::vector<Entity*>& Scene::getPlayerContainer()
{
	return playerContainer;
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
	if (particles.size() > 70) {
		delete particle;
		return;
	}
	particles.push_back(particle);
	objects.push_back(particle);
}

Scene::~Scene() 
{
	printf("[Game] [Scene] Dispose: Start!\n");
	Dispose();
	printf("[Game] [Scene] Dispose: End!\n");
};

#include <thread>

SceneManager* SceneManager::instance = nullptr;
bool SceneManager::isPaused = false;
bool SceneManager::isDrawed = false;
bool SceneManager::isReadyToPause = false;
bool SceneManager::isCameraOverlay = false;
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

void SceneManager::ChangeSceneAndDispose(int index, Scene* construct)
{
	if (scenes[current_index].get() != 0) {
		scenes[current_index].reset();
		if (construct != nullptr) {
			scenes[current_index] = std::shared_ptr<Scene>(construct);
		}
		current = nullptr;
	}
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
	return (int)scenes.size() - 1;
}

Vector2 SceneManager::GetMouseOnWorld()
{
	Vector2 mouse = GetMousePosition();
	mouse.x /= current->cam_scale;
	mouse.y /= current->cam_scale;
	return { (mouse.x - current->cam_x), (mouse.y - current->cam_y)};
}

void SceneManager::ProjectToCamera(Vector2& pos)
{
	if (isCameraOverlay) {
		pos.x -= SceneManager::current->cam_x* SceneManager::current->cam_scale;
		pos.y -= SceneManager::current->cam_y* SceneManager::current->cam_scale;
	}
	else {
		pos.x -= SceneManager::current->cam_x;
		pos.y -= SceneManager::current->cam_y;
	}
}

void SceneManager::ProjectToCamera(Rectangle& rec)
{
	rec.x -= SceneManager::current->cam_x;
	rec.y -= SceneManager::current->cam_y;

}


void SceneManager::BeginCameraOverlay()
{
	if (isCameraOverlay) return;
	isCameraOverlay = true;
	rlScalef(1 / current->cam_scale, 1 / current->cam_scale, 1);
}

void SceneManager::EndCameraOverlay()
{
	if (!isCameraOverlay) return;
	rlScalef(current->cam_scale, current->cam_scale, 1);
	isCameraOverlay = false;
}

bool SceneManager::isSceneStatic()
{
	return current->isStatic;
}

void SceneManager::PlayerGameOver(Player* player)
{
	SceneManager::removeObject(player);
	instance->Pause();
}

std::vector<Entity*>& SceneManager::GetPlayers()
{
	return current->getPlayerContainer();
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
	if(current != nullptr) current->Dispose();
	current = scenes[current_index].get();
	if (!current->isInit) current->OnStart();
	isReadyToChange = false;
	Resume();
}

