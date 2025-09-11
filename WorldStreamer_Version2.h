#pragma once
#include "TerrainChunk.h"
#include <unordered_map>
#include <memory>
#include <tuple>

class WorldStreamer {
public:
    WorldStreamer(int chunkSize, double scale, unsigned int seed, int viewDistance);

    // Call every frame/update with player position
    void update(double playerX, double playerY);

    // Get chunk pointer (returns nullptr if not loaded)
    const TerrainChunk* getChunk(int chunkX, int chunkY) const;

    // Get all loaded chunks
    std::vector<const TerrainChunk*> getLoadedChunks() const;

private:
    int chunkSize;
    double scale;
    unsigned int seed;
    int viewDistance;

    // Map: (chunkX, chunkY) -> chunk
    std::unordered_map<std::tuple<int, int>, std::unique_ptr<TerrainChunk>> chunks;

    void loadChunk(int chunkX, int chunkY);
    void unloadFarChunks(int centerChunkX, int centerChunkY);
};