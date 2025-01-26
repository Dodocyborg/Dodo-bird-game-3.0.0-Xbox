#include "player.h"
#include <GLFW/glew.h>

Player player;

void Player::move(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position.z -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position.z += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position.x -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position.x += speed * deltaTime;
}

void Player::shoot() {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        isShooting = true;
    } else {
        isShooting = false;
    }
}

void Player::render() {
    // Implement Vulkan rendering for the player
}