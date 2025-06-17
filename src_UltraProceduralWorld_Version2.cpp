#include "UltraProceduralWorld.h"
#include <sstream>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <random>

UltraProceduralWorld::UltraProceduralWorld(Renderer& r) : renderer(r) {
    // Ultra-detailed biomes
    biomes.push_back({"Plains", 0.5f, 0.2f, 0.6f, 0.5f, {0.5f, 0.85f, 0.3f, 1}, 0.8f, 0.1f, 0.03f, 0.1f});
    biomes.push_back({"Cathedral Mountains", 2.0f, 1.7f, 0.2f, 0.3f, {0.85f, 0.85f, 0.88f, 1}, 0.2f, 0.4f, 0.09f, 0.7f});
    biomes.push_back({"Jungle", 1.2f, 0.7f, 0.95f, 0.7f, {0.1f, 0.7f, 0.17f, 1}, 1.0f, 0.05f, 0.06f, 0.2f});
    biomes.push_back({"Desert", 0.3f, 0.1f, 0.05f, 1.0f, {0.96f, 0.92f, 0.6f, 1}, 0.05f, 0.01f, 0.04f, 0.05f});
    biomes.push_back({"Alien", 1.8f, 1.5f, 0.15f, 1.3f, {0.32f, 0.8f, 1.0f, 1}, 0.18f, 0.25f, 0.12f, 0.6f});
    biomeOverride = "";
}

void UltraProceduralWorld::update(const float* playerPos) {
    updateVisibleChunks(playerPos);
}

void UltraProceduralWorld::render() {
    for (auto& kv : loadedChunks) {
        Chunk& c = kv.second;
        renderer.drawProceduralChunk(c.vertices, c.indices, c.modelMatrix, c.textureID, c.biomeLighting, c.fogDensity);
        // Optionally, render flora, caves, etc.
    }
}

void UltraProceduralWorld::setBiomeOverride(const std::string& name) {
    biomeOverride = name;
}

void UltraProceduralWorld::clearAndRegenerate() {
    loadedChunks.clear();
}

void UltraProceduralWorld::ultraProceduralRegenerate() {
    clearAndRegenerate();
    // Optionally: re-seed random, shuffle biomes, ramp up detail
}

void UltraProceduralWorld::updateVisibleChunks(const float* playerPos) {
    int px = static_cast<int>(std::floor(playerPos[0] / 16.0f));
    int pz = static_cast<int>(std::floor(playerPos[2] / 16.0f));
    for (int dx = -3; dx <= 3; ++dx) {
        for (int dz = -3; dz <= 3; ++dz) {
            int cx = px + dx, cz = pz + dz;
            std::ostringstream key; key << cx << "," << cz;
            if (loadedChunks.find(key.str()) == loadedChunks.end())
                generateChunk(cx, cz);
        }
    }
}

void UltraProceduralWorld::generateChunk(int cx, int cz, bool force) {
    std::ostringstream key; key << cx << "," << cz;
    if (!force && loadedChunks.find(key.str()) != loadedChunks.end()) return;

    // Select biome
    int biomeIdx = 0;
    if (!biomeOverride.empty()) {
        auto it = std::find_if(biomes.begin(), biomes.end(),
            [&](const Biome& b) { return b.name == biomeOverride; });
        if (it != biomes.end()) biomeIdx = std::distance(biomes.begin(), it);
    } else {
        biomeIdx = std::abs((cx * 73856093 + cz * 19349669) % biomes.size());
    }
    const Biome& biome = biomes[biomeIdx];

    Chunk c;
    c.x = cx; c.z = cz; c.biomeIndex = biomeIdx;
    c.textureID = biomeIdx;
    c.biomeLighting = 0.3f + 0.6f * biome.humidity;
    c.fogDensity = 0.02f + 0.03f * biome.humidity;
    std::memset(c.modelMatrix, 0, sizeof(c.modelMatrix));
    c.modelMatrix[0] = c.modelMatrix[5] = c.modelMatrix[10] = c.modelMatrix[15] = 1.0f;
    c.modelMatrix[12] = cx * 16.0f;
    c.modelMatrix[14] = cz * 16.0f;

    // Advanced procedural terrain: 3D terrain using layers of noise
    for (int x = 0; x <= 16; ++x) {
        for (int z = 0; z <= 16; ++z) {
            float wx = static_cast<float>(cx * 16 + x);
            float wz = static_cast<float>(cz * 16 + z);
            float h = getHeight(wx, wz, biome);
            // Normals: calculate using height differences (for real: sample neighbors)
            float normal[3] = {0, 1, 0};
            c.vertices.push_back(x);
            c.vertices.push_back(h);
            c.vertices.push_back(z);
            c.vertices.insert(c.vertices.end(), normal, normal + 3);
            c.vertices.push_back(x / 16.0f);
            c.vertices.push_back(z / 16.0f);
        }
    }
    // Indices for grid mesh
    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            int i0 = x * 17 + z;
            int i1 = (x + 1) * 17 + z;
            int i2 = (x + 1) * 17 + (z + 1);
            int i3 = x * 17 + (z + 1);
            c.indices.insert(c.indices.end(), {i0, i1, i2, i0, i2, i3});
        }
    }
    // Ultra-detailed: add flora and caves
    generateFlora(c, biome);
    generateCaves(c, biome);

    loadedChunks[key.str()] = c;
}

float UltraProceduralWorld::getHeight(int x, int z, const Biome& biome) const {
    // Combine multiple octaves of Perlin/simplex noise for ultra detail
    float n = 0.0f, amp = 1.0f, freq = biome.detailNoiseScale, totalAmp = 0.0f;
    for (int o = 0; o < 6; ++o) {
        float nx = x * freq, nz = z * freq;
        float noise = std::sin(nx * 0.05f + o) * std::cos(nz * 0.07f - o); // Replace with real noise
        n += noise * amp;
        totalAmp += amp;
        amp *= 0.5f;
        freq *= 2.0f;
    }
    n /= totalAmp;
    n = std::pow(std::abs(n), biome.cliffSharpness); // sharper cliffs for some biomes
    float base = biome.baseHeight + biome.roughness * n * 8.0f;
    return base;
}

void UltraProceduralWorld::generateFlora(Chunk& chunk, const Biome& biome) const {
    // Place flora (trees, flowers) based on biome.floraDensity
    std::mt19937 rng(chunk.x * 2387 + chunk.z * 4871);
    std::uniform_real_distribution<float> dist(0, 1);
    for (int i = 0; i < 20 * biome.floraDensity; ++i) {
        float fx = dist(rng) * 16.0f;
        float fz = dist(rng) * 16.0f;
        float height = getHeight(chunk.x * 16 + fx, chunk.z * 16 + fz, biome);
        chunk.floraInstances.push_back(fx);
        chunk.floraInstances.push_back(height);
        chunk.floraInstances.push_back(fz);
    }
}

void UltraProceduralWorld::generateCaves(Chunk& chunk, const Biome& biome) const {
    // Caves: for real games, use 3D noise and mesh marching; demo: random points
    std::mt19937 rng(chunk.x * 4751 + chunk.z * 3301);
    std::uniform_real_distribution<float> dist(0, 1);
    for (int i = 0; i < 8 * biome.caveFrequency; ++i) {
        float cx = dist(rng) * 16.0f;
        float cz = dist(rng) * 16.0f;
        float cy = 1.0f + dist(rng) * biome.baseHeight * 0.5f;
        chunk.caveData.push_back(cx);
        chunk.caveData.push_back(cy);
        chunk.caveData.push_back(cz);
    }
}