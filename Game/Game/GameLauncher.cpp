#include "GameLauncher.h"

#include <raylib.h>
#include "core/Box2D.h"
#include "model/Player.h"
#include "model/Block.h"
#include "model/Item.h"
#include "model/Wall.h"
#include "model/ItemEntity.h"
#include <thread>

static void __update_thread(GameLauncher* launcher);

SceneRef GameLauncher::current_scene = 0;

GameLauncher::GameLauncher()
{

    const int screenWidth = 1024;
    const int screenHeight = 640;

    isGameAlive = true;

    current_scene = std::shared_ptr<Scene>(new Scene(screenWidth, screenHeight));

    current_scene->setDebugGrid(true);
}

void GameLauncher::OnStart()
{

    Block::InitBlocks();
    Item::InitItems();

    current_scene->addPlayerToScene(new Player(96,96));
    current_scene->addObjectToScene(new Wall(32, 32, 288, 64, BlockID::WALL));
    current_scene->addObjectToScene(new Wall(32, 64, 64, 320, BlockID::WALL));
    current_scene->addObjectToScene(new Wall(32, 320, 288, 352, BlockID::WALL));
    current_scene->addObjectToScene(new Wall(256, 64, 288, 128, BlockID::WALL));
    current_scene->addObjectToScene(new Wall(256, 192, 288, 320, BlockID::WALL));

    current_scene->addObjectToScene(new ItemEntity(128, 160, ItemID::POTION_HEAL));
    current_scene->addObjectToScene(new ItemEntity(224, 288, ItemID::POTION_XP));
}

void GameLauncher::Launch()
{
    InitWindow(current_scene->width, current_scene->height, "raylib [core] example - basic window");

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
        current_scene->Draw();
        EndDrawing();
    }
}

void GameLauncher::update()
{
    auto tick_delay = std::chrono::milliseconds(1);
    __int64 tick = 0;
    while (isGameAlive)
    {
        current_scene->Update(tick);
        tick++;
        std::this_thread::sleep_for(tick_delay);
    }
}

static void __update_thread(GameLauncher* launcher)
{
    launcher->update();
}

