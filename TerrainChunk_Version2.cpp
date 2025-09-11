#include "TerrainChunk.h"
#include "PerlinNoise.h"

TerrainChunk::TerrainChunk(int chunkX, int chunkY, int size, double scale, unsigned int seed)
    : chunkX(chunkX), chunkY(chunkY), size(size), scale(scale), seed(seed)
{
    heights.resize(size, std::vector<double>(size, 0.0));
}

void TerrainChunk::generate() {
    PerlinNoise perlin(seed);

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            double worldX = (chunkX * size + x) * scale;
            double worldY = (chunkY * size + y) * scale;

            // Multi-octave Perlin for richer terrain
            double noise = 0.0;
            double frequency = 1.0;
            double amplitude = 1.0;
            double maxValue = 0.0;
            for (int octave = 0; octave < 6; ++octave) {
                noise += perlin.noise(worldX * frequency, worldY * frequency) * amplitude;
                maxValue += amplitude;
                amplitude *= 0.5;
                frequency *= 2.0;
            }
            heights[x][y] = noise / maxValue;
        }
    }
}

const std::vector<std::vector<double>>& TerrainChunk::getHeights() const {
    return heights;
}