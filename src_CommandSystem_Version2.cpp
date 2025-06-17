#include "CommandSystem.h"
#include <iostream>

bool CommandSystem::handle(const std::string& cmdline, float* playerPos) {
    std::istringstream iss(cmdline);
    std::string cmd;
    iss >> cmd;
    if (cmd == "exit" || cmd == "quit") return false;
    if (cmd == "procedural" || cmd == "regenerate" || cmd == "worldgen") {
        m_world.clearAndRegenerate();
        std::cout << "[SYSTEM] High procedural world generation triggered!\n";
        return true;
    }
    // ... other commands
    return true;
}