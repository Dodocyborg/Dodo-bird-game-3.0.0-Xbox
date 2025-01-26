{#include "ProceduralTerrain.h"
#include <cmath>

// Perlin Noise Function (3D)
float ProceduralTerrain::perlinNoise3D(float x, float y, float z) {
    // Add a simple Perlin noise algorithm here (implementation details omitted for brevity)
    return sin(x * 0.1f) + cos(y * 0.1f) + sin(z * 0.1f);
}

void ProceduralTerrain::generateTerrain() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < HEIGHT; ++z) {
            float height = perlinNoise3D(x, 0, z);
            terrainData[x][z] = height;
        }
    }
    std::cout << "Terrain generated!" << std::endl;
}