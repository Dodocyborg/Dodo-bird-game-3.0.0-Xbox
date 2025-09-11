#include <iostream>
#include "WorldStreamer.h"

int main() {
    unsigned int seed;
    std::cout << "Enter world seed: ";
    std::cin >> seed;

    const int chunkSize = 64;
    const double scale = 0.04;
    const int viewDistance = 2; // How many chunks in each direction to load

    WorldStreamer world(chunkSize, scale, seed, viewDistance);

    double playerX = 0.0, playerY = 0.0;

    std::cout << "Use WASD to move, Q to quit.\n";
    char cmd;
    do {
        world.update(playerX, playerY);

        // Show loaded chunk coordinates
        std::cout << "Player at (" << playerX << ", " << playerY << ")\nLoaded chunks: ";
        for (const auto* chunk : world.getLoadedChunks()) {
            std::cout << "(" << chunk->getChunkX() << "," << chunk->getChunkY() << ") ";
        }
        std::cout << "\n";

        // Print center chunk's heightmap ASCII preview
        int px = static_cast<int>(std::floor(playerX / (chunkSize * scale)));
        int py = static_cast<int>(std::floor(playerY / (chunkSize * scale)));
        const TerrainChunk* center = world.getChunk(px, py);
        if (center) {
            const auto& heights = center->getHeights();
            for (int y = 0; y < chunkSize; ++y) {
                for (int x = 0; x < chunkSize; ++x) {
                    double h = heights[x][y];
                    if (h < 0.35)
                        std::cout << "~";
                    else if (h < 0.65)
                        std::cout << ".";
                    else
                        std::cout << "^";
                }
                std::cout << "\n";
            }
        }

        std::cout << "Command (WASD/Q): ";
        std::cin >> cmd;
        switch (cmd) {
            case 'w': playerY -= chunkSize * scale / 2; break;
            case 's': playerY += chunkSize * scale / 2; break;
            case 'a': playerX -= chunkSize * scale / 2; break;
            case 'd': playerX += chunkSize * scale / 2; break;
        }
    } while (cmd != 'q' && cmd != 'Q');

    std::cout << "Goodbye!\n";
    return 0;
}