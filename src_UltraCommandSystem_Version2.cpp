#include "UltraProceduralWorld.h"
#include <iostream>
#include <sstream>
#include <algorithm>

extern UltraProceduralWorld g_ultraWorld;

void HandleChatCommand(const std::string& input, float* playerPos) {
    std::string cmd = input;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "/procedural" || cmd == "/regenerateworld" || cmd == "/worldgen") {
        g_ultraWorld.ultraProceduralRegenerate();
        std::cout << "[SYSTEM] Ultra-detailed procedural world regeneration triggered!\n";
    }
    // ... other chat commands ...
}