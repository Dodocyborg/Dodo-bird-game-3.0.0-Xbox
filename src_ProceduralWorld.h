#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include "Renderer.h"

struct Biome {
    std::string name;
    float cathedralHeight;
    float humidity;
    float temp;
    DirectX::XMFLOAT4 color;
    // ...other biome visual/material data
};

struct Chunk {
    int x, z;
    std::vector<float> vertices; // 3D positions, normals, uvs, etc.
    std::vector<uint32_t> indices;
    int biomeIndex;
    float modelMatrix[16];
    int textureID;
    float biomeLighting;
    float fogDensity;
};

class ProceduralWorld {
public:
    ProceduralWorld(Renderer& renderer);
    void update(const float* playerPos);
    void render();
    void setCamera(const float* viewMat, const float* projMat);

private:
    void generateChunk(int cx, int cz);
    void updateVisibleChunks(const float* playerPos);
    std::unordered_map<std::string, Chunk> loadedChunks;
    std::vector<Biome> biomes;
    Renderer& renderer;
    float cameraView[16], cameraProj[16];
};