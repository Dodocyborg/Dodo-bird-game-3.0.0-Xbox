#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "ProceduralTerrain.h"
#include "NetworkManager.h"
#include "InputManager.h"
#include "Renderer.h"

class Game {
public:
    void initialize();
    void processInput();
    void update();
    void render();
    void cleanup();
    bool isRunning() const;

private:
    bool running = true;
    Player player;
    EnemyManager enemyManager;
    BulletManager bulletManager;
    ProceduralTerrain terrain;
    NetworkManager network;
    InputManager input;
    Renderer renderer;
};