#include "utils.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Check if two spheres collide
bool checkCollision(const glm::vec3& position1, const glm::vec3& position2, float radius1, float radius2) {
    float distance = glm::length(position1 - position2);
    return distance <= (radius1 + radius2);
}

// Check if two AABBs collide
bool checkCollisionAABB(const glm::vec3& min1, const glm::vec3& max1, const glm::vec3& min2, const glm::vec3& max2) {
    return (min1.x <= max2.x && max1.x >= min2.x) &&
           (min1.y <= max2.y && max1.y >= min2.y) &&
           (min1.z <= max2.z && max1.z >= min2.z);
}

// Check if two OBBs collide using Separating Axis Theorem (SAT)
bool checkCollisionOBB(const glm::mat4& transform1, const glm::mat4& transform2, const glm::vec3& halfExtents1, const glm::vec3& halfExtents2) {
    glm::vec3 axes[] = {
        glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1),
        glm::vec3(transform1[0][0], transform1[0][1], transform1[0][2]),
        glm::vec3(transform1[1][0], transform1[1][1], transform1[1][2]),
        glm::vec3(transform1[2][0], transform1[2][1], transform1[2][2]),
        glm::vec3(transform2[0][0], transform2[0][1], transform2[0][2]),
        glm::vec3(transform2[1][0], transform2[1][1], transform2[1][2]),
        glm::vec3(transform2[2][0], transform2[2][1], transform2[2][2])
    };

    // Check all axes for separating axis theorem (SAT)
    for (const glm::vec3& axis : axes) {
        glm::vec3 absAxis = glm::abs(axis);

        float projection1 = halfExtents1.x * absAxis.x + halfExtents1.y * absAxis.y + halfExtents1.z * absAxis.z;
        float projection2 = halfExtents2.x * absAxis.x + halfExtents2.y * absAxis.y + halfExtents2.z * absAxis.z;

        glm::vec3 center1 = glm::vec3(transform1[3][0], transform1[3][1], transform1[3][2]);
        glm::vec3 center2 = glm::vec3(transform2[3][0], transform2[3][1], transform2[3][2]);

        float distance = glm::dot(axis, center2 - center1);
        if (fabs(distance) > projection1 + projection2) {
            return false; // Separating axis found, no collision
        }
    }

    return true; // No separating axis found, collision detected
}