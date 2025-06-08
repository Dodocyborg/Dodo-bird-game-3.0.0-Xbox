#pragma once
#include <glm/vec3.hpp>
#include <random>
#include <vector>
#include <string>

enum BiomeType { PasitoMeadow, PasitoForest, PasitoLake, PasitoHills };

struct Biome {
    BiomeType type;
    glm::vec3 color;
    std::vector<std::string> decorations; // e.g., "Giant Pasito Flower", "Patterned Rock"
};

class ProceduralBiomeGenerator {
public:
    Biome getBiomeForPosition(float x, float z);
    void decorateChunk(float chunkX, float chunkZ, std::vector<std::string>& outDecorations);
};