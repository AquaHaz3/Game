#include "GameLauncher.h"

#include <raylib.h>
#include "core/Box2D.h"
#include "model/Player.h"
#include "model/Block.h"
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

    current_scene->addPlayerToScene(new Player(96,96));
    current_scene->addObjectToScene(new Block(32,32, BlockID::WALL));
    current_scene->addObjectToScene(new Block(64,32, BlockID::WALL));
    current_scene->addObjectToScene(new Block(96,32, BlockID::WALL));
    current_scene->addObjectToScene(new Block(128,32, BlockID::WALL));
    current_scene->addObjectToScene(new Block(32,64, BlockID::WALL));
    current_scene->addObjectToScene(new Block(32,96, BlockID::WALL));
    current_scene->addObjectToScene(new Block(32,128, BlockID::WALL));
    current_scene->addObjectToScene(new Block(128,64, BlockID::WALL));
    current_scene->addObjectToScene(new Block(128,96, BlockID::WALL));
    current_scene->addObjectToScene(new Block(128,128, BlockID::WALL));
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

