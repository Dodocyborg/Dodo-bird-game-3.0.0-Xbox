#pragma once
#include <string>

class AuthManager {
public:
    void login(const std::string& payload, std::string& response);
    bool validateToken(const std::string& token);
};