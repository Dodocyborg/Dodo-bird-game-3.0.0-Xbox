#ifndef PLAYER_H
#define PLAYER_H
class Player {
public:
    glm::vec3 position;
    float speed = 5.0f;
    bool isShooting = false;

    void move(float deltaTime);
    void shoot();
    void render();
};

extern Player player;

#endif