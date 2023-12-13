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
#include "model/ui/SoundUI.h"
#include "model/misc/AnimatedParticle.h"
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

    loadingStage = -1;

    isGameAlive = true;

    SceneManager::Instance()->setDefaultCameraSize(screenWidth, screenHeight);

    auto start_scene = std::shared_ptr<Scene>(new MenuScene(screenWidth, screenHeight));

    SceneFile sceneFile = SceneFile("base.scene");
    auto gameScene = new GameScene(screenWidth*2, screenHeight*2, "base.scene");
    gameScene->setDebugGrid(false);

    SceneManager::Instance()->setStartScene(start_scene);
    SceneManager::Instance()->AddScene(gameScene);
    SceneManager::Instance()->AddScene(new EditorScene(screenWidth*2, screenHeight*2));
}

void GameLauncher::OnStart()
{
    Block::InitBlocks();
    Item::InitItems(); 
    Arrow::InitArrows(); 
    Entity::InitEntities();
    SoundUI::InitSounds();
    AnimatedParticle::InitAnimatedEffects();

    SpriteLoader::LoadInGameSprites(); 

    SceneManager::Instance()->Start();
}

void GameLauncher::load()
{
    loadingStage = -1;
    std::string text = "Load: Blocks ...";
    while (loadingStage < 7)
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRectangle(256, 300, 512, 40, GRAY);
        DrawRectangle(256, 300, 73 * loadingStage, 40, BLUE);
        DrawText(text.c_str(), 470 - ((int)text.size() * 2), 355, 20, WHITE);
        loadingStage++;
        EndDrawing();
        switch (loadingStage)
        {
        case 0: Block::InitBlocks(); text = "Load: Items ..."; break;
        case 1: Item::InitItems(); text = "Load: Entities ..."; break;
        case 2: Entity::InitEntities();; text = "Load: Sounds ..."; break;
        case 3: SoundUI::InitSounds();; text = "Load: Misc ..."; break;
        case 4:
            Arrow::InitArrows();
            AnimatedParticle::InitAnimatedEffects();
            text = "Load: Scenes ...";
            break;
        case 5: SpriteLoader::LoadInGameSprites(); text = "Starting scene ..."; break;
        case 6: SceneManager::Instance()->Start(); text = "Game is ready!"; break;
        default:
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void GameLauncher::Launch()
{

    InitWindow(SceneManager::current->camWidth, SceneManager::current->camHeight, "The Game");
    InitAudioDevice();
    BeginDrawing();
    ClearBackground(DARKGRAY);
    EndDrawing();

    SetTargetFPS(144);
    //OnStart();

    std::thread th1(__update_thread, this);
    load();
    draw();

    CloseAudioDevice();
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

