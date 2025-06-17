#include "ProceduralWorld.h"
#include <sstream>
#include <cmath>

ProceduralWorld::ProceduralWorld(Renderer& r) : renderer(r) {
    // Initialize biomes, noise, etc.
    biomes.push_back({"Plains", 1.0f, 0.5f, 0.6f, {0.4f, 0.8f, 0.3f, 1}});
    biomes.push_back({"Cathedral", 2.7f, 0.3f, 0.5f, {0.9f, 0.9f, 0.92f, 1}});
    biomes.push_back({"Alien", 3.1f, 0.1f, 0.9f, {0.3f, 1, 0.7f, 1}});
    // ...
}

void ProceduralWorld::update(const float* playerPos) {
    updateVisibleChunks(playerPos);
}

void ProceduralWorld::render() {
    renderer.setCamera(cameraView, cameraProj);
    for (auto& kv : loadedChunks) {
        Chunk& c = kv.second;
        renderer.drawProceduralChunk(c.vertices, c.indices, c.modelMatrix, c.textureID, c.biomeLighting, c.fogDensity);
    }
}

void ProceduralWorld::setCamera(const float* viewMat, const float* projMat) {
    std::copy(viewMat, viewMat + 16, cameraView);
    std::copy(projMat, projMat + 16, cameraProj);
}

void ProceduralWorld::updateVisibleChunks(const float* playerPos) {
    int px = static_cast<int>(std::floor(playerPos[0] / 16.0f));
    int pz = static_cast<int>(std::floor(playerPos[2] / 16.0f));
    for (int dx = -4; dx <= 4; ++dx) {
        for (int dz = -4; dz <= 4; ++dz) {
            int cx = px + dx, cz = pz + dz;
            std::ostringstream key; key << cx << "," << cz;
            if (loadedChunks.find(key.str()) == loadedChunks.end())
                generateChunk(cx, cz);
        }
    }
}

void ProceduralWorld::generateChunk(int cx, int cz) {
    // Use Perlin/Simplex noise for height
    // Cathedral/alien biomes: vertical arches, spires, etc.
    // Fill Chunk struct: mesh vertices, indices, biome, lighting, etc.
    // For brevity, mesh generation code omitted, but should be full 3D, vertical, high quality
    Chunk c;
    c.x = cx; c.z = cz;
    // ...generate vertices/indices/modelMatrix/biome/texture/lighting/fog...
    std::ostringstream key; key << cx << "," << cz;
    loadedChunks[key.str()] = c;
}