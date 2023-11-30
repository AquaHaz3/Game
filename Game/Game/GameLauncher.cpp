#include "GameLauncher.h"

#include <raylib.h>
#include "core/Box2D.h"
#include "model/Player.h"
#include <thread>

static void __update_thread(GameLauncher* launcher);

GameLauncher::GameLauncher()
{

    const int screenWidth = 1024;
    const int screenHeight = 640;

    isGameAlive = true;

    current_scene = Scene(screenWidth, screenHeight);

    current_scene.setDebugGrid(true);
}

void GameLauncher::OnStart()
{
    current_scene.addObjectToScene(new Player(96,96));
}

void GameLauncher::Launch()
{
    InitWindow(current_scene.width, current_scene.height, "raylib [core] example - basic window");

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
        current_scene.Draw();
        EndDrawing();
    }
}

void GameLauncher::update()
{
    auto tick_delay = std::chrono::milliseconds(1);
    __int64 tick = 0;
    while (isGameAlive)
    {
        current_scene.Update(tick);
        tick++;
        std::this_thread::sleep_for(tick_delay);
    }
}

static void __update_thread(GameLauncher* launcher)
{
    launcher->update();
}

