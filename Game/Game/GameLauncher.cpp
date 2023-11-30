#include "GameLauncher.h"

#include <raylib.h>
#include "core/Box2D.h"

GameLauncher::GameLauncher()
{
    const int screenWidth = 1024;
    const int screenHeight = 640;

    current_scene = Scene(screenWidth, screenHeight);
    current_scene.addObjectToScene(new Box2D(32, 32, 32, 32));
    current_scene.addObjectToScene(new Box2D(64, 32, 32, 32));
    current_scene.addObjectToScene(new Box2D(96, 32, 32, 32));
    current_scene.addObjectToScene(new Box2D(128, 32, 32, 32));
}

void GameLauncher::Launch()
{
    InitWindow(current_scene.width, current_scene.height, "raylib [core] example - basic window");

    SetTargetFPS(60); 
    draw();

    CloseWindow();
}

void GameLauncher::draw()
{

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto obj : current_scene.objects) {
            obj->Draw();
        }

        EndDrawing();
    }
}

void GameLauncher::update()
{

}
