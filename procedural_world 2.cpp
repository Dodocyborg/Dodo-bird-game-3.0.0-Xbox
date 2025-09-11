#include "noise.h" // Perlin/Simplex noise implementation

// Example: Generate terrain height for a given position
float getTerrainHeight(float x, float z) {
    float scale = 0.01f;
    float height = perlinNoise(x * scale, z * scale) * 100.0f; // 100 is max height
    return height;
}

// Example: Generate a chunk of terrain
void generateTerrainChunk(int chunkX, int chunkZ) {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            float worldX = chunkX * CHUNK_SIZE + x;
            float worldZ = chunkZ * CHUNK_SIZE + z;
            float height = getTerrainHeight(worldX, worldZ);
            // Create vertex at (worldX, height, worldZ)
        }
    }
}