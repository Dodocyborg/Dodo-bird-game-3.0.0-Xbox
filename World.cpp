// Windows.h
#include "Windows.h"  // Ensure this is first to avoid conflicts with other headers

// Server.cpp
#include "winsock2.h"

// Player.cpp
#include "GLFW/glew.h"using json = nlohmann::json;

std::unordered_map<glm::vec3, Chunk> loadedChunks;
std::vector<Block> worldBlocks;

bool CanDeleteBlock(const Block& block) {
    return block.type != IMPORTANT_BLOCK_TYPE;
}

bool RayIntersectsBlock(const Ray& ray, const Block& block, float& distance) {
    glm::vec3 min = block.position - glm::vec3(0.5f); 
    glm::vec3 max = block.position + glm::vec3(0.5f);

    float tmin = (min.x - ray.origin.x) / ray.direction.x;
    float tmax = (max.x - ray.origin.x) / ray.direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min.y - ray.origin.y) / ray.direction.y;
    float tymax = (max.y - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (min.z - ray.origin.z) / ray.direction.z;
    float tzmax = (max.z - ray.origin.z) / ray.direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    distance = tmin;
    return true;
}

void DeleteBlockAtRayIntersection(const Ray& ray) {
    float nearestDistance = FLT_MAX;
    Block nearestBlock;

    for (auto& block : worldBlocks) {
        float distance = 0;
        if (RayIntersectsBlock(ray, block, distance)) {
            if (distance < nearestDistance) {
                nearestDistance = distance;
                nearestBlock = block;
            }
        }
    }

    if (nearestDistance != FLT_MAX) {
        if (CanDeleteBlock(nearestBlock)) {
            auto it = std::find(worldBlocks.begin(), worldBlocks.end(), nearestBlock);
            if (it != worldBlocks.end()) {
                worldBlocks.erase(it);
            }
            std::cout << "Block deleted at: " << nearestBlock.position.x << ", "
                      << nearestBlock.position.y << ", " << nearestBlock.position.z << std::endl;
        } else {
            std::cout << "Cannot delete important block at: " << nearestBlock.position.x << ", "
                      << nearestBlock.position.y << ", " << nearestBlock.position.z << std::endl;
        }
    }
}

void SaveWorldToFile(const std::string& filename) {
    json worldData;
    for (const auto& block : worldBlocks) {
        json blockData = {
            {"x", block.position.x},
            {"y", block.position.y},
            {"z", block.position.z},
            {"type", block.type}
        };
        worldData.push_back(blockData);
    }

    std::ofstream file(filename);
    file << worldData.dump(4);
    file.close();

    std::cout << "World saved to " << filename << std::endl;
}

void LoadWorldFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error loading world file." << std::endl;
        return;
    }

    json worldData;
    file >> worldData;
    file.close();

    worldBlocks.clear();
    for (const auto& blockData : worldData) {
        Block block;
        block.position = glm::vec3(blockData["x"], blockData["y"], blockData["z"]);
        block.type = blockData["type"];
        worldBlocks.push_back(block);
    }

    std::cout << "World loaded from " << filename << std::endl;
}