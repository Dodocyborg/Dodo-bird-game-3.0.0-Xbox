#include "WorldStreamer.h"
#include <cmath>

WorldStreamer::WorldStreamer(int chunkSize, double scale, unsigned int seed, int viewDistance)
    : chunkSize(chunkSize), scale(scale), seed(seed), viewDistance(viewDistance)
{}

void WorldStreamer::update(double playerX, double playerY) {
    int centerChunkX = static_cast<int>(std::floor(playerX / (chunkSize * scale)));
    int centerChunkY = static_cast<int>(std::floor(playerY / (chunkSize * scale)));

    // Load all needed chunks within view distance
    for (int dx = -viewDistance; dx <= viewDistance; ++dx) {
        for (int dy = -viewDistance; dy <= viewDistance; ++dy) {
            int chunkX = centerChunkX + dx;
            int chunkY = centerChunkY + dy;
            auto key = std::make_tuple(chunkX, chunkY);
            if (chunks.find(key) == chunks.end()) {
                loadChunk(chunkX, chunkY);
            }
        }
    }
    unloadFarChunks(centerChunkX, centerChunkY);
}

void WorldStreamer::loadChunk(int chunkX, int chunkY) {
    auto chunk = std::make_unique<TerrainChunk>(chunkX, chunkY, chunkSize, scale, seed);
    chunk->generate();
    chunks[std::make_tuple(chunkX, chunkY)] = std::move(chunk);
}

void WorldStreamer::unloadFarChunks(int centerChunkX, int centerChunkY) {
    std::vector<std::tuple<int, int>> toUnload;
    for (const auto& pair : chunks) {
        int x = std::get<0>(pair.first);
        int y = std::get<1>(pair.first);
        if (abs(x - centerChunkX) > viewDistance || abs(y - centerChunkY) > viewDistance) {
            toUnload.push_back(pair.first);
        }
    }
    for (const auto& key : toUnload) {
        chunks.erase(key);
    }
}

const TerrainChunk* WorldStreamer::getChunk(int chunkX, int chunkY) const {
    auto it = chunks.find(std::make_tuple(chunkX, chunkY));
    if (it != chunks.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<const TerrainChunk*> WorldStreamer::getLoadedChunks() const {
    std::vector<const TerrainChunk*> out;
    for (const auto& pair : chunks) {
        out.push_back(pair.second.get());
    }
    return out;
}