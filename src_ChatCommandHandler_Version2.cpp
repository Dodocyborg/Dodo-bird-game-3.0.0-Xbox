#include "ProceduralWorld.h"
#include <string>
#include <algorithm>

// Assume you have access to your ProceduralWorld instance:
extern ProceduralWorld g_proceduralWorld;

void HandleChatCommand(const std::string& input) {
    std::string cmd = input;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "/procedural" || cmd == "/regenerateworld" || cmd == "/worldgen") {
        g_proceduralWorld.clearAndRegenerate();
        // Optionally, send a chat message back to player(s)
        std::cout << "[SYSTEM] High procedural world regeneration triggered!\n";
        // Or in-game: SendSystemMessage("World regenerated!");
    }
    // ... other chat commands ...
}