#include "ProceduralTerrain.h"
#include <iostream>

class TerrainChunk {
public:
    TerrainChunk(int chunkX, int chunkY, int size, double scale, unsigned int seed)
        : chunkX(chunkX), chunkY(chunkY), size(size), scale(scale), perlin(seed) {}

    // Generate height data for this chunk
    std::vector<std::vector<double>> generateHeights() {
        std::vector<std::vector<double>> heights(size, std::vector<double>(size));
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                double worldX = (chunkX * size + x) * scale;
                double worldY = (chunkY * size + y) * scale;
                heights[x][y] = perlin.noise(worldX, worldY);
            }
        }
        return heights;
    }
private:
    int chunkX, chunkY, size;
    double scale;
    PerlinNoise perlin;
};

// Example usage
int main() {
    unsigned int seed;
    std::cout << "Enter world seed: ";
    std::cin >> seed;

    TerrainChunk chunk(0, 0, 32, 0.1, seed);
    auto heights = chunk.generateHeights();

    // Print a simple ASCII map
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            double h = heights[x][y];
            if (h < 0.3)
                std::cout << "~"; // water
            else if (h < 0.6)
                std::cout << ".";
            else
                std::cout << "^"; // mountain
        }
        std::cout << "\n";
    }
    return 0;
}