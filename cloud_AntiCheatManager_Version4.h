#pragma once
#include <string>

class AntiCheatManager {
public:
    bool validate(const std::string& gameData);
    void reportCheat(const std::string& playerId, const std::string& reason);
};