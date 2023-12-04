#include "GameLauncher.h"

#include <raylib.h>
#include "core/Box2D.h"
#include "model/Player.h"
#include "model/Block.h"
#include "model/Item.h"
#include "model/entity/Arrow.h"
#include "model/Wall.h"
#include "model/ItemEntity.h"
#include "model/Background.h"
#include "core/Sprite.h"
#include <thread>

#include "scenes/GameScene.hpp"
#include "scenes/MenuScene.hpp"
#include "scenes/EditorScene.hpp"

static void __update_thread(GameLauncher* launcher);

GameLauncher::GameLauncher()
{

    const int screenWidth = 1024;
    const int screenHeight = 640;

    isGameAlive = true;

    auto start_scene = std::shared_ptr<Scene>(new MenuScene(screenWidth, screenHeight));
    start_scene->setDebugGrid(false);

    SceneManager::Instance()->setStartScene(start_scene);
    SceneManager::Instance()->AddScene(new GameScene(screenWidth, screenHeight, "base"));
    SceneManager::Instance()->AddScene(new EditorScene(screenWidth, screenHeight));
}

void GameLauncher::OnStart()
{

    Block::InitBlocks();
    Item::InitItems();
    Arrow::InitArrows();

    SpriteLoader::LoadInGameSprites();

    SceneManager::Instance()->Start();
}

void GameLauncher::Launch()
{

    InitWindow(SceneManager::current->width, SceneManager::current->height, "The Game");

    SetTargetFPS(144);
    OnStart();
    
    std::thread th1(__update_thread, this);
    draw();

    CloseWindow();
    isGameAlive = false;
    th1.join();
}

void GameLauncher::draw()
{

    while (!WindowShouldClose())    // Detect window close button or ESC key
    { 
        BeginDrawing();
        SceneManager::DrawCurrentScene();
        EndDrawing();
    }
}

void GameLauncher::update()
{
    auto tick_delay = std::chrono::milliseconds(1);
    __int64 tick = 0;
    while (isGameAlive)
    {
        SceneManager::UpdateCurrentScene(tick);
        tick++;
        std::this_thread::sleep_for(tick_delay);
    }
}

static void __update_thread(GameLauncher* launcher)
{
    launcher->update();
}

