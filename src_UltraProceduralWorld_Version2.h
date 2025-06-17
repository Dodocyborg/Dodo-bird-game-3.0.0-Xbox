#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "Renderer.h"

struct Biome {
    std::string name;
    float baseHeight;
    float roughness;
    float humidity;
    float temperature;
    float color[4];
    // Advanced biome parameters:
    float floraDensity;
    float caveFrequency;
    float detailNoiseScale;
    float cliffSharpness;
};

struct Chunk {
    int x, z;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    int biomeIndex;
    float modelMatrix[16];
    int textureID;
    float biomeLighting;
    float fogDensity;
    std::vector<float> floraInstances; // For trees, flowers, etc.
    std::vector<float> caveData;       // For cave mesh
};

class UltraProceduralWorld {
public:
    UltraProceduralWorld(Renderer& renderer);
    void update(const float* playerPos);
    void render();
    void generateChunk(int cx, int cz, bool force = false);
    void setBiomeOverride(const std::string& name);
    void clearAndRegenerate();
    void ultraProceduralRegenerate(); // Triggers ultra-detailed worldgen

private:
    void updateVisibleChunks(const float* playerPos);
    std::unordered_map<std::string, Chunk> loadedChunks;
    std::vector<Biome> biomes;
    std::string biomeOverride;
    Renderer& renderer;
    float cameraView[16], cameraProj[16];

    // Ultra-detailed procedural helpers:
    float getHeight(int x, int z, const Biome& biome) const;
    void generateFlora(Chunk& chunk, const Biome& biome) const;
    void generateCaves(Chunk& chunk, const Biome& biome) const;
};