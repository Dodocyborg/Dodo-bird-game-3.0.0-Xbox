#include "game_logic.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include Windows.h
#include networking.h
    #include <Windows.h
    #include <glm.hpp
#include <winsock2.h
void gameLogic(float deltaTime) {
    player.move(deltaTime);
    player.shoot();

    if (player.isShooting) {
        Bullet bullet;
        bullet.position = player.position;
        bullet.direction = glm::vec3(0.0f, 0.0f, -1.0f);
        bullets.push_back(bullet);
    }

    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }

    for (auto& enemy : enemies) {
        enemy.update(deltaTime);
        for (auto& bullet : bullets) {
            if (bullet.checkCollision(enemy)) {
                enemy.position = glm::vec3(-1000.0f, -1000.0f, -1000.0f); // Destroy enemy
                bullet.position = glm::vec3(-1000.0f, -1000.0f, -1000.0f); // Destroy bullet
            }
        }
    }
}

void recordCommands() {
    // Vulkan command recording for rendering
}

void renderGameObjects() {
    player.render();
    for (const auto& bullet : bullets) {
        bullet.render();
    }
    for (const auto& enemy : enemies) {
        enemy.render();
    }
}

void presentFrame() {
    // Present the frame in Vulkan
}