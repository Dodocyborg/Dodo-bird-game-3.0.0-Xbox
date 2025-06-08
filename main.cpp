#include "Game.h"

int main() {
    Game game;
    game.initialize();

    // Main Game Loop
    while (game.isRunning()) {
        game.processInput();
        game.update();
        game.render();
    }

    game.cleanup();
    return 0;
}