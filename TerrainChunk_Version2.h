#pragma once
#include <vector>
#include <cstdint>

class TerrainChunk {
public:
    TerrainChunk(int chunkX, int chunkY, int size, double scale, unsigned int seed);

    void generate();
    const std::vector<std::vector<double>>& getHeights() const;

    int getChunkX() const { return chunkX; }
    int getChunkY() const { return chunkY; }
    int getSize() const { return size; }

private:
    int chunkX, chunkY;
    int size;
    double scale;
    unsigned int seed;
    std::vector<std::vector<double>> heights;
};